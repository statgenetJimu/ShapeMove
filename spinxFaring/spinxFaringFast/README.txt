# 元コード群
このコード群は http://www.cs.cmu.edu/~kmcrane/Projects/SpinTransformations/ のCode C++/Fastによる、Spin transformation処理に基づき、オブジェクトの曲率が均一化するような変化量指定するように書き換えたものです

# ビルドの仕方
* このプログラムはVirtualBox + vagrant + utunbu/trusty で動作確認しています

* 以下のコマンドでOpenGLとLinear Algebra、疎行列処理のライブラリをインストールする必要があります
* SuiteSparseについてはこちら http://d.hatena.ne.jp/ryamada/20160214 に少し解説を書きました

* OpenGL
sudo apt-get install -y mesa-common-dev libglu1-mesa-dev freeglut3-dev

* Linear Algebra
sudo apt-get install -y libsuitesparse-dev

* そのうえで、このprep.txtと同階層にあるMakefileをmakeしてください
make

# 使い方
* Makefileと同階層にできる spinxFairingFast という実行可能ファイルに４つの引数を渡します
 (1) 三角形メッシュのオブジェクトファイル hoge.obj
 (2) 負の実数 デフォルトは-0.95 (-1, 0) の値。１回の処理での曲率の平坦化の程度を指定する
 (3) 自然数。平坦化処理の回数
 (4) 出力ファイルの根となる文字列 hoge と入れれば、hoge0.obj, hoge1.obj,...と出る
 
* 実行コマンド例
 ./spinxFaringFast ./example/infile/bunny.obj -0.9 10 out

# 作成objファイルの描出は、viewerフォルダのそれを参照
