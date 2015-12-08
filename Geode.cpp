#include "geode.h"

Geode::~Geode()
{
}

void Geode::draw(DrawData& d) {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(d.m->ptr());
    //glLoadIdentity();
	render();
}
