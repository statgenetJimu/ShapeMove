/*以下は「画像処理プログラミング」に掲載のCのコードを書き換えたもので、市松模様の画像を吐き出します。
visual studio 2015では正常に動作しました。
とりあえず動けば良いと思い構造体などそのまま残っていて見苦しい限りですが、取り急ぎ。
何か問題等あればよろしくお願い致します。
岩﨑*/

#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define TRUE 1
#define FALSE 0
typedef unsigned char BYTE;

typedef	struct STRUCT_IMAGE {
public:	
	int width, height;
	int depth;
	BYTE* pixels;
}ImageData;

#define PIXELMAX 255
typedef struct STRUCT_PIXEL {
	int r, g, b;
}Pixel;

ImageData* createImage(int width, int height, int depth);
void disposeImage(ImageData *img);
int readBMPfile(char *fname, ImageData **img);
int writeBMPfile(char *fname, ImageData *img);
int getPixel(ImageData *img, int x, int y, Pixel *pix);
int setPixel(ImageData *img, int x, int y, Pixel *pix);

/* List1-2
画像データ作成
width :画像の横幅
height:画像の縦幅
depth :１画素あたりのビット数(8 or 24)
*/
ImageData* createImage(int width, int height, int depth)
{
	ImageData* newimg;
	newimg = new ImageData;
	int byte_per_pixel;

	if (width<0 || height<0) return NULL;
	if (depth != 8 && depth != 24) return NULL;  // １画素あたりのビット数(8,24以外はエラー）
	if (newimg == NULL) return NULL;
	// 1画素格納するのに必要なバイト数を求める
	byte_per_pixel = depth / 8;
	// 画像データを格納するのに必要なメモリを確保
	//newimg->pixels = malloc(sizeof(BYTE)*byte_per_pixel*width*height);
	BYTE* pixels;
	pixels = new BYTE[sizeof(BYTE)*byte_per_pixel*width*height];
	newimg->pixels = pixels;

	if (newimg->pixels == NULL) {
		free(newimg);
		return NULL;
	}
	// 各プロパティ値を設定
	newimg->width = width;
	newimg->height = height;
	newimg->depth = depth;
	return newimg;
}

/* List1-3
画像データの廃棄
*/
void disposeImage(ImageData *img)
{
	if (img->pixels != NULL) free(img->pixels);
	free(img);
	return;
}

/* List1-4
画像データ上の画素値を取得
x,y 画素の座標
pix 画素値を格納する
*/
int getPixel(ImageData *img, int x, int y, Pixel *pix)
{
	int ret = 1;
	int adr;  // 画素の画像上の位置
	int dep, val;
	BYTE *pixels;

	if (img == NULL) return -1;
	if (img->pixels == NULL) return -1;
	// 画像外の座標が指定された場合の処理（最も近い画像上の画素を参照する）
	if (x<0) {
		x = 0;
		ret = 0;
	}
	if (x >= img->width) {
		x = img->width - 1;
		ret = 0;
	}
	if (y<0) {
		y = 0;
		ret = 0;
	}
	if (y >= img->height) {
		y = img->height - 1;
		ret = 0;
	}
	dep = img->depth;
	adr = x + y*img->width;
	pixels = img->pixels;
	if (dep == 8) {  // グレースケールの場合は、RGBすべての同じ値をセットする
		val = pixels[adr];
		pix->r = val;
		pix->g = val;
		pix->b = val;
	}
	else if (dep == 24) {
		pixels += (adr * 3);
		pix->r = (*pixels);
		pixels++;
		pix->g = (*pixels);
		pixels++;
		pix->b = (*pixels);
	}
	else {
		return -1;
	}
	return ret;
}

/*
画素値の補正（範囲外の値を範囲内に収める）
*/
int correctValue(int val, int max)
{
	if (val<0) return 0;
	if (val>max) return max;
	return val;
}

