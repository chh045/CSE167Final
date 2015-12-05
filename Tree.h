#ifndef Tree_h
#define Tree_h
#endif

#include <iostream>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;


const float ANGLE = 42.0;

class Tree
{
private:
	string language = "F[+YL][-YL]YL";  // F/Y = branch   +/-  = rotate    L = leaf
	float lineLength = 0.5; 
	int lineWidth = 6;

public:

	Tree();
	int depth = 0;

	float angle = 0;

	void drawLine();

	void draw();

	void expand();

	void printLanguage();

};
