#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Geode.h"
#include "UpdateData.h"


class Cube : public Geode{
    
public:
    Cube(float s);
    ~Cube();
    void render();
    void update(UpdateData&);
private:
    
    
    double size;
};

#endif

