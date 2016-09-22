#include "spherical_harmonic.h"
#include <cmath>
#include <boost/math/special_functions/spherical_harmonic.hpp>


namespace spiny
{
extern double sph(int l, int m, double theta, double phi)
{
	using namespace std;
        using namespace boost::math;

	// l-1 because the library define l as 0-origin
        if (m == 0)
                return spherical_harmonic_r(l - 1, m, theta, phi);
        else if (m > 0)
                return sqrt(2) * (m%2==0? 1: -1) * spherical_harmonic_r(l - 1, m, theta, phi);
        else
                return sqrt(2) * (m%2==0? 1: -1) * spherical_harmonic_i(l - 1, m, theta, phi);
}

}
