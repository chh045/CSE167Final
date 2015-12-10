#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);
Sphere Globals::sphere(3, 50, 50);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

CubeMapTexture Globals::testRoomTex;
RoomCube Globals::testRoom(3000.0);

Texture* Globals::grdTex = 0;
Texture* Globals::wallTex = 0;

float Globals::grd_length = 100;
float Globals::grd_width = 100;
float Globals::grd_depth = -20;
float Globals::wall_height = 25;
bool Globals::bounding_box_on = false;
bool Globals::env_mapping_on = true;

bool Globals::cam3Dmove_on = false;

Tree Globals::tree(0);
Tree Globals::tree1(1);
Tree Globals::tree2(2);

Particle Globals::particle;



bool Globals::emulateDay = false;

Group* Globals::group = new Group();

OBJObject Globals::bunny("bunny.obj", true);

OBJObject Globals::girl("girl.obj", true);

OBJObject Globals::house("house.obj", false);

OBJObject Globals::monster("monster.obj", true);

Shader Globals::skybox("skybox.vert", "skybox.frag", true);