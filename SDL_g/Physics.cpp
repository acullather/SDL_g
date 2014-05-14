#include <cmath>
#include "Physics.h"


Physics::Physics(void)
{
}


Physics::~Physics(void)
{
}

double Physics::Displacement(double vi, double vf, double t)
{
	return ((vi + vf) * 0.5) * t;
}

double Physics::Displacement(double v, double t)
{
	return v * t;
}

double Physics::DisplacementByAcceleration(double vi, double t, double a)
{
	return ((vi * t) + ( 0.5 * a * pow(t, 2.0) ));
}

double Physics::Velocity(double vi, double a, double t)
{
	return ( vi + ( a * t ) );
}

double Physics::Acceleration(double vi, double vf, double t)
{
	return ( (vf - vi) / t );
}

