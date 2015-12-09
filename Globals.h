

#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Shader.h"
#include "CubeMapTexture.h"

#include "Texture.h"
#include "Tree.h"
#include "Particle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Light.h"
#include "DrawData.h"
#include "OBJObject.h"
#include "Group.h"
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
    
	static CubeMapTexture testRoomTex;
	static RoomCube testRoom;

	static Texture* grdTex;
	static Texture* wallTex;
	static float grd_length;
	static float grd_width;
	static float grd_depth;
	static float wall_height;
	

	/* flags */
	static bool cam3Dmove_on; // camera moving up/down?

	static Tree tree;

	static Particle particle;
    
    
    static bool emulateDay;
    static Group* group;
    static OBJObject bunny;
    
    static Shader skybox;
};

#endif
