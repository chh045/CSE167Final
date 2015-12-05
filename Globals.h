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

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
	static Sphere sphere;
    static Light light;
    static DrawData drawData;
    static UpdateData updateData;
    //Feel free to add more member variables as needed
    
	static CubeMapTexture testRoomTex;
	static RoomCube testRoom;
};

#endif
