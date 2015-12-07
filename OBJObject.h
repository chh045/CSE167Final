#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"
#include "Geode.h"
#include "UpdateData.h"


struct Face
{
    int vertexIndices[4];
    int normalIndices[4];
    //Add more members as necessary
    
    int colorIndices[4];
};

class OBJObject : public Drawable
{
    
protected:
    
    //Storage vectors
    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    std::vector<Vector3*>* colors;
    std::vector<Face*>* faces;
    
    std::vector<float> allVertex;
    std::vector<float> allNormal;
    
    std::vector<Vector3> rasterizeVertices;
    std::vector<Vector3> rasterizeNormals;
    
    //int printTimes;
    
    //Helper functions
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string&, char);
    
    std::vector<float> vert;
    
    //Parse
    void parse(std::string&);
    void load();
    //why nothing here ? lol
    
public:
    
    OBJObject();
    OBJObject(std::string);
    virtual ~OBJObject(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    std::vector<Vector3> getRasterVertices();
    std::vector<Vector3> getRasterNormals();
    
    void render();
    //void update(UpdateData&);
    
    void normalize();
    
    
    
    
};

#endif
