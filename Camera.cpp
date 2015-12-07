#include "Camera.h"
#include <math.h>

Camera::Camera()
{
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    
    //Pre-define a camera matrix (and its inverse) that are shifted 'e' from the origin
    //This is used as a default camera position for Project 1
    c.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, e[0], e[1], e[2], 1);
    ci.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
}

Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
    //Use these axes and the e vector to create a camera matrix c
    //Use c to solve for an inverse camera matrix ci

	//Update the Camera Matrix using d, e, and up
	//Solve for the z, x, and y axes of the camera matrix
	Vector3 z_c = (e - d).normalize();
	Vector3 x_c = (up.cross(z_c)).normalize();
	Vector3 y_c = (z_c.cross(x_c)).normalize();

	//Use these axes and the e vector to create a camera matrix c
	c.set(x_c[0], x_c[1], x_c[2], 0,
		y_c[0], y_c[1], y_c[2], 0,
		z_c[0], z_c[1], z_c[2], 0,
		e[0], e[1], e[2], 1);

	//Use c to solve for an inverse camera matrix ci
	//Ci = Ri x Ti
	Matrix4 R(x_c[0], x_c[1], x_c[2], 0,
		y_c[0], y_c[1], y_c[2], 0,
		z_c[0], z_c[1], z_c[2], 0,
		0, 0, 0, 1);
	Matrix4 Ri = R.transpose();

	Matrix4 Ti;
	Ti.identity();
	Ti.set(3, 0, -e[0]);
	Ti.set(3, 1, -e[1]);
	Ti.set(3, 2, -e[2]);

	ci = Ri*Ti;

}

void Camera::rotate(Vector3 axis, float angle) {

	// project e and d into camera space
	Vector3 e_cam = ci * e;
	Vector3 d_cam = ci * d;

	// translate e_cam and d_cam s.t. e_cam is at origin
	Vector3 old_e_cam = e_cam;
	e_cam = e_cam - old_e_cam;
	d_cam = d_cam - old_e_cam;

	// rotate d_cam around e_cam (also the origin) (in camera space
	Matrix4 m;
	d_cam = m.makeRotateArbitrary(axis, angle) * d_cam;

	// translate d_cam and e_cam back
	e_cam = e_cam + old_e_cam;
	d_cam = d_cam + old_e_cam;

	// project back to world space
	e = c * e_cam;
	d = c * d_cam;

	// re calculate up
	Vector3 true_up(0, 1, 0);
	Vector3 right = (d - e).cross(true_up);
	up = right.cross(d - e).normalize();

	//e.print("e is ");
	//d.print("d is ");

	update();
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    update();
}

Vector3 Camera::getPos() {
	return e;
}

Vector3 Camera::getLookDirec() {
	return d - e;
}

void Camera::moveFoward(bool changeY_on) {
	Vector3 dir = getLookDirec();

	if (!changeY_on)
		dir.set(1, 0);
	dir.normalize();
	dir.scale(0.4);

	e = e + dir;
	d = d + dir;

	update();
}

void Camera::moveBack(bool changeY_on) {
	Vector3 dir = getLookDirec();

	if (!changeY_on)
		dir.set(1, 0);
	dir.normalize();
	dir.scale(0.4);

	e = e - dir;
	d = d - dir;

	update();
}

void Camera::moveLeft() {
	Vector3 dir = getLookDirec();

	Vector3 rightDir = dir.cross(up);
	rightDir.normalize();
	rightDir.scale(0.4);

	e = e - rightDir;
	d = d - rightDir;

	update();
}

void Camera::moveRight() {
	Vector3 dir = getLookDirec();

	Vector3 rightDir = dir.cross(up);
	rightDir.normalize();
	rightDir.scale(0.4);

	e = e + rightDir;
	d = d + rightDir;

	update();
}




//--------------------------------------------------



void Camera::moveLeftRight(float value) {
    Matrix4 move;
    this->e.set(0, e[0] + value);
    this->d.set(0, d[0] + value);
    this->set(e, d, up);
}
void Camera::moveUpDown(float value) {
    Matrix4 move;
    float test = e.operator[](1);
    float test2 = d.operator[](1);
    this->e.set(1, test + value);
    this->d.set(1, test2 + value);
    this->set(e, d, up);
}
void Camera::lookLeftRight(float value) {
    Matrix4 move;
    this->d.set(0, d.operator[](0) + value);
    this->set(e, d, up);
}
void Camera::lookUpDown(float value) {
    Matrix4 move;
    float test = e.operator[](1);
    this->e.set(1, test + value);
    this->set(e, d, up);
    
}
void Camera::moveZoom(float value) {
    Matrix4 move;
    float test = e.operator[](2);
    float test2 = d.operator[](2);
    this->e.set(2, test + value);
    this->d.set(2, test2 + value);
    this->set(e, d, up);
}

void Camera::orbitTrack(Vector3 v, float angle) {
    Matrix4 orb;
    float pi = M_PI;
    float rads = angle * (pi / 180.0);
    orb.makeRotateArbitrary(v, rads);
    Vector3 lookAt = d - e;
    Vector3 newLookAt = orb * lookAt;
    d = e + newLookAt;
    this->set(e, d, up);
}

Vector3 Camera::getUp(){
    return up;
}
Vector3 Camera::getPosition(){
    return e;
}
Vector3 Camera::getLookAt(){
    return d;
}



