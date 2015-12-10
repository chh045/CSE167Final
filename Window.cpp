/*            :::::::::::WARNING::::::::::::::
** PLEASE, DO NOT INCLUDE SAME HEADER MORE THAN ONCE.
** PLEASE, GLEE.H ALWAYS DEFINED BEFORE GLUT.H
** PLEASE, ADD ONLY HEADERS IN H-FILES IF YOU NEED TO USE IN THE HEADER
** OTHERWISE, PUT IT IN .CPP FILE
**/

#include <iostream>

#include "ShadowMapping.h" 

#include "Window.h"  // contains glee

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <math.h>
#else
#include <GL/glut.h>
#endif

//#include "Utility.h"

int Window::width = 1350;   //Set window width in pixels here
int Window::height = 750;   //Set window height in pixels here

bool Window::mouse_rotate_on = false;
int Window::last_x = 0;
int Window::last_y = 0;

ShadowMapping* shadow;
//Shader* shadow1;
Shader* skybox_shader;
Shader* envMapping_shader;


// For frame
int frame = 0, time1, timebase = 0;

float mouseXposition = 0;
float mouseYposition = 0;

extern const int NUMBER_OF_PIXELS;  // from Particle.h
extern const int MAX_DEPTH;

void draw_wall() {
	Globals::wallTex->bind();
	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);
	float sq_l = Globals::grd_length / 2;
	float sq_w = Globals::grd_width / 2;
	float wall_h = Globals::wall_height;
	float wall_d = Globals::grd_depth;

	// front
	glTexCoord2f(0, 2); glVertex3f(-sq_w, wall_d, -sq_l);
	glTexCoord2f(10, 2); glVertex3f(sq_w, wall_d, -sq_l);
	glTexCoord2f(10, 0); glVertex3f(sq_w, wall_d + wall_h, -sq_l);
	glTexCoord2f(0, 0); glVertex3f(-sq_w, wall_d + wall_h, -sq_l);

	// back
	glTexCoord2f(0, 2); glVertex3f(sq_w, wall_d, sq_l);
	glTexCoord2f(10, 2); glVertex3f(-sq_w, wall_d, sq_l);
	glTexCoord2f(10, 0); glVertex3f(-sq_w, wall_d + wall_h, sq_l);
	glTexCoord2f(0, 0); glVertex3f(sq_w, wall_d + wall_h, sq_l);

	// right
	glTexCoord2f(0, 2); glVertex3f(sq_w, wall_d, -sq_l);
	glTexCoord2f(10, 2); glVertex3f(sq_w, wall_d, sq_l);
	glTexCoord2f(10, 0); glVertex3f(sq_w, wall_d + wall_h, sq_l);
	glTexCoord2f(0, 0); glVertex3f(sq_w, wall_d + wall_h, -sq_l);

	// left
	glTexCoord2f(0, 2); glVertex3f(-sq_w, wall_d, sq_l);
	glTexCoord2f(10, 2); glVertex3f(-sq_w, wall_d, -sq_l);
	glTexCoord2f(10, 0); glVertex3f(-sq_w, wall_d + wall_h, -sq_l);
	glTexCoord2f(0, 0); glVertex3f(-sq_w, wall_d + wall_h, sq_l);

	glEnd();
	Globals::wallTex->unbind();
}

