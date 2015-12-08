/*            :::::::::::WARNING::::::::::::::
** PLEASE, DO NOT INCLUDE SAME HEADER MORE THAN ONCE.
** PLEASE, GLEE.H ALWAYS DEFINED BEFORE GLUT.H
** PLEASE, ADD ONLY HEADERS IN H-FILES IF YOU NEED TO USE IN THE HEADER
** OTHERWISE, PUT IT IN .CPP FILE
**/

#ifndef _GEODE_H_
#define _GEODE_H_


#include "Node.h"

#ifdef __APPLE__
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


class Geode : public Node{
public:
	Geode(){};
	void draw(DrawData& d);
	~Geode();

	void scale(float, float, float);
	void translate(float,float,float);
	void rotateX(float);
	void rotateY(float);
	void rotateZ(float);
    Material materials;
	
protected:
	virtual void update(UpdateData&)=0;
	virtual void render() = 0;

};


#endif