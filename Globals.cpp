#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);
Sphere Globals::sphere(1, 50, 5);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

CubeMapTexture Globals::testRoomTex;
RoomCube Globals::testRoom(3000.0);

Texture* Globals::grdTex = 0;
Texture* Globals::wallTex = 0;
float Globals::grd_length = 1000;
float Globals::grd_width = 700;
float Globals::grd_depth = -20;
float Globals::wall_height = 50;

bool Globals::cam3Dmove_on = false;

Tree Globals::tree(0);
Tree Globals::tree1(1);
Tree Globals::tree2(2);

Particle Globals::particle;



bool Globals::emulateDay = false;

Group* Globals::group = new Group();

OBJObject Globals::bunny("bunny.obj");

OBJObject Globals::girl("girl.obj");

OBJObject Globals::house("house.obj");

OBJObject Globals::monster("monster.obj");

Shader Globals::skybox("skybox.vert", "skybox.frag", true);