void Window::initialize(void)
{
	//Setup the light
	Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
	Globals::light.position = lightPos;
	Globals::light.quadraticAttenuation = 0.02;



	//Initialize cube matrix:
	//Globals::cube.toWorld.identity();

	//Setup the cube's material properties
	Color color(0x23ff27ff);
	Globals::cube.material.color = color;

	//shadow = new ShadowMapping("shadow_map.vert", "shadow_map.frag");
	//shadow1 = new Shader("shadow_map.vert", "shadow_map.frag", true);
//	shadow = new ShadowMapping(shadow1->getPid());
	
	Globals::grdTex = new Texture("beachsand.ppm", true);// repeat mode on
	Globals::wallTex = new Texture("brick_wall_original.ppm", true);// repeat mode on

	Globals::testRoomTex = CubeMapTexture(
		"plains-of-abraham_right.ppm",
		"plains-of-abraham_left.ppm",
		"plains-of-abraham_up.ppm",
		"plains-of-abraham_dn.ppm",
		"plains-of-abraham_front.ppm",
		"plains-of-abraham_back.ppm", false);

	skybox_shader = new Shader("skybox.vert", "skybox.frag", true);
	envMapping_shader = new Shader("envMapping.vert", "envMapping.frag", true);
	shadow = new ShadowMapping("shadow_map.vert", "shadow_map.frag");
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	//Set up a static time delta for update calls
	Globals::updateData.dt = 1.0 / 60.0;// 60 fps

										//Rotate cube; if it spins too fast try smaller values and vice versa
										//Globals::cube.spin(0.005);

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
	gluPerspective(60.0, double(width) / (double)height, 1, 1000.0); //Set perspective projection viewing frustum
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


	
	// draw test room
	skybox_shader->bind();
	Globals::testRoomTex.bind();
	Globals::testRoom.draw(Globals::drawData);
	Globals::testRoomTex.unbind();
	skybox_shader->unbind();

	glDisable(GL_LIGHTING);


	// shadow mapping
	glEnable(GL_CULL_FACE);
	shadow->bind();
	shadow->renderScene(Globals::group, Globals::emulateDay);
	//glDisable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	shadow->drawObjects(Globals::group);
	shadow->unbind();
	glDisable(GL_CULL_FACE);




	/* drawing sphere with environmental mapping */
	envMapping_shader->bind();
	GLint camPos_loc = glGetUniformLocationARB(envMapping_shader->getPid(), "camPos");
	glUniform3fv(camPos_loc, 1, Globals::camera.getPos().ptr());
	//glProgramUniform3fvEXT(cube_shader->getPid(), camPos_loc, 1, Globals::camera.getPos().ptr());

	Globals::testRoomTex.bind();
	Globals::sphere.draw(Globals::drawData);
	Globals::testRoomTex.unbind();

	envMapping_shader->unbind();


	Globals::particle.draw();  // draw the p[0] ~ p[?]  particles

	//Globals::tree.draw();

	// try to draw the groud
	if (false) {
		int length = Globals::grd_length;
		Globals::grdTex->bind();
		glBegin(GL_QUADS);

		glColor3f(1, 1, 1);
		//glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0, 50); glVertex3f(length, -100, -length);
		glTexCoord2f(50, 50); glVertex3f(length, -100, length);
		glTexCoord2f(50, 0); glVertex3f(-length, -100, length);
		glTexCoord2f(0, 0); glVertex3f(-length, -100, -length);

		glEnd();
		Globals::grdTex->unbind();
	}

	// draw wall
	draw_wall();

	//Globals::cube.draw(Globals::drawData);

	frame++;
	time1 = glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		//std::cout << "FPS: " << frame*1000.0 / (time1 - timebase) << std::endl;
		timebase = time1;
		frame = 0;
	}

	glEnable(GL_LIGHTING);

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();


	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();

	// comment out when debugging
	//EnableFPS();
}



