#ifndef Particle_h
#define Particle_h
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


	const int NUMBER_OF_PIXELS = 6000;

	struct Pixel {
		// position
		float x;
		float y;
		float z;

		// color
		float red;
		float blue;
		float green;
		float alpha;

		// speed
		float xSpeed;
		float ySpeed;
		float zSpeed;

		// size and life
		float particleSize;
		float lifespam;
	};

class Particle
{
private:
	float baseYSpeed;
	float factorYSpeed;
	
	float baseXSpeed;
	float factorXSpeed;

	float baseZSpeed;
	float factorZSpeed;

	Pixel pixel[NUMBER_OF_PIXELS];

public:
	Particle();
	void initialize();
	void move();
	void draw();
	void pixelReborn(int);
};