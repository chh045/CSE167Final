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
	string rule = "F[+YL][-YL][++YL][+++YL][++++YL][--YL][---YL][----YL][+++++YL][-----YL]YL";  // type 1  ( good lengths = 0.1, 0.2 )
	string DefaultRule = "F[+YL][-YL][++YL][+++YL][--YL][---YL]YL";  // type 1  ( good lengths = 0.1, 0.2 )
	string rule1 = "F[+YL]F[-YL]+YL";  // type 2 ( good lengths = 0.1, 0.2 )
	string rule2 = "F[[YL]++YL]+F[+FL]-YL";  // widht 10 looks good
	string ruleChosen;
	float lineLength = 0.2; 
	int lineWidth = 10;
	std::vector<string> language;

public:

	Tree();

	Tree(int);

	int current_depth = 0;

	float angle = 30.0;

	void drawLine();

	void draw();

	void expand();

	void printLanguage(int);
};
