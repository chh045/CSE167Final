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
const int MAX_DEPTH = 5;

class Tree
{
private:
	//string rule = "F[+YL][-YL]YL";  // F/Y = branch   +/-  = rotate    L = leaf
	string rule = "F[+YL][-YL][++YL][+++YL][--YL][---YL]YL";  // F/Y = branch   +/-  = rotate    L = leaf
	float lineLength = 0.5; 
	int lineWidth = 3;
	std::vector<string> language;

public:

	Tree();

	int current_depth = 0;

	float angle = 30.0;

	void drawLine();

	void draw();

	void expand();

	void printLanguage(int);
};
