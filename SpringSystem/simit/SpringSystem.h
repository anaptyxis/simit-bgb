#ifndef _SpringSystem_SpringSystem_h
#define _SpringSystem_SpringSystem_h

#include "graph.h"
#include "program.h"
#include "error.h"
#include "function.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <iostream>

class SpringSystem
{
public:
    
    SpringSystem();
	void load();
	void step();
	bool loadObject(const char * file_name);
	void update_angle();
    
protected:

    simit::MeshVol mesh;   
    simit::Set points;
    simit::Set springs;
    simit::Program program;
    simit::Function timeStepper;
    
};

#endif
