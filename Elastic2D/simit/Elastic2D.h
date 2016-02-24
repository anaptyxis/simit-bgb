#ifndef _Elastic2D_Elastic2D_h
#define _Elastic2D_Elastic2D_h

#include "graph.h"
#include "program.h"
#include "error.h"
#include "function.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>

class Elastic2D
{
public:
    
    Elastic2D();
	void load();
	void step();
	bool loadObject(const char * file_name);
	float precompute_area(Eigen::Vector2f vA, 
						  Eigen::Vector2f vB, 
						  Eigen::Vector2f vC);
    
protected:

    simit::MeshVol mesh;   
    simit::Set points;
    simit::Set hyperedges;
    simit::Program program;
    simit::Function timeStepper;
    
};

#endif
