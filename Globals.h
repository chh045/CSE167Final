#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Cube.h"
#include "Sphere.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "OBJObject.h"
#include "CubeMapTexture.h"
#include "RoomCube.h"
#include "Tree.h"
#include "Particle.h"
#include "Group.h"
#include "Shader.h"



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

	static Particle particle;
    
    
    static bool emulateDay;
    static Group* group;
    static OBJObject bunny;
    
    //static Shader skybox;

};

#endif
