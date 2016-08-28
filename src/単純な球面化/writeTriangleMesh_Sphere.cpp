// This code is originally from http://d.hatena.ne.jp/ryamada/20150406/1428009914
// implemented by ryamada


#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <cstdlib>


template <class T>
constexpr T PI = static_cast<T>(3.14159265358979323846);
const double pi = PI<double>;

/*
   球座標を利用して球面に対応する三角メッシュを生成する
   psiはz軸となす角、phiはx軸となす角
*/
void writeTriangleMesh_Sphere( const unsigned int npsi, std::ofstream& out )
{
   // psiごとの頂点の数
   std::vector<int> nphis;
   // 頂点の座標
   std::vector< std::tuple<double, double, double> > vertices;
   double dpsi = pi / npsi;

   // 頂点をファイルに書き込む
   for ( int i = 0; i <= npsi; i++ )
   {
      int nphi;
      if ( i == 0 )
      {
         vertices.push_back( std::make_tuple(0, 0, 1));
         nphi = 1;
      }
      else if ( i == npsi )
      {
         vertices.push_back( std::make_tuple(0, 0, -1));
         nphi = 1;
      }
      else
      {
         double r_xy = std::sin( i * dpsi ), r_z = std::cos( i * dpsi );
         nphi = std::ceil( 2 * pi * r_xy / dpsi );
         double dphi = 2 * pi / nphi;
         for ( int j = 0; j < nphi; j++ )
         {
            vertices.push_back( std::make_tuple
               ( r_xy * std::cos( j * dphi ), r_xy * std::sin( j * dphi ), r_z )
            );
         }
      }
      nphis.push_back( nphi );
   }
   
/*
   面に対応する頂点のtupleを集める
   頂点はファイル上の行数を表すintで表される
*/
   std::vector< std::tuple<unsigned int, unsigned int, unsigned int> > faces;

   // 頂点に上の極を含む場合
   int down_end = nphis[1];
   for ( int i = 0; i < down_end; i++ )
   {
      faces.push_back(
         std::make_tuple(1, i + 2, (i + 1) % down_end + 2) );
   }

   // 極以外の頂点のみを含む場合 
   int up_ori = 2, down_ori = 2 + nphis[1];
   for ( int i = 1; i < npsi - 1; i++ )
   {
      int up_pos = 0, down_pos = 0;
      int up_end = nphis[i], down_end = nphis[i + 1];
      while ( true )
      {
         // 2つの半径の異なる円上の頂点を対応付けるので三角形が歪になる
         // 頂点同士で近いものを組み合わせる
         if ( up_pos < up_end && down_pos < down_end)
         {
            if ( up_pos * nphis[i + 1] > down_pos * nphis[i] )
            {
               faces.push_back( std::make_tuple
                  (up_pos + up_ori, down_pos + down_ori, (down_pos + 1) % down_end + down_ori)
               );
               down_pos++;
            }
            else
            {
               faces.push_back( std::make_tuple
                  (up_pos + up_ori, down_pos + down_ori, (up_pos + 1) % up_end + up_ori)
               );
               up_pos++;
            }
         }
         // 三角形が境界にある場合
         else if ( up_pos == up_end && down_pos < down_end )
         {
            faces.push_back(
               std::make_tuple(up_ori, down_pos + down_ori, (down_pos + 1) % down_end + down_ori) );
            down_pos++;
         }
         else if ( up_pos < up_end && down_pos == down_end )
         {
            faces.push_back(
               std::make_tuple(up_pos + up_ori, (up_pos + 1) % up_end + up_ori, down_ori) );
            up_pos++;
         }
         else
            break;
      }
      up_ori += up_end;;
      down_ori += down_end;
   }


   // 下の極を頂点に含む場合
   int bottom = std::accumulate( nphis.begin(), nphis.end(), 0);
   int up_end = nphis[ nphis.size() - 2 ];
   for ( int i = 0; i < up_end; i++)
   {
      faces.push_back(
         std::make_tuple(bottom, i + ( bottom - up_end), (i + 1) % up_end + ( bottom - up_end )) );
   }


/*   頂点をファイルに書き込む   */
   for ( auto vertex : vertices )
   {
      out << "v " << std::get<0>(vertex) << " "
                  << std::get<1>(vertex) << " "
                  << std::get<2>(vertex) << std::endl;
   }

/*   面をファイルに書き込む   */
   for ( auto face : faces )
   {
      out << "f " << std::get<0>(face) << " "
                  << std::get<1>(face) << " "
                  << std::get<2>(face) << std::endl;
   }
}


int main( const int argc, const char* const argv[])
{
   if ( argc == 1)
   {
      std::cout << "Usage: ./writeTriangleMesh_Sphere npsi[ positive int ]" << std::endl;
      return 1;
   }

   int npsi = std::atoi( argv[1] );
   if ( npsi <= 1 )
   {
      std::cout << "Error: npsi should be integer larger than 2." << std::endl;
      return 1;
   }
   else
   {
      std::ofstream outobj( "sphere.obj" );
      writeTriangleMesh_Sphere( npsi, outobj );
   }

   return 0;
}
