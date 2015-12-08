#ifndef _NODE_H_
#define _NODE_H_

#include "DrawData.h"
#include "UpdateData.h"
#include "Material.h"

class Node
{
public:
	Node();
	~Node();
	virtual void draw(DrawData&)=0;
	virtual void update(UpdateData&) = 0;
	Material material;
	
};
#endif

