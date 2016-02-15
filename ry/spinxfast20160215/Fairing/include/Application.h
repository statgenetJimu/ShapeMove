/*
 * Implicit Fairing of Arbitrary Meshes using Diffusion and Curvature Flow
 * Mathieu Desbrun, Mark Meyer, Peter Schröder, Alan H. Barr
 * ACM Siggraph '99 Proceedings
 */

#ifndef DDG_APPLICATION_H
#define DDG_APPLICATION_H

#include "Mesh.h"
#include "MeshX.h"
#include "MeshY.h"
#include "Real.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include "DiscreteExteriorCalculus.h"

namespace DDG
{
   class Application
   {
   public:
      void run2(const double step, Mesh& mesh)
      {
         SparseMatrix<Real> star0;
         HodgeStar0Form<Real>::build( mesh, star0 );

         SparseMatrix<Real> star1;
         HodgeStar1Form<Real>::build( mesh, star1 );
         
         SparseMatrix<Real> d0;
         ExteriorDerivative0Form<Real>::build( mesh, d0 );
         
         SparseMatrix<Real> L = d0.transpose() * star1 * d0;
         SparseMatrix<Real> A = star0 + Real(step) * L;
         
         DenseMatrix<Real> x;
         getPositions(mesh, x);
         DenseMatrix<Real> rhs = star0 * x;
         
         solvePositiveDefinite(A, x, rhs);
         setPositions(x, mesh);
      }
      void run(const double step, Mesh& mesh, spiny::Mesh& meshY, spinx::Mesh& meshS)
      //void run(const double step, Mesh& mesh)
      {
         //SparseMatrix<Real> star0;
         //HodgeStar0Form<Real>::build( mesh, star0 );

         //SparseMatrix<Real> star1;
         //HodgeStar1Form<Real>::build( mesh, star1 );
         
         //SparseMatrix<Real> d0;
         //ExteriorDerivative0Form<Real>::build( mesh, d0 );
         
         //SparseMatrix<Real> L = d0.transpose() * star1 * d0;
         //SparseMatrix<Real> A = star0 + Real(step) * L;
         //cout << "L: " << "\n";

   //meshS.setCurvatureChange2( v, step );
   //meshS.setCurvatureChange3(step);
      cout << "setDeform1" << "\n";
   meshY.setCurvatureChange3(step);
   cout << "setCurvature " << "\n";
   //meshS.setCurvatureChange0(meshY.returnRho());
   //meshS.updateDeformation();
   meshY.updateDeformation();
      cout << "setDeform1" << "\n";

   //meshS.updateDeformation2();
   meshY.updateDeformation2();
      cout << "setDeform2" << "\n";
DenseMatrix<Real> x;
         getPositions(mesh, x);
         //DenseMatrix<Real> rhs = star0 * x;
         
         //solvePositiveDefinite(A, x, rhs);
         //setPositions(x, mesh);
         for(unsigned int i = 0; i < meshS.vertices.size(); i++ )
         //for(unsigned int i = 0; i < meshY.vertices.size(); i++ )
   {
      //x(i,0) = meshS.newVertices[i].im().x;
      //x(i,1) = meshS.newVertices[i].im().y;
      //x(i,2) = meshS.newVertices[i].im().z;
      x(i,0) = meshY.newVertices[i].im().x;
      x(i,1) = meshY.newVertices[i].im().y;
      x(i,2) = meshY.newVertices[i].im().z;
      //x(i,0) = 0.0;
      //x(i,1) = 0.0;
      //x(i,2) = 0.0;

   }
         //getPositions(mesh, x);
         //DenseMatrix<Real> rhs = star0 * x;
         
         //solvePositiveDefinite(A, x, rhs);
         setPositions(x, mesh);
      }
      void run_bkup(const double step, Mesh& mesh, spinx::Mesh& meshS)
      //void run(const double step, Mesh& mesh)
      {
         SparseMatrix<Real> star0;
         HodgeStar0Form<Real>::build( mesh, star0 );

         SparseMatrix<Real> star1;
         HodgeStar1Form<Real>::build( mesh, star1 );
         
         SparseMatrix<Real> d0;
         ExteriorDerivative0Form<Real>::build( mesh, d0 );
         
         SparseMatrix<Real> L = d0.transpose() * star1 * d0;
         SparseMatrix<Real> A = star0 + Real(step) * L;
         
         std::vector<double> v(L.nColumns());
         
         //cout << "[qr] size: " << L.nRows() << " x " << L.nColumns() << "\n";
         //std::ofstream outputfile("outTT.txt");
         
         meshS.buildLaplacian();
         
         for( int ii = 0; ii < L.nColumns(); ii++ )
         {
			//outputfile<<L(ii,ii);
			//outputfile<<"\n";
			//cout << "L value" << L(ii,ii) << "\n";
			//cout << "LX value" << meshS.L(ii,ii) << "\n";
			
			v[ii] = L(ii,ii);
			//cout << "star0: " << star0(ii,ii) << "\n";
			//v[ii] = 0;
		}
         //const double scale = -0.05;
   //meshS.setCurvatureChange2( v, step );
   meshS.setCurvatureChange3(step);
   meshS.updateDeformation();
DenseMatrix<Real> x;
         getPositions(mesh, x);
         DenseMatrix<Real> rhs = star0 * x;
         
         solvePositiveDefinite(A, x, rhs);
         //setPositions(x, mesh);
         for(unsigned int i = 0; i < meshS.vertices.size(); i++ )
   {
      x(i,0) = meshS.newVertices[i].im().x;
      x(i,1) = meshS.newVertices[i].im().y;
      x(i,2) = meshS.newVertices[i].im().z;
      //x(i,0) = 0.0;
      //x(i,1) = 0.0;
      //x(i,2) = 0.0;

   }
         //getPositions(mesh, x);
         //DenseMatrix<Real> rhs = star0 * x;
         
         //solvePositiveDefinite(A, x, rhs);
         setPositions(x, mesh);
      }
   protected:
      void getPositions(const Mesh& mesh, DenseMatrix<Real>& x) const
      {
         x = DenseMatrix<Real>( mesh.vertices.size(), 3 );
         for ( VertexCIter v = mesh.vertices.begin();
              v != mesh.vertices.end();
              v ++)
         {
            for( int i = 0; i < 3; ++i)
               x(v->index, i) = v->position[i];
         }
      }
      
      void setPositions(const DenseMatrix<Real>& x, Mesh& mesh)
      {
         for ( VertexIter v = mesh.vertices.begin();
              v != mesh.vertices.end();
              v ++)
         {
            v->position = Vector(x(v->index, 0),
                                 x(v->index, 1),
                                 x(v->index, 2));
         }
      }
   };
}

#endif
