// ============================================================================
// SpinXForm -- Mesh.cpp
// Keenan Crane
// August 16, 2011
//

#include <fstream>
#include <sstream>
#include <array>
#include <cmath>
#include <cassert>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include "MeshY.h"
#include "VectorY.h"
#include "LinearSolverY.h"
#include "EigenSolverY.h"
#include "UtilityY.h"


namespace spiny{
extern cm::Common cc;
Mesh :: Mesh( void )
// default constructor
: L( cc ), E( cc ) // give matrices a handle to the CHOLMOD environment "cc"
{}

void Mesh :: updateDeformation( void )
{
   int t0 = clock();
   // solve eigenvalue problem for local similarity transformation lambda
   buildEigenvalueProblem();

   EigenSolver::solve( E, lambda );

   // solve Poisson problem for new vertex positions
   // (we assume the final degree of freedom equals zero
   // in order to get a strictly positive-definite matrix)
   buildPoissonProblem();
   int nV = vertices.size();
   vector<Quaternion> v( nV-1 );
   buildLaplacian();
   LinearSolver::solve( L, v, omega );
   for( int i = 0; i < nV-1; i++ )
   {
      newVertices[i] = v[i];
   }
   newVertices[nV-1] = 0.;
   normalizeSolution();

   int t1 = clock();
   cout << "time: " << (t1-t0)/(double) CLOCKS_PER_SEC << "s" << endl;
}
void Mesh :: updateDeformation2( void )
{

   int nV = vertices.size();
   for( int i = 0; i < nV; i++ )
   {
      vertices[i] = newVertices[i];
   }
}

void Mesh :: resetDeformation( void )
{
   // copy original mesh vertices to current mesh
   for( size_t i = 0; i < vertices.size(); i++ )
   {
      newVertices[i] = vertices[i];
   }

   normalizeSolution();
}

void Mesh :: setCurvatureChange4(const double scale)
{
	int nV = vertices.size();
	int nF = faces.size();
	vector<Quaternion> rhov(nV);
	vector<Quaternion> rhof(nF);
	// triangle area area(i) 
	// 頂点周囲面積
	vector<double> areav(nV);
	for(int i=0; i < nF; i++){
		
		// visit each triangle corner
		for( int j = 0; j < 3; j++ ){
			// get vertex indices
			int k0 = faces[i].vertex[ (j+0) % 3 ];
			int k1 = faces[i].vertex[ (j+1) % 3 ];
			int k2 = faces[i].vertex[ (j+2) % 3 ];

			// get vertex positions
			Quaternion f0 = vertices[k0];
			Quaternion f1 = vertices[k1];
			Quaternion f2 = vertices[k2];
			
			//edge
			Quaternion u0 = f1 - f0;
			Quaternion u1 = f2 - f0;
			Quaternion u2 = f2 - f1;
			// cotalpha
			Quaternion u01 = u0 * u1;
			double cotAlpha = (-u01.re())/(u01.im()).norm();
			rhov[k1] += cotAlpha * u2;
			rhov[k2] -= cotAlpha * u2;
			
			areav[k0] += area(i);
		}
	}
	for(int i = 0 ; i < nV; i++){
		rhov[i] /= (4*areav[i]);
	}
	for(int i =0; i < nF ; i++){
		for(int j=0; j < 3; j++){
			int k0 = faces[i].vertex[j];
			rhof[i] += rhov[k0]/3;
		}
	}
	for(int i=0;i < nF; i++){
		rho[i] = (rhof[i].im()).norm() * scale;
		Quaternion p1 = vertices[ faces[i].vertex[0] ];
		Quaternion p2 = vertices[ faces[i].vertex[1] ];
		Quaternion p3 = vertices[ faces[i].vertex[2] ];

		if(((( p2-p1 ) * ( p3-p1 )) * rhof[i]).re() < 0){
			rho[i] *= (-1);
		}

	}
}




double Mesh :: area( int i )
// returns area of triangle i in the original mesh
{
   Vector& p1 = vertices[ faces[i].vertex[0] ].im();
   Vector& p2 = vertices[ faces[i].vertex[1] ].im();
   Vector& p3 = vertices[ faces[i].vertex[2] ].im();

   return .5 * (( p2-p1 ) ^ ( p3-p1 )).norm();
}

void Mesh :: buildEigenvalueProblem( void )
{
   // allocate a sparse |V|x|V| matrix
   int nV = vertices.size();
   QuaternionMatrix E0;
   E0.resize( nV, nV );

   // visit each face
   for( size_t k = 0; k < faces.size(); k++ )
   {
      double A = area(k);
      //cout << A << endl;
      double a = -1. / (4.*A);
      double b = rho[k] / 6.;
      double c = A*rho[k]*rho[k] / 9.;

      // get vertex indices
      int I[3] =
      {
         faces[k].vertex[0],
         faces[k].vertex[1],
         faces[k].vertex[2]
      };

      // compute edges across from each vertex
      Quaternion e[3];
      for( int i = 0; i < 3; i++ )
      {
         e[i] = vertices[ I[ (i+2) % 3 ]] -
                vertices[ I[ (i+1) % 3 ]] ;
      }

      // increment matrix entry for each ordered pair of vertices
      for( int i = 0; i < 3; i++ )
      for( int j = 0; j < 3; j++ )
      {
         E0(I[i],I[j]) += a*e[i]*e[j] + b*(e[j]-e[i]) + c;
      }
   }

   // build Cholesky factorization
   E.build( E0.toReal() );
}

void Mesh :: buildPoissonProblem( void )
{
   buildOmega();
}

void Mesh :: buildLaplacian( void )
// builds the cotan-Laplace operator, where the final row and
// column are omitted to make the system strictly positive-
// definite (equivalent to setting the final degree of freedom
// to zero)
{
   // allocate a sparse |V|x|V| matrix
   int nV = vertices.size();
   QuaternionMatrix L0;
   L0.resize( nV-1, nV-1 );

   // visit each face
   for( size_t i = 0; i < faces.size(); i++ )
   {
      // visit each triangle corner
      for( int j = 0; j < 3; j++ )
      {
         // get vertex indices
         int k0 = faces[i].vertex[ (j+0) % 3 ];
         int k1 = faces[i].vertex[ (j+1) % 3 ];
         int k2 = faces[i].vertex[ (j+2) % 3 ];

         // get vertex positions
         Vector f0 = vertices[k0].im();
         Vector f1 = vertices[k1].im();
         Vector f2 = vertices[k2].im();

         // compute cotangent of the angle at the current vertex
         // (equal to cosine over sine, which equals the dot
         // product over the norm of the cross product)
         Vector u1 = f1 - f0;
         Vector u2 = f2 - f0;
         double cotAlpha = (u1*u2)/(u1^u2).norm();

         // add contribution of this cotangent to the matrix
         if( k1 != nV-1 && k2 != nV-1 ) L0( k1, k2 ) -= cotAlpha / 2.;
         if( k2 != nV-1 && k1 != nV-1 ) L0( k2, k1 ) -= cotAlpha / 2.;
         if( k1 != nV-1 && k1 != nV-1 ) L0( k1, k1 ) += cotAlpha / 2.;
         if( k2 != nV-1 && k2 != nV-1 ) L0( k2, k2 ) += cotAlpha / 2.;
      }
   }
   
   // build Cholesky factorization
   L.build( L0.toReal() );
}

void Mesh :: buildOmega( void )
{
   int nV = vertices.size();

   // clear omega
   for( size_t i = 0; i < omega.size(); i++ )
   {
      omega[i] = 0.;
   }

   // visit each face
   for( size_t i = 0; i < faces.size(); i++ )
   {
      // get indices of the vertices of this face
      int v[3] = { faces[i].vertex[0],
                   faces[i].vertex[1],
                   faces[i].vertex[2] };

      // visit each edge
      for( int j = 0; j < 3; j++ )
      {
         // get vertices
         Quaternion f0 = vertices[ v[ (j+0) % 3 ]];
         Quaternion f1 = vertices[ v[ (j+1) % 3 ]];
         Quaternion f2 = vertices[ v[ (j+2) % 3 ]];

         // determine orientation of this edge
         int a = v[ (j+1) % 3 ];
         int b = v[ (j+2) % 3 ];
         if( a > b )
         {
            swap( a, b );
         }

         // compute transformed edge vector
         Quaternion lambda1 = lambda[a];
         Quaternion lambda2 = lambda[b];
         Quaternion e = vertices[b] - vertices[a];
         Quaternion eTilde = (1./3.) * (~lambda1) * e * lambda1 +
                             (1./6.) * (~lambda1) * e * lambda2 +
                             (1./6.) * (~lambda2) * e * lambda1 +
                             (1./3.) * (~lambda2) * e * lambda2 ;

         // compute cotangent of the angle opposite the current edge
         Vector u1 = ( f1 - f0 ).im();
         Vector u2 = ( f2 - f0 ).im();
         double cotAlpha = (u1*u2)/(u1^u2).norm();

         // add contribution of this edge to the divergence at its vertices
         if( a != nV-1 ) omega[a] -= cotAlpha * eTilde / 2.;
         if( b != nV-1 ) omega[b] += cotAlpha * eTilde / 2.;
      }
   }
}

void Mesh :: normalizeSolution( void )
{
   // center vertices around the origin
   removeMean( newVertices );

   // find the vertex with the largest norm
   double r = 0.;
   for( size_t i = 0; i < vertices.size(); i++ )
   {
      r = max( r, newVertices[i].norm2() );
   }
   r = sqrt(r);

   // rescale so that vertices have norm at most one
   for( size_t i = 0; i < vertices.size(); i++ )
   {
      newVertices[i] /= r;
   }
}

std::vector<double> Mesh :: returnRho()
{
	return rho;
}

// FILE I/O --------------------------------------------------------------------

void Mesh :: read( const string& filename )
// loads a triangle mesh in Wavefront OBJ format
{
   // open mesh file
   ifstream in( filename.c_str() );
   if( !in.is_open() )
   {
      cerr << "Error: couldn't open file ";
      cerr << filename;
      cerr << " for input!" << endl;
      exit( 1 );
   }

   // temporary list of vertex coordinates
   vector<Vector> uv;

   // parse mesh file
   string s;
   while( getline( in, s ))
   {
      stringstream line( s );
      string token;

      line >> token;

      if( token == "v" ) // vertex
      {
         double x, y, z;

         line >> x >> y >> z;

         vertices.push_back( Quaternion( 0., x, y, z ));
         newVertices.push_back( Quaternion( 0., x, y, z ));
      }
      if( token == "vt" ) // texture coordinate
      {
         double u, v;

         line >> u >> v;

         uv.push_back( Vector( u, v, 0. ));
      }
      else if( token == "f" ) // face
      {
         Face triangle;

         // iterate over vertices
         for( int i = 0; i < 3; i++ )
         {
            line >> s;
            stringstream item( s );

            int I[3] = { -1, -1, -1 };

            // iterate over v, vt, and vn indices
            for( int j = 0; getline( item, s, '/' ) && j < 3; j++ )
            {
               stringstream index( s );
               index >> I[j];
            }

            triangle.vertex[i] = I[0]-1;

            if( I[1] != -1 )
            {
               triangle.uv[i] = uv[ I[1]-1 ];
            }
         }

         faces.push_back( triangle );
      }
   }

   // allocate space for mesh attributes
   lambda.resize( vertices.size() );
   omega.resize( vertices.size()-1 );
   rho.resize( faces.size() );
   normalizeSolution();

   // prefactor Laplace matrix
   buildLaplacian();
}

void Mesh :: write( const string& filename )
// saves a triangle mesh in Wavefront OBJ format
{
   ofstream out( filename.c_str() );

   if( !out.is_open() )
   {
      cerr << "Error: couldn't open file ";
      cerr << filename;
      cerr << " for output!" << endl;
      return;
   }

   for( size_t i = 0; i < vertices.size(); i++ )
   {
      out << "v " << newVertices[i].im().x << " "
                  << newVertices[i].im().y << " "
                  << newVertices[i].im().z << endl;
   }

   for( size_t i = 0; i < faces.size(); i++ )
   {
      out << "f " << 1+faces[i].vertex[0] << " "
                  << 1+faces[i].vertex[1] << " "
                  << 1+faces[i].vertex[2] << endl;
   }
}

// -----------------------------------------------------------------------------
// Spherical Harmonic Projection
// added:
// #include "VectorY.h"
// #include <boost/math/special_functions/spherical_harmonic.hpp>
// #include <array>
// #include <cmath>
// #include <cassert>
// see document for mathematical settings
std::vector<double> Mesh :: SH_innerProduct(int n_coef, const std::vector<double> &data) const
{
	using namespace std;
	const double pi = 3.1415926535897932384626433832795028841971;

	// input validation
	assert(data.size() == vertices.size());

	// project vertices onto S2 sphere
	size_t nV = vertices.size();
	vector<Vector> vertices_on_S2(nV);
	for (size_t i = 0; i < nV; i++)
	{
		Quaternion q = vertices[i];
		vertices_on_S2[i][0] = q[1] / q.im().norm(); 
		vertices_on_S2[i][1] = q[2] / q.im().norm();
		vertices_on_S2[i][2] = q[3] / q.im().norm();
	}

	// face angle on triangloid, which is a face projected onto S2
	// arguments: index to Vertices (0 <= i < nV)
	function< double(int,int,int) > face_angle = [&](int pivot, int ind0, int ind1)
	{
		auto P = vertices_on_S2[pivot];
		auto V0 = vertices_on_S2[ind0];
		auto V1 = vertices_on_S2[ind1];
		Vector v0 = V0 - (P*V0) * P;
		Vector v1 = V1 - (P*V1) * P;
		return acos(v0*v1 / ( v0.norm() * v1.norm() ));
	};

	// triangloid area distributed equally to verticies that share the face
	vector<double> triangloid_area(nV, 0);
	for (auto f : faces)
	{
		int index[5] = {0, 1, 2, 0, 1};
		double area = -pi;
		for (int i = 0; i < 3; i++)
		{
			area += face_angle(f.vertex[index[i]], f.vertex[index[i+1]], f.vertex[index[i+2]]);
		}
		for (int i = 0; i < 3; i++)
		{
			triangloid_area[f.vertex[i]] += area / 3;
		}
	}

	// SH projection
	vector<double> coef(n_coef);
	int l = 1, m = 0;
	// continue until coef become full
	while (l*l - l + m + 1 != n_coef)
	{
		double sum = 0;
		for (size_t i = 0; i < nV; i++)
		{
			using namespace boost::math;
			double x = vertices_on_S2[i][0];
			double y = vertices_on_S2[i][1];
			double z = vertices_on_S2[i][2];
			double theta = acos(z);
			double phi;
			// classification to numerically correct
			if (x == 0 && y == 0)
				phi = 0;
			else
			{
				double rx = x / sqrt(x*x + y*y);
				double ry = y / sqrt(x*x + y*y);
				if (abs(rx) < pi/4 && ry > 0)
					phi = acos(rx);	
				else if (abs(rx) < pi/4 && ry < 0)
					phi = acos(rx) + pi;
				else if (rx > 0)
					if (y > 0) phi = asin(ry);
					else phi = 2*pi + asin(ry);
				else
					phi = pi - asin(ry);
			}
			// integral(l-1 because the library define l as 0-origin)
			sum += data[i] * spherical_harmonic_r(l-1, m, theta, phi) * triangloid_area[i];
		}
		coef[l*l - l + m] = sum;
		
		if (m == l - 1)
		{
			l += 1;
			m = -l + 1;
		}
		else m += 1;
	}
	return coef;
}

}
