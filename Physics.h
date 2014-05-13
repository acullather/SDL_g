#pragma once
class Physics
{
public:
	Physics(void);
	~Physics(void);

	static double Physics::Displacement(double vi, double vf, double t);
	static double Physics::Displacement(double v, double t);
	static double Physics::DisplacementByAcceleration(double vi, double t, double a);

	static double Physics::Velocity(double vi, double a, double t);

	static double Physics::Acceleration(double vi, double vf, double t);
};

