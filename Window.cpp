#include <iostream>
#include "Shader.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"
#include "Particle.h"

using namespace std;
int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here

Shader* shader;


/* NOTE: Each Particle has 'x' amounts of small particles.
** I have used a bad name for that, but we can change that one later.
**
**Example: p[0] contains 100 small little particles. 
*/

Particle p[15];
extern const int NUMBER_OF_PIXELS;

void Window::initialize(void)
{
    //Setup the light
    Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;
    
    //Initialize cube matrix:
    Globals::cube.toWorld.identity();
    
    //Setup the cube's material properties
    Color color(0x23ff27ff);
    Globals::cube.material.color = color;

	shader = new Shader("flag.vert", "flag.frag", true);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
    //Rotate cube; if it spins too fast try smaller values and vice versa
    Globals::cube.spin(0.005);
    
    //Call the update function on cube
    Globals::cube.update(Globals::updateData);
    
    //Call the display routine to draw the cube
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    Globals::light.bind(0);
    
    //Draw the cube!
	//shader->bind();
   // Globals::cube.draw(Globals::drawData);

	draw();  // draw the p[0] ~ p[?]  particles


	//shader->unbind();
    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}

void Window::draw() {

	glMatrixMode(GL_MODELVIEW);

	// I'm using PARTICLE to refer to big particle ( the one that contains many many small particles )
	// First step... we loop the 15 PARTICLES
	for (int i = 0; i < 15; i++)
	{

		// Looping all pixels of the PARTICLE
		for (int j = 0; j < NUMBER_OF_PIXELS; j++)
		{		
			// I added this to make particle to have size... you can take it out and you will see it will be only pixels
			// Note: It will only work if it is called before glBegin
			glPointSize(p[i].particleSize);

			glBegin(GL_POINTS);
			
			glColor4f(1.0f, 0.5f, 0.0f, 1.0f);  // Orange
			//glColor4f(p[loop].red, p[loop].green, p[loop].blue, p[loop].alpha);   // If in the future we want to make better colors

			// Draw the point in x/y plane - I am not good with 3D   :[
			// But let's see if we can use 2D plane later. If not, we just have to convert to 3D which is only few calculation
			// By the way... I used the technique of rasterize from project 3 or 4... to make it :)
			glVertex2f(p[i].x[j], p[i].y[j]);

			glEnd();
		}

		//Move particles - remember: each particle has its unique speed
			p[i].move();
	}

	glPopMatrix();
}

//TODO: Keyboard callbacks!

//TODO: Function Key callbacks!

//TODO: Mouse callbacks!

//TODO: Mouse Motion callbacks!