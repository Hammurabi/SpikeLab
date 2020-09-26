#include "Equations.h"
#include <cmath>

double nernst(const double& t_celsius, const double& ion, const double& concentration_out, const double& concentration_in)
{
	double t_kelvin = t_celsius + 273.15;

	return ((8.314 * t_kelvin) / (ion * 96.485))*log(concentration_out/concentration_in);
}

double ghk(const double& t_celsius,
	const double& pK,
	const double& pNa,
	const double& pCl,
	const double& outK,
	const double& inK,
	const double& outNa,
	const double& inNa,
	const double& outCl,
	const double& inCl)
{
	double t_kelvin = t_celsius + 273.15;

	return ((8.314 * t_kelvin) / (96.485)) * log((pK * outK + pNa * outNa + pCl * inCl) / (pK * inK + pNa * inNa + pCl * outCl));
}

double lerp(double a, double b, double f)
{
	return a + f * (b - a);
}