#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "CubeMapTexture.h"
#include "RoomCube.h"
#include "Tree.h"

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
	static Sphere sphere;
    static Light light;
    static DrawData drawData;
    static UpdateData updateData;
    
	static CubeMapTexture testRoomTex;
	static RoomCube testRoom;

	/* flags */
	static bool cam3Dmove_on; // camera moving up/down?

	static Tree tree;

};

#endif
