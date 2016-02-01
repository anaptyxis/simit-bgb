#ifndef _Elastic2D_Elastic2D_h
#define _Elastic2D_Elastic2D_h

#include "graph.h"
#include "program.h"
#include "error.h"
#include "function.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <iostream>

class Elastic2D
{
public:
    
    Elastic2D();
	void load();
	void step();
	bool loadObject(const char * file_name);
	void update_angle();
    
protected:

    simit::MeshVol mesh;   
    simit::Set points;
    simit::Set hyperedges;
    simit::Program program;
    simit::Function timeStepper;
    
};

#endif
