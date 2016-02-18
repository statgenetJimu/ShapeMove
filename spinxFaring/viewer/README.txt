# 元コード群
このコード群は https://github.com/dgpdec/course/tree/master/Fairing です

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
* Makefileと同階層にできる fairing という実行可能ファイルに１つの引数を渡します
 (1) 三角形メッシュのオブジェクトファイル hoge.obj

* 実行コマンド例
 ./fairing bunny.obj
