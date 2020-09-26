#pragma once
/*
	Calculate the nernst potential.
*/
double nernst(const double& t_celsius, const double& ion, const double& concentration_out, const double& concentration_in);

/*
	Calculate the GHK equation.
*/
double ghk(const double& t_celsius,
	const double& pK,
	const double& pNa,
	const double& pCl,
	const double& outK,
	const double& inK,
	const double& outNa,
	const double& inNa,
	const double& outCl,
	const double& inCl);

double lerp(double a, double b, double f);