
//#include "Window.h"
#include "math.h"
#include <sstream>
#include <fstream>
#include "OBJObject.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define deleteVector(__type__, __vect__) do {\
std::vector<__type__>::iterator iter; \
std::vector<__type__>::iterator end; \
iter = __vect__->begin();\
end = __vect__->end();\
while(iter != end) delete (*(iter++));\
delete __vect__;\
} while(false)





OBJObject::OBJObject(std::string filename) : Drawable()
{
    this->vertices = new std::vector<Vector3*>();
    this->normals = new std::vector<Vector3*>();
    this->colors = new std::vector<Vector3*>();
    this->faces = new std::vector<Face*>();
    //this->rasterizeVertices = new std::vector<Vector3*>();
    
    parse(filename);
}

OBJObject::~OBJObject()
{
    //Delete any dynamically allocated memory/objects here
    
    deleteVector(Vector3*, vertices);
    deleteVector(Vector3*, normals);
    deleteVector(Face*, faces);
    deleteVector(Vector3*, colors);
    //deleteVector(Vector3*, rasterizeVertices);
}

void OBJObject::draw(DrawData& data)
{
    
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, allNormal.data());
    glVertexPointer(3, GL_FLOAT, 0, allVertex.data());
    glDrawArrays(GL_TRIANGLES, 0, allVertex.size()/3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
    
}

void OBJObject::update(UpdateData& data)
{
    //
}

