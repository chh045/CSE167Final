#ifndef CSE167_Camera_h
#define CSE167_Camera_h

#include "Matrix4.h"
#include "Vector3.h"

class Camera
{
private:
    
protected:
    
    Matrix4 c;   //Camera Matrix
    Matrix4 ci;  //Inverse of Camera Matrix
    
public:
    
    Vector3 e;   //Center of Projection
    Vector3 d;   //Look At
    Vector3 up;  //Up Vector ^
    
    Camera(void);
    ~Camera(void);
    
    void update(void);
    
	void rotate(Vector3 axis, float angle);

    Matrix4& getMatrix(void);
    Matrix4& getInverseMatrix(void);
    
    void set(Vector3&, Vector3&, Vector3&);
	Vector3 getPos(void);
	Vector3 getLookDirec(void);

	void moveFoward(bool changeY_on);
	void moveBack(bool changeY_on);
	void moveLeft(void);
	void moveRight(void);
    
    
    
    
    void moveLeftRight(float);
    void moveUpDown(float);
    void lookLeftRight(float);
    void lookUpDown(float);
    void moveZoom(float);
    
    void orbitTrack(Vector3 v, float angle);
    
    Vector3 getUp();
    Vector3 getPosition();
    Vector3 getLookAt();
};

#endif