//TODO: Keyboard callbacks!
void Window::keyboardCallback(unsigned char key, int x, int y)
{
	switch (key) {

	case 27:
		exit(0);
		break;

	case 'A':
		Globals::tree.angle++;
		Globals::tree1.angle++;
		Globals::tree2.angle++;
		break;
	case 'a':
		Globals::tree.angle--;
		Globals::tree1.angle--;
		Globals::tree2.angle--;
		break;
	case 'B':
		Globals::bounding_box_on = !Globals::bounding_box_on;
		//OBJObject::bbox_on = !OBJObject::bbox_on;
		break;

	case 'D':
		if (Globals::tree.current_depth < MAX_DEPTH)
			Globals::tree.current_depth++;
		if (Globals::tree1.current_depth < MAX_DEPTH)
			Globals::tree1.current_depth++;
		if (Globals::tree2.current_depth < MAX_DEPTH)
			Globals::tree2.current_depth++;
		break;
	case 'd':
		if (Globals::tree.current_depth > 0)
			Globals::tree.current_depth--;
		if (Globals::tree1.current_depth > 0)
			Globals::tree1.current_depth--;
		if (Globals::tree2.current_depth > 0)
			Globals::tree2.current_depth--;
		break;
	case 'h':
		Globals::cam3Dmove_on = !Globals::cam3Dmove_on;
		break;

	case 'p':
		Globals::emulateDay = !Globals::emulateDay;
		break;


	case 'l':
		Globals::camera.lookUpDown(-1);
		//Globals::camera.moveZoom(-4);
		break;

	case '.':
		Globals::camera.lookUpDown(1);
		//Globals::camera.moveZoom(4);
		break;

	case ',': //left
		Globals::camera.orbitTrack(Globals::camera.getUp(), 2);
		//Globals::camera.moveLeftRight(-4);
		break;

	case '/': //right
		//Globals::camera.moveLeftRight(4);
		Globals::camera.orbitTrack(Globals::camera.getUp(), -2);
		break;

	case '1':
		//rot_axis[0] -= 0.1;
//		Globals::camera.rotate(rot_axis, rot_angle);
		break;
	case '2':
	//	rot_axis[0] += 0.1;
	//	Globals::camera.rotate(rot_axis, rot_angle);
		break;
	case '3':
		//rot_axis[1] += 0.1;
//Globals::camera.rotate(rot_axis, rot_angle);
		break;
	case '4':
		//rot_axis[1] -= 0.1;
	//	Globals::camera.rotate(rot_axis, rot_angle);
		break;

	default:
		break;
	}
}

void Window::specialKeyCallback(int key, int x, int y)
{
	switch (key) {
		/*
		case GLUT_KEY_UP:
		Globals::camera.moveFoward(Globals::cam3Dmove_on);
		break;
		case GLUT_KEY_DOWN:
		Globals::camera.moveBack(Globals::cam3Dmove_on);
		break;
		*/
		case GLUT_KEY_LEFT:
		Globals::camera.moveLeft();
		break;
		case GLUT_KEY_RIGHT:
		Globals::camera.moveRight();
		break;
		


	case GLUT_KEY_DOWN:
		Globals::camera.moveBack(Globals::cam3Dmove_on);
		break;

	case GLUT_KEY_UP:
		Globals::camera.moveFoward(Globals::cam3Dmove_on);
		break;

		/*
	case GLUT_KEY_RIGHT:
		Globals::camera.moveRight();
		break;


		case GLUT_KEY_LEFT:
		Globals::camera.moveLeft();
		break;
		*/

	default:
		break;
	}
}

//TODO: Function Key callbacks!

//TODO: Mouse callbacks!


//TODO: Mouse Motion callbacks!

Vector3 rotateAxis(int x, int y, int width, int height) {
	float xx = (2.0 * x - width) / width, // why is this?
										  //float xx = (width - 2.0 * x) / width, // why is this?
										  //yy = (2.0 * y - height) / height;
		yy = (height - 2.0 * y) / height;
	Vector3 vec(xx, yy, 0.0);

	float dis = vec.magnitude();
	dis = (dis < 1.0) ? dis : 1.0;

	//if (Globals::camera.getLookDirec()[2] < 0)
	vec.set(2, -sqrtf(1.001 - dis * dis));

	vec.normalize();

	return vec;
}

void Window::mouseButton(int button, int state, int x, int y)
{
	/*
	if (button == GLUT_LEFT_BUTTON) {
	Window::mouse_rotate_on = (state == GLUT_DOWN) ? true : false;
	Window::last_x = x;
	Window::last_y = y;
	}
	*/
}

//TODO: Mouse Motion callbacks!
void Window::mouseMotion(int x, int y)
{
	/*
	// rotate obj about the center
	if (Window::mouse_rotate_on) {
	Vector3 last = rotateAxis(last_x, last_y, Window::width, Window::height);
	Vector3 curr = rotateAxis(x, y, Window::width, Window::height);
	float velocity = (curr - last).magnitude();
	if (velocity > 0.0001) {
	Vector3 rot_axis = last.cross(curr).normalize();
	rot_axis.print("axis is ");
	//Vector3 rot_axis = curr.cross(last);
	float rot_angle = velocity * 0.5; // radians
	Globals::camera.rotate(rot_axis, rot_angle);
	}
	last_x = x;
	last_y = y;
	}
	*/
}