/* List1-5
画像データ上の画素値を変更する
x,y 画素の座標
pix セットする画素値
*/
int setPixel(ImageData *img, int x, int y, Pixel *pix)
{
	int adr;  // 画素の画像上の位置
	int dep, val;
	BYTE *pixels;

	if (img == NULL) return -1;
	if (img->pixels == NULL) return -1;
	// 画像外の座標が指定されたらなにもしない
	if (x<0 || x >= img->width || y<0 || y >= img->height) {
		return 0;
	}
	dep = img->depth;
	adr = x + y*img->width;
	pixels = img->pixels;
	if (dep == 8) {
		pixels[adr] = correctValue(pix->r, PIXELMAX);
	}
	else if (dep == 24) {
		pixels += (adr * 3);
		(*pixels) = correctValue(pix->r, PIXELMAX);
		pixels++;
		(*pixels) = correctValue(pix->g, PIXELMAX);
		pixels++;
		(*pixels) = correctValue(pix->b, PIXELMAX);
	}
	else {
		return -1;
	}
	return 1;
}

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned short      WORD;
typedef unsigned long       LONG;

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

// BMPヘッダ部のデータ構造定義
typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPCOREHEADER {
	DWORD   bcSize;
	WORD    bcWidth;
	WORD    bcHeight;
	WORD    bcPlanes;
	WORD    bcBitCount;
} BITMAPCOREHEADER, *PBITMAPCOREHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#define MAXCOLORS 256

// ファイルより２バイト整数を書き込む（リトルエンディアン）
int fwriteWORD(WORD val, FILE *fp)
{
	int i, c;

	c = val;
	for (i = 0; i<2; i++) {
		fputc(c % 256, fp);
		c /= 256;
	}
	return TRUE;
}

// ファイルより４バイト整数を書き込む（リトルエンディアン）
int fwriteDWORD(DWORD val, FILE *fp)
{
	int i, c;

	c = val;
	for (i = 0; i<4; i++) {
		fputc(c % 256, fp);
		c /= 256;
	}
	return TRUE;
}

// ファイルより２バイト整数を読み込む（リトルエンディアン）
int freadWORD(WORD *res, FILE *fp)
{
	int i, c;
	int val[2];

	for (i = 0; i<2; i++) {
		c = fgetc(fp);
		if (c == EOF) return FALSE;
		val[i] = c;
	}
	*res = val[1] * 256 + val[0];
	return TRUE;
}

// ファイルより４バイト整数を読み込む（リトルエンディアン）
int  freadDWORD(DWORD *res, FILE *fp)
{
	int i, c;
	int val[4];
	DWORD tmp = 0;

	for (i = 0; i<4; i++) {
		c = fgetc(fp);
		if (c == EOF) return FALSE;
		val[i] = c;
	}
	tmp = 0;
	for (i = 3; i >= 0; i--) {
		tmp *= 256;
		tmp += val[i];
	}
	*res = tmp;
	return TRUE;
}

// BMPの種類を判別
// 戻り値：FALSE　OS/2形式
//           TRUE   WIndows形式
static BOOL	IsWinDIB(BITMAPINFOHEADER* pBIH)
{
	if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER)) {
		return FALSE;
	}
	return TRUE;
}

// パレットのサイズを取得
// iBitCount １画素あたりのビット数
int countOfDIBColorEntries(int iBitCount)
{
	int	iColors;

	switch (iBitCount) {
	case 1:
		iColors = 2;
		break;
	case 4:
		iColors = 16;
		break;
	case 8:
		iColors = 256;
		break;
	default:
		iColors = 0;
		break;
	}

	return iColors;
}

// パディング要素を考慮して１列分のバイト数を求める
int getDIBxmax(int mx, int dep)
{
	switch (dep) {
	case 32:
		return mx * 4;
	case 24:
		//return mx;
		return ((mx * 3) + 3) / 4 * 4;
		break;
	case 16:
		return (mx + 1) / 2 * 2;
		break;
	case 8:
		return (mx + 3) / 4 * 4;
		break;
	case 4:
		return (((mx + 1) / 2) + 3) / 4 * 4;
		break;
	case 1:
		return (((mx + 7) / 8) + 3) / 4 * 4;
	}
	return mx;
}

