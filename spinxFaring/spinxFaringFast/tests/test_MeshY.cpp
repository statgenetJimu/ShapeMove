#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <vector>
#include "VectorY.h"
#include "QuaternionY.h"
#include "MeshY.h"
#include "spherical_harmonic.h"

#include <iostream>

using namespace std;
using namespace spiny;

const double pi = 3.1415926535897932384626433832795028841971;


template<class Type>
ostream& operator<<(ostream& stream, const vector<Type>& vec)
{
        stream << "[ ";
        for (auto& i: vec) stream << i << " ";
        stream << "]";
        return stream;
}

template<class Type, size_t N>
ostream& operator<<(ostream& stream, const array<Type, N>& arr)
{
        stream << "[ ";
        for (auto& i: arr) stream << i << " ";
        stream << "]";
        return stream;
}




TEST_CASE("check Mesh::SH_restoreData","[Mesh]")
{
	Mesh mesh;

	SECTION("vertices: uniform, value: equal")
	{
		mesh.read("uniform_sphere.obj");
		vector<double> coef = {1, 0, 0, 0};
		vector<double> data = mesh.SH_restoreData(coef);
		vector<double> correct_data(data.size(), 0.28209479177387814);
		REQUIRE(data == correct_data);
	}

	SECTION("vertices: uniform, value: not equal")
	{
		mesh.read("uniform_sphere.obj");

		// test1
		vector<double> coef = {0, 1, 0, 0};
		vector<double> data = mesh.SH_restoreData(coef);
		vector<double> correct_data(data.size());
		for (size_t i = 1; i < data.size(); ++i)
		{
			Vector vertex(mesh.vertices[i].im());
			double theta = vertex.theta();
			double phi = vertex.phi();
			correct_data[i] = sph(2, -1, theta, phi);
		}
		REQUIRE(data == correct_data);

		// test2
		coef = {0, 0, 0, 0, 0, 1, 0, 0, 0};
		data = mesh.SH_restoreData(coef);
		fill(correct_data.begin(), correct_data.end(), 0);
		for (size_t i = 1; i < data.size(); ++i)
		{
			Vector vertex(mesh.vertices[i].im());
			double theta = vertex.theta();
			double phi = vertex.phi();
			correct_data[i] = sph(3, -1, theta, phi);
		}
		REQUIRE(data == correct_data);
	}

	SECTION("vertices: not uniform, coef = {1, 0, 0, 0}")
	{
		mesh.read("bunny_sphere.obj");
		vector<double> coef = {1, 0, 0, 0};
		vector<double> data = mesh.SH_restoreData(coef);
		vector<double> correct_data(data.size(), 0.28209479177387814);
		REQUIRE(data == correct_data);
	}
}

TEST_CASE("check Mesh::SH_expand", "[Mesh]")
{
	Mesh mesh;
	
	SECTION("vertices: uniform, value: equal")
	{
		mesh.read("uniform_sphere.obj");
		vector<double> data(mesh.vertices.size(), 0.28209479177387814); // 1/2/pi^(0.5)
		vector<double> coef = mesh.SH_expand(5, data);
	
		REQUIRE(coef[0] == Approx(1));
		for (size_t i = 1; i < coef.size(); ++i)
		{
			CHECK(coef[i] == Approx(0));
		}
	}
	
	SECTION("vertices: uniform, value: {1, 0, 0, 0}")
	{
		mesh.read("uniform_sphere.obj");
		vector<double> coef_original = {1, 0, 0, 0};
		vector<double> data = mesh.SH_restoreData(coef_original);
	
		vector<double> coef = mesh.SH_expand(coef_original.size(), data);
		for (size_t i = 0; i < coef.size(); ++i)
		{
			CHECK(coef[i] == Approx(coef_original[i]));
		}
	}

	SECTION("vertices: uniform, value: {10, 1, 2, 0, 1, 5, 3, 4, 0}")
	{
		mesh.read("uniform_sphere.obj");
		// use same coefficient as http://takuyaokada.hatenablog.com/entry/20160218/1455758454 (written in python)
		vector<double> coef_original = {10, 1, 2, 0, 1, 5, 3, 4, 0};
		vector<double> data = mesh.SH_restoreData(coef_original);
	
		vector<double> coef = mesh.SH_expand(coef_original.size(), data);
		for (size_t i = 0; i < coef.size(); ++i)
		{
			CHECK(coef[i] == Approx(coef_original[i]));
		}
	}

	SECTION("vertices: not uniform, value: equal")
	{
		mesh.read("bunny_sphere.obj");
	
		vector<double> data(mesh.vertices.size(), 0.28209479177387814); // 1/2/pi^0.5
		vector<double> coef = mesh.SH_expand(100, data);
	
		REQUIRE(coef[0] == Approx(1));
		for (size_t i = 1; i < coef.size(); ++i)
		{
			REQUIRE(coef[i] == Approx(0));
		}
	}
	
	SECTION("vertices: not uniform, value: not equal")
	{
		mesh.read("bunny_sphere.obj");
	
		// use same coefficient as http://takuyaokada.hatenablog.com/entry/20160218/1455758454 (written in python)
		vector<double> coef_original = {10, 1, 2, 0, 1, 5, 3, 4, 0};
		vector<double> data = mesh.SH_restoreData(coef_original);
	
		vector<double> coef = mesh.SH_expand(coef_original.size(), data);
		for (size_t i = 0; i < coef.size(); ++i)
		{
			REQUIRE(coef[i] == Approx(coef_original[i]));
		}
	}
}
