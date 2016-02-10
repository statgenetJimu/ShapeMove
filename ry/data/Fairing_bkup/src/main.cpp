#include <iostream>
using namespace std;

#include "Mesh.h"
#include "Viewer.h"
#include "DenseMatrix.h"
#include "Mesh.h"
#include "Real.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"
#include <iostream>
#include <fstream>

using namespace DDG;

int main( int argc, char** argv )
{
   if( argc != 2 )
   {
      cerr << "usage: " << argv[0] << " in.obj" << endl;
      return 1;
   }


   Viewer viewer;
   viewer.mesh.read( argv[1] );
   viewer.init();
   
   return 0;
}

