

//#include "Window.h"
//#include "math.h"

#include "RoomCube.h"

RoomCube::RoomCube(float roomLength) : Drawable()
{
	this->size = roomLength;
}

void RoomCube::draw(DrawData& data)
{
	float halfSize = size / 2.0;

	//Apply the material properties
	//From here forward anything drawn will be drawn with this material
	//material.apply();

	//Set the OpenGL Matrix mode to ModelView (used when drawing geometry)
	glMatrixMode(GL_MODELVIEW);

	//Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	//glMultMatrixf(toWorld.ptr());


	//Make cube!
	//Note: The glBegin, and glEnd should always be as close to the glNormal/glVertex calls as possible
	//These are special calls that 'freeze' many internal states of OpenGL.
	//Once the glBegin state is active many of the calls made to OpenGL (like glMultMatrixf) will be IGNORED!
	//As a good habit, only call glBegin just before you need to draw, and call end just after you finish
	glBegin(GL_QUADS);

	// Draw front face: (behind camera) vertices should be drawn counter clock wise
	glNormal3f(0.0, 0.0, -1.0); // change to inward for final project
	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);

	// Draw left side:
	glNormal3f(1.0, 0.0, 0.0); // change to inward for final project
	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);

	// Draw right side:
	glNormal3f(-1.0, 0.0, 0.0); // change to inward for final project
	glVertex3f(halfSize, halfSize, halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);

	// Draw back face: (in front of camera
	glNormal3f(0.0, 0.0, 1.0); // change to inward for final project
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);

	// Draw top side:
	glNormal3f(0.0, -1.0, 0.0); // change to inward for final project
	glVertex3f(-halfSize, halfSize, halfSize);
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, halfSize, -halfSize);
	glVertex3f(halfSize, halfSize, halfSize);

	// Draw bottom side:
	glNormal3f(0.0, 1.0, 0.0); // change to inward for final project
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);
	glVertex3f(halfSize, -halfSize, halfSize);
	glVertex3f(halfSize, -halfSize, -halfSize);

	glEnd();

	//The above glBegin, glEnd, glNormal and glVertex calls can be replaced with a glut convenience function
	//glutSolidCube(size);

	//Pop the save state off the matrix stack
	//This will undo the multiply we did earlier
	glPopMatrix();
}