void OBJObject::parse(std::string& filename)
{
    
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float xn, yn, zn; // normal coordinates
    float r,g,b;  // vertex color
    char c1,c2;    // characters read from file
    int indicesVertex[3], indicesNormal[3];
    char lines[100];
    char c3;
    int lineNum = 0;
    fp = fopen(filename.c_str(),"rb");
    
    /*vertices->push_back(new Vector3(0, 0, 0));
     normals->push_back(new Vector3(0, 0, 0));
     colors->push_back(new Vector3(0, 0, 0));
     */
    if (!fp) {
        return;
    }
    
    while(fgets(lines,80,fp)){
        
        //Progress
        if(++lineNum % 10000 == 0)
            std::cout << "At line " << lineNum << std::endl;
        
        c1 = fgetc(fp);
        if(c1 == '\r')
            continue;
        c2 = fgetc(fp);
        if ((c1=='v') && (c2==' ')){
            fscanf(fp, "%f %f %f%c", &x, &y, &z, &c3);
            if( c3 == ' '){
                fscanf(fp, "%f %f %f", &r, &g, &b);
                //colors->push_back(new Vector3(r, g, b));
            }
            vertices->push_back(new Vector3(x, y, z));
        }
        
        else if ((c1 == 'v') && (c2 == 'n')){
            fscanf(fp, "%f %f %f", &xn, &yn, &zn);
            normals->push_back(new Vector3(xn, yn, zn));
        }
        else if ((c1 == 'f') && (c2 == ' ')){
            
            
            
            fscanf(fp, "%d//%d %d//%d %d//%d", &indicesVertex[0], &indicesNormal[0],&indicesVertex[1], &indicesNormal[1],
                   &indicesVertex[2], &indicesNormal[2]);
            
            
            Face* facees = new Face;
            for (int i =0; i< 3; i++){
                facees->normalIndices[i] = indicesNormal[i] - 1;
                facees->vertexIndices[i] = indicesVertex[i] - 1;
                
            }
            faces->push_back(facees);
        }
    }
    
    load();
    fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::load(){
    
    for (int i = 0; i < faces->size(); i++){
        for (int j = 0; j< 3; j++){
            for (int k = 0; k< 3; k++){
                allVertex.push_back(vertices->at(faces->at(i)->vertexIndices[j])->operator[](k));
                allNormal.push_back(normals->at(faces->at(i)->normalIndices[j])->operator[](k));
            }
        }
    }
}

//Split functions from the interwebs
//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string>& OBJObject::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> OBJObject::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


std::vector<Vector3> OBJObject::getRasterVertices(){
    
    for (int i = 0; i < allVertex.size(); i+=3) {
        rasterizeVertices.push_back(Vector3(allVertex[i], allVertex[i+1], allVertex[i+2]));
    }
    
    return rasterizeVertices;
}


std::vector<Vector3> OBJObject::getRasterNormals(){
    
    for (int i = 0; i < allNormal.size(); i+=3) {
        rasterizeNormals.push_back(Vector3(allNormal[i], allNormal[i+1], allNormal[i+2]));
    }
    
    return rasterizeNormals;
}



void OBJObject::normalize(){
    
    
    Matrix4 temp;
    
    
}




void OBJObject::render(){
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, allNormal.data());
    glVertexPointer(3, GL_FLOAT, 0, allVertex.data());
    glDrawArrays(GL_TRIANGLES, 0, allVertex.size()/3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}


/*
 void OBJObject::draw(DrawData& data)
 {
 
 material.apply();
 
 glMatrixMode(GL_MODELVIEW);
 
 glPushMatrix();
 glMultMatrixf(toWorld.ptr());
 
 glBegin(GL_TRIANGLES);
 
 
 //Loop through the faces
 //For each face:
 //  Look up the vertices, normals (if they exist), and texcoords (if they exist)
 //  Draw them as triplets:
 
 //      glNorm(normals->at(face.normalIndices[0]))
 //      glVert(vertices->at(face.vertexIndices[0]))
 //      Etc.
 //
 
 Vector3* v1;
 Vector3* v2;
 Vector3* v3;
 Vector3* n1;
 Vector3* n2;
 Vector3* n3;
 Vector3* c1 = nullptr;
 Vector3* c2 = nullptr;
 Vector3* c3 = nullptr;
 
 //rasterizeVertices->push_back(new Vector3(0,0,0));
 
 //std::cerr<<"hello world\n";
 
 for (std::vector<Face*>::iterator it = faces->begin(), end = faces->end(); it != end; ++it) {
 
 v1 = vertices->at((*it)->vertexIndices[0]);
 rasterizeVertices->push_back(v1);
 v2 = vertices->at((*it)->vertexIndices[1]);
 rasterizeVertices->push_back(v2);
 v3 = vertices->at((*it)->vertexIndices[2]);
 rasterizeVertices->push_back(v3);
 
 n1 = normals->at((*it)->normalIndices[0]);
 n2 = normals->at((*it)->normalIndices[1]);
 n3 = normals->at((*it)->normalIndices[2]);
 
 
 if (colors->size()>1) {
 c1 = colors->at((*it)->colorIndices[0]);
 c2 = colors->at((*it)->colorIndices[1]);
 c3 = colors->at((*it)->colorIndices[2]);
 }
 
 //draw the first point of the face
 if(c1 != nullptr)
 glColor3f( (*c1)[0], (*c1)[1], (*c1)[2]);
 glNormal3f((*n1)[0], (*n1)[1], (*n1)[2]);
 glVertex3f((*v1)[0], (*v1)[1], (*v1)[2]);
 
 //draw the second point of the face
 if(c2 != nullptr)
 glColor3f( (*c2)[0], (*c2)[1], (*c2)[2]);
 glNormal3f((*n2)[0], (*n2)[1], (*n2)[2]);
 glVertex3f((*v2)[0], (*v2)[1], (*v2)[2]);
 
 //draw the third point of the face
 if(c3 != nullptr)
 glColor3f( (*c3)[0], (*c3)[1], (*c3)[2]);
 glNormal3f((*n3)[0], (*n3)[1], (*n3)[2]);
 glVertex3f((*v3)[0], (*v3)[1], (*v3)[2]);
 
 
 rasterizeVertices->push_back(v1);
 
 }
 glEnd();
 
 //my code
 //glEnableClientState(GL_VERTEX_ARRAY);
 //glVertexPointer(<#GLint size#>, <#GLenum type#>, 0, const GLvoid *pointer);
 
 //end mycode
 
 glPopMatrix();
 
 }*/

/*
 void OBJObject::parse(std::string& filename)
 {
 std::ifstream infile(filename);
 std::string line;
 std::vector<std::string> tokens;
 std::string token;
 
 int lineNum = 0;
 
 vertices->push_back(new Vector3(0, 0, 0));
 normals->push_back(new Vector3(0, 0, 0));
 colors->push_back(new Vector3(0, 0, 0));
 
 
 std::cout << "Starting parse..." << std::endl;
 
 //While all your lines are belong to us
 while (std::getline(infile, line))
 {
 //Progress
 if(++lineNum % 100000 == 0)
 std::cout << "At line " << lineNum << std::endl;
 
 //Split a line into tokens by delimiting it on spaces
 //"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
 tokens.clear();
 tokens = split(line, ' ', tokens);
 
 //If first token is a v then it gots to be a vertex
 if(!tokens.empty() && tokens.at(0).compare("v") == 0)
 {
 //Parse the vertex line
 float x = std::stof(tokens.at(1));
 float y = std::stof(tokens.at(2));
 float z = std::stof(tokens.at(3));
 //std::cerr<<"size of tokens: "<<tokens.size()<<"\n";
 
 //if the current line has color presented, read color into file
 if(tokens.size() == 7){
 
 float r = std::stof(tokens.at(4));
 float g = std::stof(tokens.at(5));
 float b = std::stof(tokens.at(6));
 colors->push_back(new Vector3(r, g, b));
 }
 
 vertices->push_back(new Vector3(x, y, z));
 
 }
 else if(!tokens.empty() && tokens.at(0).compare("vn") == 0)
 {
 //Parse the normal line
 float x = std::stof(tokens.at(1));
 float y = std::stof(tokens.at(2));
 float z = std::stof(tokens.at(3));
 Vector3 n(x, y, z);
 n = n.normalize();
 normals->push_back(new Vector3(n[0], n[1], n[2]));
 
 }
 else if(!tokens.empty() && tokens.at(0).compare("f") == 0)
 {
 Face* face = new Face;
 
 //Parse the face line
 std::vector<std::string> x;
 std::vector<std::string> y;
 std::vector<std::string> z;
 
 x.clear();
 y.clear();
 z.clear();
 //std::cerr<<"tokens(1): "<<tokens.at(1)<<"  tokens(2): "<<tokens.at(2)<<"  tokens(3): "<<tokens.at(3)<<"\n";
 
 x = split(tokens.at(1), '/', x);
 y = split(tokens.at(2), '/', y);
 z = split(tokens.at(3), '/', z);
 
 //std::cerr<<"size of x[]: "<<x.size()<<"  x(0): "<<x.at(0)<<"  x(1): "<<x.at(1)<<"  x(2): "<<x.at(2)<<"\n";
 
 std::vector<std::vector<std::string>> v;
 v.push_back(x);
 v.push_back(y);
 v.push_back(z);
 
 for (int i = 0; i < 3; i++) {
 face->vertexIndices[i] = std::stof(v[i].at(0));
 face->colorIndices[i] = std::stof(v[i].at(0));
 face->normalIndices[i] = std::stof(v[i].at(2));
 }
 
 v.clear();
 
 faces->push_back(face);
 }
 else if(tokens.at(0).compare("How does I are C++?!?!!") == 0)
 {
 //Parse as appropriate
 //There are more line types than just the above listed
 //See the Wavefront Object format specification for details
 }
 
 }
 //load();
 std::cout << "Done parsing." << std::endl;
 }*/
