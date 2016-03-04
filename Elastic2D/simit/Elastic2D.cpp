#include "Elastic2D.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>

#define str(s) #s
#define toString(s) str(s) 

using namespace std;

const int spr_k = 0;//1e4;

float angleX = 0.f;
float angleY = 0.f;
float angleZ = 0.f;
float zoom = 1.5f;
float panX = -0.5f;
float panY = -0.5f;
double xpos = -1, ypos = -1;
bool spinning = false;
bool rolling = false;

/* GLFW Viewer callbacks */
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		if (button == GLFW_MOUSE_BUTTON_LEFT) 
			spinning = true;
		if (button == GLFW_MOUSE_BUTTON_RIGHT) 
			rolling = true;
	}

	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) 
			spinning = false;
		if (button == GLFW_MOUSE_BUTTON_RIGHT) 
			rolling = false;
	}
    
	
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	float scroll_sensitivity = 0.25;
	panX -= xoffset*scroll_sensitivity;
	panY -= yoffset*scroll_sensitivity;
}

/* ********************* */


Elastic2D::Elastic2D() : points(), hyperedges(points,points,points) {

}

void Elastic2D::load() {

	srand ( time(NULL) );
	// Point field references
    simit::FieldRef<simit_float,2> init_position = 
    	points.addField<simit_float,2>("init_position");
    simit::FieldRef<simit_float,2> position = 
    	points.addField<simit_float,2>("position");
    simit::FieldRef<simit_float,2> velocity = 
    	points.addField<simit_float,2>("velocity");
    simit::FieldRef<bool> pinned = points.addField<bool>("pinned");
    
	//Hyperedge field references
    simit::FieldRef<simit_float,2,2> strain = 
    	hyperedges.addField<simit_float,2,2>("strain");
    simit::FieldRef<simit_float> init_area = 
    	hyperedges.addField<simit_float>("init_area");    	
    simit::FieldRef<simit_float> mass = 
    	hyperedges.addField<simit_float>("mass");    	
    simit::FieldRef<simit_float,4,6> dDphi = 
    	hyperedges.addField<simit_float,4,6>("dDphi");
    simit::FieldRef<simit_float,4,4> dStrain = 
    	hyperedges.addField<simit_float,4,4>("dStrain");
    simit::FieldRef<simit_float,4> dEnergyDensity = 
    	hyperedges.addField<simit_float,4>("dEnergyDensity");
    simit::FieldRef<simit_float,6,500> dEnergy = 
    	hyperedges.addField<simit_float,6,500>("dEnergy");
//    simit::FieldRef<simit_float,6,500> T_i = 
//    	hyperedges.addField<simit_float,6,500>("T_i");
    	
    vector<simit::ElementRef> pointRefs;
    bool pinStart = true;
    for(auto v : mesh.v) {
        pointRefs.push_back(points.add());
        simit::ElementRef pRef = pointRefs.back();
        
        init_position.set(pRef, {v[0], v[1]});
        position.set(pRef, {v[0], v[1]});      	
      	float dx = (rand() % 4)-2.0, dy = (rand() % 4)-2.0;
   	    velocity.set(pRef, {dx, dy});      	
      	if (pinStart) {
//     	    velocity.set(pRef, {dx, dy});
      		mass.set(pRef, numeric_limits<float>::infinity());
	      	pinned.set(pRef, true);     
	      	pinStart = false; 		
		} else {
//	      	velocity.set(pRef, {0.0, 0.0});
	      	mass.set(pRef, 1.0);
      		pinned.set(pRef, false);
      	}
	}

    for(int idx = 0; idx < mesh.edges.size(); ) {
    
    	std::array< int,2> e1 = mesh.edges[idx++];
    	std::array< int,2> e2 = mesh.edges[idx++];
    	std::array< int,2> e3 = mesh.edges[idx++];
    	    	
    	//Check for bad triangles
    	if (e1[1] != e2[0]) 
    		cerr << "e1 != e2 : " << e1[1] << ", " << e2[0] << endl;
    	if (e2[1] != e3[0]) 
    		cerr << "e2 != e3 : " << e2[1] << ", " << e3[0] << endl;
    	if (e3[1] != e1[0]) 
    		cerr << "e3 != e1 : " << e3[1] << ", " << e1[0] << endl;
    		
        simit::ElementRef heRef = hyperedges.add(pointRefs[e1[0]], 
        									    pointRefs[e2[0]],
        									    pointRefs[e3[0]]); 
        std::array<double,3> pA = mesh.v[e1[0]];
        std::array<double,3> pB = mesh.v[e2[0]];   
        std::array<double,3> pC = mesh.v[e3[0]];   
           
		init_area.set(heRef, 0.0);

    }

    // Load simit program here
    string filename = string(toString(SIMIT_CODE_DIR))+"/Elastic2D.sim";
    FILE *fin = fopen(filename.c_str(), "r");
   
    //DBG//cout<<"Loading "<<filename<<"\n";
    
    //load simit program
    int errorCode = program.loadFile(filename);

    if(errorCode) { cout<<program.getDiagnostics().getMessage(); exit(0); }

    //DBG//cout<<"Compiling \n";
    precomputation = program.compile("init");

    precomputation.bind("points", &points);
    precomputation.bind("hyperedges", &hyperedges);
    precomputation.runSafe();

    timeStepper = program.compile("main");

    //DBG//cout<<"Binding \n";
    timeStepper.bind("points", &points);
    timeStepper.bind("hyperedges", &hyperedges);
    //DBG//cout<<"Initializing \n";
    timeStepper.init();
    
    }
    
