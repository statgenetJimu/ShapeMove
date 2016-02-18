// =============================================================================
// SpinXForm -- QuaternionMatrix.cpp
// Keenan Crane
// August 16, 2011
//

#include "QuaternionMatrixY.h"
#include <iostream>

namespace spiny{
using namespace std;

Quaternion QuaternionMatrix::zero( 0., 0., 0., 0. );
// dummy value for const access of zeros

extern cm::Common cc;
QuaternionMatrix :: QuaternionMatrix( void )
: A( cc, 0, 0 )
{}

void QuaternionMatrix :: resize( int _m, int _n )
// initialize an mxn matrix of zeros
{
   m = _m;
   n = _n;
   data.clear();
}

int QuaternionMatrix :: size( int dim ) const
// returns the size of the dimension specified by scalar dim
{
   if( dim == 1 ) return m;
   if( dim == 2 ) return n;
   return 0;
}

Quaternion& QuaternionMatrix :: operator()( int row, int col )
// return reference to element (row,col)
// note: uses 0-based indexing
{
   EntryIndex index( col, row );

   EntryMap::const_iterator entry = data.find( index );
   if( entry == data.end())
   {
      data[ index ] = Quaternion( 0., 0., 0., 0. );
   }

   return data[ index ];
}

const Quaternion& QuaternionMatrix :: operator()( int row, int col ) const
// return const reference to element (row,col)
// note: uses 0-based indexing
{
   EntryIndex index( col, row );

   EntryMap::const_iterator entry = data.find( index );
   if( entry == data.end())
   {
      return zero;
   }

   return entry->second;
}

cm::Upper& QuaternionMatrix :: toReal( void )
// returns real matrix where each quaternion becomes a 4x4 block
{
   double Q[4][4];

   // convert quaternionic matrix to real matrix
   A.resize( n*4, n*4 );
   for( EntryMap::iterator e = data.begin(); e != data.end(); e++ )
   {
      int i = e->first.second;
      int j = e->first.first;
      e->second.toMatrix( Q );

      for( int u = 0; u < 4; u++ )
      for( int v = 0; v < 4; v++ )
      {
         if( Q[u][v] != 0. )
         {
            A( i*4+u, j*4+v ) = Q[u][v];
         }
      }
   }

   return A;
}
std::vector<Quaternion> QuaternionMatrix :: multV(std::vector<Quaternion> v)
{
	vector<Quaternion> ret( n, 0. );
	for( EntryMap::iterator e = data.begin(); e != data.end(); e++ )
   {
      int i = e->first.second;
      int j = e->first.first;
      Quaternion q = e->second;
      ret[i] += q * v[j];

   }
   return ret;
}
}
