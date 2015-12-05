#include "Globals.h"

Camera Globals::camera;

Cube Globals::cube(10.0);
Sphere Globals::sphere(10, 50, 50);

Light Globals::light;

DrawData Globals::drawData;
UpdateData Globals::updateData;

CubeMapTexture Globals::testRoomTex;
RoomCube Globals::testRoom(500.0);

bool Globals::cam3Dmove_on = false;