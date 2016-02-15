// =============================================================================
// SpinXForm -- main.cpp
// Keenan Crane
// August 16, 2011
//

#include <iostream>

#include "MeshX.h"
#include "ImageX.h"

#include "Mesh.h"
#include "Viewer.h"
#include "DenseMatrix.h"

#include "Real.h"

#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"

#include <iostream>


#include <fstream>
#include <iterator>
#include <vector>


using namespace std;
using namespace DDG;


int main( int argc, char **argv )
{
   if( argc != 2 )
   {
      cerr << "usage: " << argv[0] << " mesh.obj image.tga result.obj" << endl;
      return 1;
   }

   // load mesh
   
   DDG::Mesh mesh;
   mesh.read( argv[1] );

   spinx::Mesh meshS;
   meshS.read( argv[1] );
   

   
   
   // load mean curvature
   //std::ifstream ifs(argv[2]);
   //const std::vector<double> v(std::istream_iterator<double>(ifs),(std::istream_iterator<double>()));

   // load image
   //Image image;
   //image.read( argv[2] );


   // apply transformation
   //const double scale = -0.5;
   //meshS.setCurvatureChange2( v, scale );
   //mesh.updateDeformation();

   // write result
   //meshS.write( argv[3] );
   
   DDG::Viewer viewer;
   viewer.mesh.read( argv[1] );
   viewer.meshS.read( argv[1] );
   //viewer.mesh = mesh;
   viewer.init();

   return 0;
}

