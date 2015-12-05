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
	factorXSpeed = 0.0001;

	baseZSpeed = -0.01;
	factorZSpeed = 0.0001;

	initialize();
}
void Particle::initialize() {

	for (int i = 0; i < NUMBER_OF_PIXELS; i++)
		pixelReborn(i);

}
void Particle::move() {

	// Moving all pixels - basically changing it's position
	// Note: I think if we apply benzier curve here it will very awesome ( i think )
	for (int i = 0; i < NUMBER_OF_PIXELS; i++)
	{


		pixel[i].x += pixel[i].xSpeed;

		pixel[i].y += pixel[i].ySpeed;

		pixel[i].z += pixel[i].zSpeed;
	

		// just for fun... testing stuff:
		if ( ( pixel[i].x > -4.8 && pixel[i].x < -4.2 ) && (pixel[i].y > -5.0 && pixel[i].y < -4.2) )
			pixel[i].green = 0.8;
		else
			pixel[i].green = 0.6;

		//cout << pixel[i].z << endl;

	}

	for (int i = 0; i < NUMBER_OF_PIXELS; i++) {
		pixel[i].particleSize = pixel[i].particleSize*0.89;  // decrease size of pixel each movement by small amount
		pixel[i].lifespam--; // decrease his life... like human life lol

		 // if any pixel is too small.. OR he has no more life... REBORN!
		if (pixel[i].lifespam < 0 || pixel[i].particleSize < 0.2f)
			pixelReborn(i);
	}

	

}

void Particle::draw() {


	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();


		for (int j = 0; j < NUMBER_OF_PIXELS; j++)
		{
			// I added this to make particle to have size... you can take it out and you will see it will be only pixels
			// Note: It will only work if it is called before glBegin
			glPointSize(pixel[j].particleSize);

			glBegin(GL_POINTS);


		//	glColor4f(1.0f, 0.5f, 0.0f, 1.0f);  // Orange
			glColor4f(pixel[j].red, pixel[j].green, pixel[j].blue, pixel[j].alpha);   // If in the future we want to make better colors
																	  // Draw the point in x/y plane - I am not good with 3D   :[
																	  // But let's see if we can use 2D plane later. If not, we just have to convert to 3D which is only few calculation
																	  // By the way... I used the technique of rasterize from project 3 or 4... to make it :)
			glVertex3f(pixel[j].x, pixel[j].y, pixel[j].z);

			glEnd();
		}

		//Move particles - remember: each particle has its unique speed
		move();
	

	glPopMatrix();
}

void Particle::pixelReborn(int id) {


	float zSpeedVal;
	float ySpeedVal;
	float xSpeedVal;

	/*
	** Loop each pixel, giving it a location and x/y speed
	*/
	
		// give a new lifespam each time it is called
		pixel[id].lifespam = 700 + rand() % 1000;

		// Generate speed of X and Y
		zSpeedVal = baseYSpeed + ((float)rand() / (float)RAND_MAX) * factorYSpeed;
		ySpeedVal = baseYSpeed + ((float)rand() / (float)RAND_MAX) * factorYSpeed;
		xSpeedVal = baseXSpeed + (rand() / (float)RAND_MAX)*factorXSpeed;

		// Setting initial position

		pixel[id].x = -5 + (rand() / (float)RAND_MAX);
		pixel[id].y = -5 + (rand() / (float)RAND_MAX) / 2;
		pixel[id].z = -5 + (rand() / (float)RAND_MAX);

		// Setting speed of the pixel
		pixel[id].xSpeed = xSpeedVal;
		pixel[id].ySpeed = ySpeedVal;
		pixel[id].zSpeed = ySpeedVal;

		// For future use.. We can set the RGB of the particle. Note: I am including alpha for opacity ( can't make it work though )
		//pixel[id].red = (rand() / (float)RAND_MAX);
		//pixel[id].green = (rand() / (float)RAND_MAX);
		//pixel[id].blue = (rand() / (float)RAND_MAX);
		pixel[id].red = 1.0;
		pixel[id].green = 0.5;
		pixel[id].blue = 0;
		pixel[id].alpha = 1.0f;

		// Giving PARTICLE a size ( it affects all small pixels )
		pixel[id].particleSize = 0.5f + ((float)rand() / (float)RAND_MAX) * 3.3f;
	

}