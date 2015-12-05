#include "Camera.h"

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
	Vector3 old_e = e;

	// translate d and e s.t. e is at the origin
	e = e - old_e;
	d = d - old_e;

	// rotate d around e (also the origin)
	Matrix4 m;
	d = m.makeRotateArbitrary(axis, angle) * d;

	// translate back
	e = e + old_e;
	d = d + old_e;

	e.print("e is ");
	d.print("d is ");

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