#include <iostream>
#include "Particle.h"

using namespace std;

extern const int NUMBER_OF_PIXELS;

Particle::Particle() {

	// modifier x/y speed.  The number is randomly put to make it work nicely 
    baseYSpeed = 0.001;
	factorYSpeed = 0.1;
	//baseXSpeed = -0.0008;
	baseXSpeed = -0.01;
	factorXSpeed = 0.01;

	initialise();
}
void Particle::initialise() {

	float ySpeedVal;
	float xSpeedVal;

	lifespam = 500 + rand() % 1000; // give a new lifespam each time it is called
	

	/*
	** Loop each pixel, giving it a location and x/y speed
	*/
	for (int i = 0; i < NUMBER_OF_PIXELS; i++)
	{
		// Generate speed of X and Y
		ySpeedVal = baseYSpeed + ((float)rand() / (float)RAND_MAX) * factorYSpeed;
		xSpeedVal = baseXSpeed + (rand() / (float)RAND_MAX)*factorXSpeed;

		// Setting initial position
		x[i] =  (rand() / (float)RAND_MAX);
		y[i] = (rand() / (float)RAND_MAX)/2;
		
		// Setting speed of the pixel
		xSpeed[i] = xSpeedVal;
		ySpeed[i] = ySpeedVal;
	}

	// For future use.. We can set the RGB of the particle. Note: I am including alpha for opacity ( can't make it work though )
	red = (rand() / (float)RAND_MAX);
	green = (rand() / (float)RAND_MAX);
	blue = (rand() / (float)RAND_MAX);
	alpha = 1.0f;

	// Giving PARTICLE a size ( it affects all small pixels )
	particleSize = 0.5f + ((float)rand() / (float)RAND_MAX) * 3.3f;

}
void Particle::move() {

	// Moving all pixels - basically changing it's position
	// Note: I think if we apply benzier curve here it will very awesome ( i think )
	for (int i = 0; i < NUMBER_OF_PIXELS; i++)
	{

			x[i] += xSpeed[i];

			y[i] += ySpeed[i];
		
	}

	particleSize = particleSize*0.9;  // decrease size of pixel each movement by small amount
	lifespam--; // decrease his life... like human life lol

	// if the particle is too small.. OR he has no more life... REBORN!
	if (lifespam < 0 || particleSize < 0.2f)
		initialise();

}

