#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <array>
#include <vector>
#include <iostream>
#include "MeshY.h"
#include "VectorY.h"


using namespace std;
using namespace spiny;
const double pi = 3.1415926535897932384626433832795028841971;

template<class Type>
ostream& operator<<(ostream& stream, const vector<Type>& vec)
{
	stream << "[ ";
        for (auto& i: vec) cout << i << " ";
	cout << "]";
	return stream;
}

template<class Type, size_t N>
ostream& operator<<(ostream& stream, const array<Type, N>& arr)
{
	stream << "[ ";
        for (auto& i: arr) cout << i << " ";
	cout << "]";
	return stream;
}

TEST_CASE("check Vector.theta and Vector.phi", "[Vector]")
{
	// ((x, y, z), (r, theta, phi))
	vector<pair<array<double, 3>, array<double, 3>>> coord_s2 = {
	{{ 0,  0,  0}, {0,    0,      0}},
	{{ 1,  0,  0}, {1, pi/2,      0}},
	{{ 0,  1,  0}, {1, pi/2,   pi/2}},
	{{-1,  0,  0}, {1, pi/2,     pi}},
	{{ 0, -1,  0}, {1, pi/2, 1.5*pi}},
	{{ 0,  0,  1}, {1,    0,      0}},
	{{ 0,  0, -1}, {1,   pi,      0}},
	{{ 1/sqrt(2),  1/sqrt(2), 0}, {1, pi/2, pi/4}},
	{{-1/sqrt(2),  1/sqrt(2), 0}, {1, pi/2, 0.75*pi}},
	{{ 1/sqrt(2), -1/sqrt(2), 0}, {1, pi/2, 1.25*pi}},
	{{-1/sqrt(2), -1/sqrt(2), 0}, {1, pi/2, 1.75*pi}},
	{{sqrt(3)/2, 0, 0.5}, {1, 2./3*pi/2, 0}},
	};

	for (auto i: coord_s2)
	{
		auto xyz = get<0>(i);
		auto rt = get<1>(i);
		//cout << xyz << " " << rt << endl;

		Vector vec = Vector(xyz.at(0), xyz.at(1), xyz.at(2));
		REQUIRE(vec.theta() == rt.at(1));
		REQUIRE(vec.phi() == rt.at(2));
	}
}
