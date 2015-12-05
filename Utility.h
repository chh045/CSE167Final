#include <iostream>

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

void EnableFPS() {
	static int prevTime = 0;

	int currTime = glutGet(GLUT_ELAPSED_TIME),
		diffTime = currTime - prevTime;

	if (diffTime != 0)
		std::cout << "FPS = " << 1000 / diffTime << std::endl;

	prevTime = currTime;
}
