#ifndef CSE167_RenderData_h
#define CSE167_RenderData_h

#include <iostream>
#include "Matrix4.h"
#include "Camera.h"
#include "Vector3.h"

class DrawData
{
    
public:
    
    //Place any objects here that you want to pass to a draw() call
    Matrix4* m;
    Camera* cam;
    //Light* light;
    DrawData(void);
    ~DrawData(void);
    
    void setMatrix(Matrix4 c);
    Matrix4 getMatrix();
    //Light getLight();
    
};

#endif
