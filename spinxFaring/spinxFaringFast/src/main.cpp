// =============================================================================
// SpinYForm is for spintransformation-based fairing
// that is based on combination of DDG and spinxform by Keenan Crane
// Ryo Yamada
// Feburuary 18, 2017
// main.cpp
//

#include <iostream>
#include <sstream>
#include <fstream>

#include "MeshY.h"
#include "Application.h"



using namespace std;
using namespace DDG;

// argv[1] : objファイル名。球面同相オブジェクトのobjファイル
// argv[2] : Fairing処理の１ステップでの幅、0.9というのは、曲率を10%変化させて平坦化する、と言う意味
// argv[3] : ステップ数
// argv[4] : 出力ファイル名の根 hogeと指定すると hoge0.obj,hoge1.obj,..という名前のファイルができる
// ./fairing hogei.objとすればみられる
int main( int argc, char **argv )
{

   int n = atoi( argv[3] ); //第3引数を整数型に変換してnに代入
   double step = atof( argv[2] ); //第2引数を少数型に変換してxに代入
   // load mesh
   spiny::Mesh meshY;
   meshY.read( argv[1] ); //第1引数はobjファイル名
   
   // processing with Application.h
   
   Application app;
   for(int i=0;i<n;i++){
     app.process(step,meshY);
     std::ostringstream os;
     os << argv[4] << i << ".obj";
     std::string name=os.str();
     meshY.write(name);
   }
   

   return 0;
}

