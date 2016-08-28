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
      cout << argv[1] : obj�t�@�C�����B���ʓ����I�u�W�F�N�g��obj�t�@�C��
           << argv[2] : Fairing�����̂P�X�e�b�v�ł̕��A0.9�Ƃ����̂́A�ȗ���10%�ω������ĕ��R������A�ƌ����Ӗ�
           << argv[3] : �X�e�b�v��
           << argv[4] : �o�̓t�@�C�����̍� hoge�Ǝw�肷��� hoge0.obj,hoge1.obj,..�Ƃ������O�̃t�@�C�����ł���
           << ./fairing hogei.obj�Ƃ���΂݂���
           << endl;
      return 0;
   }

   int n = atoi( argv[3] ); //��3�����𐮐��^�ɕϊ�����n�ɑ��
   double step = atof( argv[2] ); //��2�����������^�ɕϊ�����x�ɑ��
   // load mesh
   spiny::Mesh meshY;
   meshY.read( argv[1] ); //��1������obj�t�@�C����
   
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
