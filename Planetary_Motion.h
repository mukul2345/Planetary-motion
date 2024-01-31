#pragma once
#include "pch.h"

#define PI			3.14159265358979
#define G			6.67384e-11			// gravitational constant in m^3/kg * s^2
#define M_EARTH		5.97219e24			// earth mass [kg]
#define R_EARTH		6.371e6				// earth radius [m]
#define T_GEOSTATIC	86164.0905l			// earth rotational period [s]
#define TOTAL_TIME  1.728e+6			//20 days in seconds

class BODIES {

private:

	int time_step; //the change in time used for updating velocity and position
	double current_time = 0; //holds the current time
	int num_bodies; //holds the number of planetary objects
	double x, y; //holds x, y coordinate values
	double vx, vy; //holds velocity component values
	double fx, fy, f; //holds force component, magnitude values
	double dy, dx; //holds component of position from another body
	float theta; //holds the angle of a force
	double mass; //holds the mass of an object

public:
	
	void get_planets();
	void trajectory();
	void output();
};
