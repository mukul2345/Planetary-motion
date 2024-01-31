#include "pch.h"
#include "Planetary_Motion.h"

BODIES body;

int main() {

	body.get_planets();
	body.trajectory();
	body.output();

	return 0;
}