// List1-6
// BMPデータをファイルより読み込む
int readBMPfile(char *fname, ImageData **img)
{
	int i, c;
	int errcode = 0;
	BITMAPFILEHEADER BMPFile;
	int	fsize;
	BITMAPINFOHEADER BMPInfo;
	BITMAPCOREHEADER BMPCore;
	int	colors;
	int	colorTableSize;
	int	bitsSize;
	int	BISize;
	int x, y;
	int mx, my, depth;
	int pad;
	int mxb, myb;
	int isPM = FALSE;	// BMPの形式を記録するフラグ
	FILE *fp;

	WORD    HEAD_bfType;
	DWORD   HEAD_bfSize;
	WORD    HEAD_bfReserved1;
	WORD    HEAD_bfReserved2;
	DWORD   HEAD_bfOffBits;
	DWORD   INFO_bfSize;
	Pixel palet[MAXCOLORS];
	Pixel setcolor;

	errno_t error;
	if(error=fopen_s(&fp, fname, "rb") != 0){
		return -1;
	}

	// BMPファイルは必ず'BM(0x4d42)'で始まる．それ以外の場合はBMPではないので，中止する
	if (!freadWORD(&HEAD_bfType, fp)) {
		errcode = -2;
		goto $ABORT;
	}
	if (HEAD_bfType != 0x4d42) {
		errcode = -10;
		goto $ABORT;
	}
	// ヘッダ部のサイズ(Byte)
	if (!freadDWORD(&HEAD_bfSize, fp)) {
		errcode = -10;
		goto $ABORT;
	}
	// 予約用領域（未使用）
	if (!freadWORD(&HEAD_bfReserved1, fp)) {
		errcode = -10;
		goto $ABORT;
	}
	// 予約用領域（未使用）
	if (!freadWORD(&HEAD_bfReserved2, fp)) {
		errcode = -10;
		goto $ABORT;
	}
	// オフセット
	if (!freadDWORD(&HEAD_bfOffBits, fp)) {
		errcode = -10;
		goto $ABORT;
	}
	// ヘッダ部のサイズ
	if (!freadDWORD(&INFO_bfSize, fp)) {
		errcode = -10;
		goto $ABORT;
	}

	// ヘッダ部のサイズが規定外ならばエラーとする
	if (INFO_bfSize == 40/*sizeof(BITMAPINFOHEADER)*/ || INFO_bfSize == 12/*sizeof(BITMAPCOREHEADER)*/) {
		BMPInfo.biSize = INFO_bfSize;
		// BITMAPCOREHEADER形式の場合
		if (INFO_bfSize == sizeof(BITMAPCOREHEADER)) {
			WORD tmp;
			isPM = TRUE;
			// 画像の横幅
			if (!freadWORD(&tmp, fp)) {
				errcode = -10;
				goto $ABORT;
			}
			BMPInfo.biWidth = tmp;
			// 画像の縦幅
			if (!freadWORD(&tmp, fp)) {
				errcode = -10;
				goto $ABORT;
			}
			BMPInfo.biHeight = tmp;
			// 画像のプレーン数
			if (!freadWORD(&(BMPInfo.biPlanes), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// １画素あたりのビット数
			if (!freadWORD(&(BMPInfo.biBitCount), fp)) {
				errcode = -10;
				goto $ABORT;
			}
		}
		else {		// BITMAPINFOHEADER形式の場合
					// 画像の横幅
			if (!freadDWORD(&(BMPInfo.biWidth), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// 画像の縦幅
			if (!freadDWORD(&(BMPInfo.biHeight), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// 画像のプレーン数
			if (!freadWORD(&(BMPInfo.biPlanes), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// １画素あたりのビット数
			if (!freadWORD(&(BMPInfo.biBitCount), fp)) {
				errcode = -10;
				goto $ABORT;
			}
		}
		// BITMAPINFOHEADERの場合のみ存在する情報を読み込む
		if (!isPM) {
			// 圧縮形式
			if (!freadDWORD(&(BMPInfo.biCompression), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// 画像データ部のサイズ
			if (!freadDWORD(&(BMPInfo.biSizeImage), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// X方向の解像度
			if (!freadDWORD(&(BMPInfo.biXPelsPerMeter), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// Y方向の解像度
			if (!freadDWORD(&(BMPInfo.biYPelsPerMeter), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// 格納されているパレットの色数
			if (!freadDWORD(&(BMPInfo.biClrUsed), fp)) {
				errcode = -10;
				goto $ABORT;
			}
			// 重要なパレットのインデックス
			if (!freadDWORD(&(BMPInfo.biClrImportant), fp)) {
				errcode = -10;
				goto $ABORT;
			}
		}
	}
	else {
		errcode = -10;
		goto $ABORT;
	}
	mx = BMPInfo.biWidth;
	my = BMPInfo.biHeight;
	depth = BMPInfo.biBitCount;
	// 256色，フルカラー以外サポート外
	if (depth != 8 && depth != 24) {
		errcode = -3;
		goto $ABORT;
	}
	// 非圧縮形式以外はサポート外
	if (BMPInfo.biCompression != BI_RGB) {
		errcode = -20;
		goto $ABORT;
	}
	// ヘッダ部にパレットサイズの情報がない場合は１画素あたりのビット数から求める
	if (BMPInfo.biClrUsed == 0) {
		colors = countOfDIBColorEntries(BMPInfo.biBitCount);
	}
	else {
		colors = BMPInfo.biClrUsed;
	}

	// パレット情報の読み込み
	// BMPの種類によってフォーマットが異なるので処理をわける
	if (!isPM) {
		for (i = 0; i<colors; i++) {
			// Blue成分
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].b = c;
			// Green成分
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].g = c;
			// Red成分
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].r = c;
			// あまり
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
		}
	}
	else {
		for (i = 0; i<colors; i++) {
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].b = c;
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].g = c;
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -10;
				goto $ABORT;
			}
			palet[i].r = c;
		}
	}
	// フルカラーで画像データを作成
	*img = createImage(mx, my, 24);
	mxb = getDIBxmax(mx, depth);
	pad = mxb - mx*depth / 8;
	// 画像データ読み込み
	for (y = my - 1; y >= 0; y--) {
		for (x = 0; x<mx; x++) {
			if (depth == 8) {	// 256色形式の場合はパレットからRGB値を求める
				c = fgetc(fp);
				if (c == EOF) {
					errcode = -20;
					goto $ABORT;
				}
				setcolor.r = palet[c].r;
				setcolor.g = palet[c].g;
				setcolor.b = palet[c].b;
			}
			else if (depth == 24) {
				c = fgetc(fp);
				if (c == EOF) {
					errcode = -20;
					goto $ABORT;
				}
				setcolor.b = c;
				c = fgetc(fp);
				if (c == EOF) {
					errcode = -20;
					goto $ABORT;
				}
				setcolor.g = c;
				c = fgetc(fp);
				if (c == EOF) {
					errcode = -20;
					goto $ABORT;
				}
				setcolor.r = c;
			}
			setPixel(*img, x, y, &setcolor);
		}
		// Padding部の読み飛ばし
		for (i = 0; i<pad; i++) {
			c = fgetc(fp);
			if (c == EOF) {
				errcode = -20;
				goto $ABORT;
			}
		}
	}
$ABORT:	// エラー時の飛び先
	fclose(fp);
	return errcode;
}

//List1-7
// 画像データをBMP形式(Windows形式)でファイルに書き出す
// （フルカラーの画像データのみサポート）
int writeBMPfile(char *fname, ImageData *img)
{
	FILE *fp;
	BITMAPFILEHEADER bfn;
	int w, h, rw;
	int mxb, pad;
	/*int width;
	int height;*/
	int depth;	// １画素あたりのビット数
	int pbyte;	//１要素あたりのバイト数
	int palsize;	// パレットサイズ（未実装）
	int x, y, i;
	int saveloop, saverest;
	int	iBytes;
	unsigned int wsize;
	Pixel pix;

	w = img->width;
	h = img->height;
	depth = img->depth;

	 //フルカラー以外サポート外
	if (depth != 24) {
		//errcode=-3;
		goto $abort1;
	}

	// フルカラー以外のことを若干考慮しているが未実装．フルカラーのみなら、本来-- end -- の部分まで不要
	if (depth == 24) {
		pbyte = 1;
	}
	else {
		pbyte = depth / 8;
	}
	if (depth >= 24) {
		palsize = 0;
	}
	else {
		palsize = 256;
	}
	// -- end --

	// パディングを考慮した１列分に必要なバイト数
	rw = getDIBxmax(w, depth);
	// ヘッダ部の設定（一部のみ）
	bfn.bfType = 0x4d42;	//'BM'
	bfn.bfSize = 14 + 40 +/*sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER) +*/
		palsize * 4/*sizeof(RGBQUAD)*/ +
		rw*h*pbyte;
	bfn.bfReserved1 = 0;
	bfn.bfReserved2 = 0;
	bfn.bfOffBits = 14 + 40/*sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER)*/ +
		palsize * 4/*sizeof(RGBQUAD)*/;

	errno_t error;
	if (error = fopen_s(&fp, fname, "wb") != 0) {
		goto $abort1;
	}

	// ヘッダ部の書き出し
	fwriteWORD(bfn.bfType, fp);
	fwriteDWORD(bfn.bfSize, fp);
	fwriteWORD(bfn.bfReserved1, fp);
	fwriteWORD(bfn.bfReserved2, fp);
	fwriteDWORD(bfn.bfOffBits, fp);
	fwriteDWORD(40/*sizeof(BITMAPINFOHEADER)*/, fp);	//biSize
	fwriteDWORD(w, fp);		// biWidth
	fwriteDWORD(h, fp);		// biHeight
	fwriteWORD(1, fp);		// biPlanes
	fwriteWORD(depth, fp);	// biBitCount
	fwriteDWORD(BI_RGB, fp);	// biCompression
	fwriteDWORD(0, fp);	// biSizeImage
	fwriteDWORD(300, fp);	// biXPelsPerMeter
	fwriteDWORD(300, fp);	// biYPelsPerMeter
	fwriteDWORD(0, fp);		// biClrUsed
	fwriteDWORD(0, fp);		// biClrImportant

							// 必要なパディングのサイズ
	pad = rw - w*depth / 8;
	// 画像データの書き出し
	for (y = h - 1; y >= 0; y--) {
		for (x = 0; x<w; x++) {
			getPixel(img, x, y, &pix);
			fputc(pix.b, fp);
			fputc(pix.g, fp);
			fputc(pix.r, fp);
		}
		// Padding部の出力
		for (i = 0; i<pad; i++) {
			fputc(0, fp);
		}
	}
	return 0;
$abort1:
	return 1;
$abort2:
	fclose(fp);
	return 1;
}

int main(){
	ImageData *img;
	int res;
	int x, y, mx, my;
	Pixel pix1, pix2;

	pix1.r = 255;
	pix1.g = 255;
	pix1.b = 255;
	pix2.r = 0;
	pix2.g = 0;
	pix2.b = 0;
	img = createImage(100, 100, 24);
	for (y = 0; y<100; y++) {
		for (x = 0; x<100; x++) {
			if (x % 2 == y % 2) {
				setPixel(img, x, y, &pix1);
			}
			else {
				setPixel(img, x, y, &pix2);
			}
		}
	}
	writeBMPfile("out.bmp", img);
	disposeImage(img);
}
