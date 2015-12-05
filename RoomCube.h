/**********************************************************
 Description:
	This class is dedicated for building a room. The normals
	of the 6 sides of the cube point inward, therefore we can
	stick textures on to the walls with cull_face mode enabled.
**********************************************************/
#ifndef CSE167_RoomCube_h
#define CSE167_RoomCube_h

#include "Drawable.h"

class RoomCube : public Drawable
{
	float size;
public:
	RoomCube(float roomLength);
	
	virtual void draw(DrawData&);
};

#endif

