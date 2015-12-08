#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);
Sphere Globals::sphere(1, 50, 5);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

CubeMapTexture Globals::testRoomTex;
RoomCube Globals::testRoom(500.0);

bool Globals::cam3Dmove_on = false;

Tree Globals::tree;

Particle Globals::particle;



bool Globals::emulateDay = false;

Group* Globals::group = new Group();

OBJObject Globals::bunny("bunny.obj");

Shader Globals::skybox("skybox.vert", "skybox.frag", true);