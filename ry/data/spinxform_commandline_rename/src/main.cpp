// =============================================================================
// SpinXForm -- main.cpp
// Keenan Crane
// August 16, 2011
//

#include <iostream>
#include "MeshX.h"
#include "ImageX.h"
#include <fstream>
#include <iterator>
#include <vector>


using namespace std;

namespace spinx{
int main( int argc, char **argv )
{
   if( argc != 4 )
   {
      cerr << "usage: " << argv[0] << " mesh.obj image.tga result.obj" << endl;
      return 1;
   }

   // load mesh
   
   //DDG::Mesh meshD;
   //meshD.read( argv[1] );

   Mesh mesh;
   mesh.read( argv[1] );
   
   // load mean curvature
   std::ifstream ifs(argv[2]);
   const std::vector<double> v(std::istream_iterator<double>(ifs),(std::istream_iterator<double>()));

   // load image
   //Image image;
   //image.read( argv[2] );


   // apply transformation
   const double scale = -0.5;
   mesh.setCurvatureChange2( v, scale );
   //mesh.updateDeformation();

   // write result
   mesh.write( argv[3] );

   return 0;
}
}
