#ifndef Particle_h
#define Particle_h
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


	const int NUMBER_OF_PIXELS = 200;

class Particle
{
private:
	float baseYSpeed;
	float factorYSpeed;
	float baseXSpeed;
	float factorXSpeed;
	float lifespam;

public:
	float x[NUMBER_OF_PIXELS];
	float y[NUMBER_OF_PIXELS];
	float xSpeed[NUMBER_OF_PIXELS];
	float ySpeed[NUMBER_OF_PIXELS];

	float red;
	float blue;
	float green;
	float alpha;

	float particleSize;

	Particle();
	void initialise();
	void move();
};