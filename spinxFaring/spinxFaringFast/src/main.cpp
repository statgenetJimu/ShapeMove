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

int main( int argc, char **argv )
{

   //usage
   if (argc != 5)
   {
      cout << "argv[1] : objファイル名。球面同相オブジェクトのobjファイル\n"
              "argv[2] : Fairing処理の１ステップでの幅、0.9というのは、曲率を10%変化させて平坦化する、と言う意味\n"
              "argv[3] : ステップ数\n"
              "argv[4] : 出力ファイル名の根 hogeと指定すると hoge0.obj,hoge1.obj,..という名前のファイルができる\n"
              "./fairing hogei.objとすればみられる"
           << endl;
      return 0;
   }

   int n = atoi( argv[3] ); //第3引数を整数型に変換してnに代入
   double step = atof( argv[2] ); //第2引数を少数型に変換してxに代入
   // load mesh
   spiny::Mesh meshY;
   meshY.read( argv[1] ); //第1引数はobjファイル名
   
   // processing with Application.h
   
   Application app;
   for (int i=0; i<n; i++){
      app.process(step,meshY);
      std::ostringstream os;
      os << argv[4] << i << ".obj";
      std::string name=os.str();
      meshY.write(name);
   }
  
   // spherical harmonic expansion 
   std::vector<double> data(meshY.vertices.size(), 0.28209479177387814); // 1/2/pi^(0.5)
   std::vector<double> coef = meshY.SH_innerProduct(10, data);
   std::ofstream out("coef.txt");
   for (auto c : coef)
   {
      out << c << " ";
   }

   return 0;

}