void Elastic2D::step() {

	int numSteps = -1;		// negative for unbounded

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1024, 768, "Simit Elastic2D Example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 10.f, -10.f);
		glMatrixMode(GL_MODELVIEW);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);		
		glLoadIdentity();
		if (spinning) {
			double oldXPos = xpos;
			double oldYPos = ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			angleX += fmod((xpos - oldXPos),360.f);
			angleY += fmod((ypos - oldYPos),360.f);
		}
		if (rolling) {
			double oldXPos = xpos;
			double oldYPos = ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			angleZ += fmod(sqrt((xpos - oldXPos)*(xpos - oldXPos) +
							(ypos - oldYPos)*(ypos - oldYPos)),360.f);

		}
		glRotatef(angleZ, 0.f, 0.f, 1.f);
		glRotatef(angleX, 0.f, 1.f, 0.f);
		glRotatef(angleY, 1.f, 0.f, 0.f);
		glScalef(zoom, zoom, zoom);
		glTranslatef(panX, panY, 0.f);
		//DBG//cout<<"Running\n";
		if (numSteps > 0) {
			timeStepper.run();
			numSteps--;
		} else if (numSteps < 0) {
			timeStepper.run();
		}
  
		//DBG//cout << "Position : Velocity " << endl;
		simit::FieldRef<simit_float,2> pos = 	
			points.getField<simit_float,2>("position");
		simit::FieldRef<simit_float,2> vel = 
			points.getField<simit_float,2>("velocity");
		simit::FieldRef<bool> pin = 
			points.getField<bool>("pinned");

		glPointSize(4.f*zoom);
		glBegin(GL_POINTS);
		for (auto p : points) {
			//DBG//cout << pos.get(p) << " : " << vel.get(p) << endl;
		if (pin.get(p))
			glColor3f(1.f,0.f,0.f);
		else
			glColor3f(0.f,0.f,0.f);
			glVertex3f((pos.get(p)(0)), 
   					   (pos.get(p)(1)), 
						0.f);
//					   (pos.get(p)(2)));
		}
		glEnd();

		//DBG//cout << endl;
		//DBG//cout << "Strain" << endl;

//		simit::FieldRef<simit_float,3> str = 
//			hyperedges.getField<simit_float,3>("strain");

		glColor3f(0.5f, 0.5f, 0.5f);
		glLineWidth(2.f);
		int hyperEdgeCounter = 0;
		for (auto s : hyperedges) {
		
			hyperEdgeCounter++;
			simit::ElementRef ep0 = hyperedges.getEndpoint(s,0);
			simit::ElementRef ep1 = hyperedges.getEndpoint(s,1);
			simit::ElementRef ep2 = hyperedges.getEndpoint(s,2);

			glBegin(GL_TRIANGLES);
			glColor3f(0.f,0.f,0.f);
			glVertex3f((pos.get(ep0)(0)), 
						(pos.get(ep0)(1)), 
						0.f);
//						(pos.get(ep0)(2)));
			glVertex3f((pos.get(ep1)(0)), 
						(pos.get(ep1)(1)), 
						0.f);
//						(pos.get(ep1)(2)));	
			glVertex3f((pos.get(ep2)(0)), 
						(pos.get(ep2)(1)), 
						0.f);
//						(pos.get(ep2)(2)));												
			glEnd();

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
 
}

bool Elastic2D::loadObject(const char * file_name) {

    //check if the file opens
    if ( !file_name ) {
        return false;
    }

    // open file
    FILE *fp = fopen( file_name, "r" );
    if ( !fp ) {
		cerr << "Unable to open " << file_name << endl;
        return false;
    }

	float x_temp, y_temp, z_temp;
	int a,b,c;
	char* type;
	int faceCount = 0;
	int EOFp;

	EOFp = fscanf( fp, "%s", type );
	while ((EOFp != EOF) && (strcmp( type, "" ) != 0)) {
	    //push the content into the vertices
		if (strcmp( type, "v" ) == 0) {
			fscanf( fp, "%f %f %f", &x_temp, &y_temp, &z_temp );
			mesh.v.push_back({static_cast<simit_float>(x_temp), 
				static_cast<simit_float>(y_temp), 
				static_cast<simit_float>(z_temp)});
 		}
	    //push the face content into the edges
 		if (strcmp( type, "f" ) == 0) {
			fscanf( fp, "%d %d %d", &a, &b, &c );
			mesh.edges.push_back({a-1, b-1});
			mesh.edges.push_back({b-1, c-1});
			mesh.edges.push_back({c-1, a-1});
			faceCount++;
 		}
		EOFp = fscanf( fp, "%s", type ); 
 	}
 
	cout << "Number of vertices loaded: " << mesh.v.size() << endl;
	cout << "Number of edges loaded: " << mesh.edges.size() << endl;
	cout << "Number of faces loaded: " << faceCount << endl;

	
	//create T_i
	int m[faceCount][6][2*mesh.v.size()];
	
	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < 3; j++) 
			for (int k = 0; k < mesh.v.size(); k++) {
				if (mesh.edges[i*3+j][0] == k) {
					m[i][j*2][2*k] = 1;
					m[i][j*2][2*k+1] = 0;
					m[i][j*2+1][2*k] = 0;
					m[i][j*2+1][2*k+1] = 1;
				}
				else {
					m[i][j*2][2*k] = 0;
					m[i][j*2][2*k+1] = 0;
					m[i][j*2+1][2*k] = 0;
					m[i][j*2+1][2*k+1] = 0;				}
			}
	}

	for (int i = 0; i < faceCount; i++) {
		for (int j = 0; j < 6; j++) {
			for (int k = 0; k < 2*mesh.v.size(); k++) 
				std::cout << m[i][j][k];
			std::cout << "\n";
		}
		std::cout << "\n";
	}

//	exit(0);
	return true;
}