void Window::passiveMouseMotion(int x, int y) {


	// rotate in camera space (z axis is out from the screen)
/*	Vector3 axis = rotateAxis(x, y, Window::width, Window::height);
	Vector3 neg_z(0, 0, -1);

	// put it back to mouse passive later
	Vector3 rot_axis;
	float rot_angle;

	 rot_axis = neg_z.cross(axis);
	float velocity = (axis - neg_z).magnitude();

	float dx;
	float dy;

	float valUPRIGHT = 0.00001;
	float valLEFTRIGHT = 0.00001;

	float limitHorizontal = 0.001;
	float limitVertical = 0.001;

	dx = mouseYposition - rot_axis[1];
	dy = rot_axis[0] - mouseXposition ;
	

	mouseXposition = rot_axis[0];
	mouseYposition = rot_axis[1];

	// quad 1  (+,+)
	if (dx >= 0 && dy>= 0) {
		if (rot_axis[0] < 0)
			rot_axis[0] = valUPRIGHT;
		if (rot_axis[1] > 0)
			rot_axis[1] = -valLEFTRIGHT;
		
		// move right only
		if (dy >= 0 && dy <= limitHorizontal) {
			rot_axis[0] = 0;
			rot_axis[1] = -valLEFTRIGHT;
		}

		// move up only
		if (dx >= 0 && dx <= limitVertical) {
			rot_axis[0] = valUPRIGHT;
			rot_axis[1] = 0;
		}


	}
	// quad 2 ( +, -)
	else if (dx >= 0 && dy < 0) {
		if (rot_axis[0] > 0)
   		 rot_axis[0] = -valUPRIGHT;
		if (rot_axis[1] > 0)
			rot_axis[1] = -valLEFTRIGHT;

		// move right only
		if (dy >= 0 && dy <= limitHorizontal) {
			rot_axis[0] = 0;
			rot_axis[1] = -valLEFTRIGHT;
		}

		// move down only
		if (dx < 0 && dx >= -limitVertical) {
			rot_axis[0] = -valUPRIGHT;
			rot_axis[1] = 0;
		}

	}
	// quad 3 ( - , + )
	else if (dx < 0 && dy >= 0) {
		if (rot_axis[0] < 0)
		rot_axis[0] = valUPRIGHT;
		if (rot_axis[1] < 0)
			rot_axis[1] = valLEFTRIGHT;

		// move left only
		if (dy < 0 && dy >= -limitHorizontal) {
			rot_axis[0] = 0;
			rot_axis[1] = valLEFTRIGHT;
		}

		// move up only
		if (dx >= 0 && dx <= limitVertical) {
			rot_axis[0] = valUPRIGHT;
			rot_axis[1] = 0;
		}
	}
	//quad 4 ( - , - )
	else if (dx < 0 && dy < 0) {
		if (rot_axis[0] > 0)
			rot_axis[0] = -valUPRIGHT;
		if (rot_axis[1] < 0)
			rot_axis[1] = valLEFTRIGHT;

		// move left only
		if (dy < 0 && dy >= -limitVertical) {
			rot_axis[0] = 0;
			rot_axis[1] = valLEFTRIGHT;
		}

		// move down only
		if (dx < 0 && dx >= -limitHorizontal) {
			rot_axis[0] = -valUPRIGHT;
			rot_axis[1] = 0;
		}
	}
	
	if (velocity > 0.1) {
		rot_angle = velocity * 0.05 * 0.5;


		Globals::camera.rotate(rot_axis, rot_angle);
		rot_axis.print("vect");
		cout << "x: " << x << endl;
		cout << "dx: " << dx << endl;
		cout << "y: " <<y << endl;
		cout << "dy: " << dy << endl;

		

	}
	*/

	//gluLookAt(1.0, 0.0, 0.0, x, y, 0, 0.0, 1.0, 0.0);
	

// louis cam:
Vector3 axis = rotateAxis(x, y, Window::width, Window::height);
Vector3 neg_z(0, 0, -1);

Vector3 rot_axis = neg_z.cross(axis);
float velocity = (axis - neg_z).magnitude();

if (velocity > 0.001) {
	float rot_angle = velocity * 0.05;
	Globals::camera.rotate(rot_axis, rot_angle);
}

}