#ifndef CSE167_OBJObject_h
#define CSE167_OBJObject_h

//#include <iostream>
//#include <vector>
#include <string>
#include "Vector3.h"
#include "Drawable.h"
#include "Geode.h"
//#include "UpdateData.h"


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
	Vector3 max_x, max_y, max_z;
	Vector3 min_x, min_y, min_z;
	Vector3 center_bounding;
	float radius_bounding;
	
	bool isCharacter;

    //Storage vectors
	std::string objName;
    std::vector<Vector3*>* vertices;
    std::vector<Vector3*>* normals;
    std::vector<Vector3*>* colors;
    std::vector<Face*>* faces;
    
    std::vector<float> allVertex;
    std::vector<float> allNormal;
	std::vector<float> allColor;

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
	void findEdgePoint(void);
	void findBoundingBox(void);
    //why nothing here ? lol
    
public:
	static bool bbox_on;
    OBJObject();
    OBJObject(std::string, bool isCharac=false);
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
