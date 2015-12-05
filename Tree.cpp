#include "Tree.h"


Tree::Tree() {
}

void Tree::drawLine() {

	glLineWidth(lineWidth);

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, lineLength, 0);
	glEnd();

	glTranslatef(0, lineLength, 0);

}


void Tree::draw() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	string str = "";


	for (int i = 0; i < language.size(); i++) {
		str = language.at(i);

		if (str.compare("F") == 0 || str.compare("Y") == 0) {
			glColor3f(1, 0, 0); // red
			drawLine();
		}
		else if (str.compare("[") == 0) {
			if(lineWidth>2)
			lineWidth-=2;
			glPushMatrix();
		}
		else if (str.compare("]") == 0) {
			lineWidth+=2;
			glPopMatrix();
		}
		else if (str.compare("L") == 0) {
			glColor3f(0, 1, 0); // green
			glBegin(GL_TRIANGLES);

			glVertex3f(0, 0, 0);
			glVertex3f(-0.15, 0, -0.4);
			glVertex3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(0.15, 0, 0.4);
			glVertex3f(0, 1.0, 0);

			glEnd();
		}
		else if (str.compare("+") == 0) {
			glRotatef(-ANGLE, 1, 0, 0);
			glRotatef(ANGLE * 8, 0, 1, 0);
			glRotatef(-ANGLE, 0, 0, 1);
		}
		else if (str.compare("-") == 0) {
			glRotatef(ANGLE, 1, 0, 0);
			glRotatef(ANGLE * 8, 0, 1, 0);
			glRotatef(ANGLE, 0, 0, 1);
		}


	}

	glPopMatrix();
}

void Tree::expand() {

	string newLanguage = "";

	for (int i = 0; i < language.length(); i++) {

		if (language.at(i) == 'F')
			newLanguage.append("FF");
		else if (language.at(i) == 'Y')
			newLanguage.append("F[+YL]F[-YL]+Y");
		else
			newLanguage += language.at(i);
		}
	language = newLanguage;
}

void Tree::printLanguage() {

	cout << language << endl;

}