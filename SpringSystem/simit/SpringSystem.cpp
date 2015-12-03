#include "SpringSystem.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>

#define str(s) #s
#define toString(s) str(s) 


using namespace std;

const int spacing = 2;
const int spr_k = 1e6;

float angleX = 0.f;
float angleY = 0.f;
float angleZ = 0.f;
float panX = 0.f;
float panY = 0.f;
float zoom = 1.25f;

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


SpringSystem::SpringSystem() : points(), springs(points,points) {


	int numX = spacing;
	int numY = spacing;
	int z = 0; // set z to 0 for in plane mesh
	
/*
	// Create points
	//DBG//cout<<"Building mesh vertices\n";
    for(int i = 0; i < numX; i++) {
        for(int j = 0; j < numY; j++) {
//DBG//cout << ">>>>Creating point (" << i << ", " << j << ", " << z << ")" << endl;
            mesh.v.push_back({static_cast<simit_float>(i), 
            	static_cast<simit_float>(j), static_cast<simit_float>(z)});
        }
    }
    
    // Create springs
	//DBG//cout<<"Building mesh edges\n";
    for(int i = 0; i < numX - 1; i++) {
        for(int j = 0; j < numY; j++) {
       	 cout << ">>>>Creating spring between points " << i*numY+j << " and " << 
       	 	(i+1)*numY+j << endl;
         mesh.edges.push_back({i*numY+j, (i+1)*numY+j});
        }
    }	

    for(int i = 0; i < numX; i++) {
        for(int j = 0; j < numY - 1; j++) {
       	 cout << ">>>>Creating spring between points " << i*numY+j << " and " << 
       	 	i*numY+(j+1) << endl;
         mesh.edges.push_back({i*numY+j, i*numY+(j+1)});
        }
    }	
*/
}

void SpringSystem::load() {

    //DBG//cout<<"Setting field references\n";	
    simit::FieldRef<simit_float,3> position = 
    	points.addField<simit_float,3>("position");
    simit::FieldRef<simit_float,3> velocity = 
    	points.addField<simit_float,3>("velocity");
    simit::FieldRef<simit_float> mass = points.addField<simit_float>("mass");

    simit::FieldRef<simit_float> k = springs.addField<simit_float>("k");
    simit::FieldRef<simit_float> L_0 = springs.addField<simit_float>("L_0");
    simit::FieldRef<simit_float> strain = 
    	springs.addField<simit_float>("strain");

    vector<simit::ElementRef> pointRefs;
    for(auto v : mesh.v) {
        pointRefs.push_back(points.add());
        simit::ElementRef pRef = pointRefs.back();
        
        //DBG//cout << v[0] << v[1] << v[2] << endl;
        position.set(pRef, {v[0], v[1], v[2]});
      	velocity.set(pRef, {0.0, 0.0, 0.0});
      	mass.set(pRef, 1.0);
	}

    for(auto e : mesh.edges) {
        simit::ElementRef sRef = springs.add(pointRefs[e[0]-1], 
        									 pointRefs[e[1]-1]); 
        std::array<double,3> pA = mesh.v[e[0]-1];
        std::array<double,3> pB = mesh.v[e[1]-1];      
        double spr_L0 = sqrt(((pA[0])-(pB[0])) * 		
        					((pA[0])-(pB[0])) +
        					((pA[1])-(pB[1])) * 		
        					((pA[1])-(pB[1])) +
        					((pA[2])-(pB[2])) * 		
        					((pA[2])-(pB[2])) );
		//DBG//cout << e[0] << " " << e[1] << endl;
		//DBG//cout << setprecision(10) <<pA[0] << " " << pB[0] <<endl 
		//DBG//	 << pA[1] << " " << pB[1] <<endl
		//DBG//	 << pA[2] << " " << pB[2] <<endl;        								
        //DBG//cout << spr_L0 << endl;
        float stretch = (rand() % 10)/100.0; 	//add random stretch to springs
        L_0.set(sRef, spr_L0*(1+stretch));
        k.set(sRef, spr_k);
        strain.set(sRef, 0);
    }


    // Load simit program here
    string filename = string(toString(SIMIT_CODE_DIR))+"/SpringSystem.sim";
    FILE *fin = fopen(filename.c_str(), "r");
   
    //DBG//cout<<"Loading "<<filename<<"\n";
    
    //load simit program
    int errorCode = program.loadFile(filename);

    if(errorCode) { cout<<program.getDiagnostics().getMessage(); exit(0); }

    //DBG//cout<<"Compiling \n";

    timeStepper = program.compile("main");

    //DBG//cout<<"Binding \n";
    timeStepper.bind("points", &points);
    timeStepper.bind("springs", &springs);
    //DBG//cout<<"Initializing \n";
    timeStepper.init();
    
    }
    
void SpringSystem::step() {

	int numSteps = 10000;

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1024, 768, "Simit PM Spring Example", NULL, NULL);
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
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (spinning) {
			double oldXPos = xpos;
			double oldYPos = ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			angleX += (xpos - oldXPos);
			while (angleX > 360) angleX -= 360;
			angleY += (ypos - oldYPos);
			while (angleY > 360) angleY -= 360;
		}
		if (rolling) {
			double oldXPos = xpos;
			double oldYPos = ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			angleZ += sqrt((xpos - oldXPos)*(xpos - oldXPos) +
							(ypos - oldYPos)*(ypos - oldYPos));
			while (angleZ > 360) angleZ -= 360;		
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
		}
  
		//DBG//cout << "Position : Velocity " << endl;
		simit::FieldRef<simit_float,3> pos = 	
			points.getField<simit_float,3>("position");
		simit::FieldRef<simit_float,3> vel = 
			points.getField<simit_float,3>("velocity");

		glColor3f(0.f,0.f,0.f);
		glPointSize(8.f);
		glBegin(GL_POINTS);
		for (auto p : points) {
			//DBG//cout << pos.get(p) << " : " << vel.get(p) << endl;
		
			glVertex3f((pos.get(p)(0)-spacing/2)/spacing*1.5, 
						(pos.get(p)(1)-spacing/2)/spacing*1.5, 
						(pos.get(p)(2)-spacing/2)/spacing*1.5);
		}
		glEnd();

		//DBG//cout << endl;
		//DBG//cout << "Strain" << endl;

		simit::FieldRef<simit_float,3> str = 
			springs.getField<simit_float,3>("strain");

		glColor3f(0.5f, 0.5f, 0.5f);
		glLineWidth(2.f);
		for (auto s : springs) {
			simit::ElementRef ep0 = springs.getEndpoint(s,0);
			simit::ElementRef ep1 = springs.getEndpoint(s,1);
			glBegin(GL_LINES);
			glVertex3f((pos.get(ep0)(0)-spacing/2)/spacing*1.5, 
						(pos.get(ep0)(1)-spacing/2)/spacing*1.5, 
						(pos.get(ep0)(2)-spacing/2)/spacing*1.5);
			glVertex3f((pos.get(ep1)(0)-spacing/2)/spacing*1.5, 
						(pos.get(ep1)(1)-spacing/2)/spacing*1.5, 
						(pos.get(ep1)(2)-spacing/2)/spacing*1.5);						
			glEnd();
			//DBG//cout << pos.get(ep0) << ", " << pos.get(ep1) << endl;
			//DBG//cout << str.get(s) << endl;
		}

		//DBG//cout << endl;
		//DBG//cout << endl;   
		//DBG//cout<<"Done \n";

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
 
}

bool SpringSystem::loadObject(const char * file_name) {

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

    // num of vertices and indices
    //fscanf( fp, "data%d%d", &number_vertices, &number_faces);

       
    //push the content into the vertices
	float x_temp, y_temp, z_temp;
	char* obj_id;

    int EOFp = fscanf( fp, "%s %f %f %f ", obj_id, &x_temp, &y_temp,&z_temp );
  
	while ((obj_id[0] == 'v') && (obj_id[1] != 'n')){
		if (EOFp == EOF) break;
		mesh.v.push_back({static_cast<simit_float>(x_temp), 
			static_cast<simit_float>(y_temp), 
			static_cast<simit_float>(z_temp)});
        EOFp = fscanf( fp, "%s %f %f %f ", obj_id, &x_temp, &y_temp,&z_temp );
    } 

    cout << endl;
	while ((obj_id[0] != 'f') && (EOFp != EOF)) {
        EOFp = fscanf( fp, "%s", obj_id);
	}
    cout << endl;
    
    int a,b,c;

    //push the face content into the edges
    while (obj_id[0] == 'f') {
        EOFp = fscanf( fp, "%s %d %d %d", obj_id, &a, &b, &c);
		if (EOFp == EOF) break;
		mesh.edges.push_back({a, b});
		mesh.edges.push_back({b, c});
		mesh.edges.push_back({a, c});
    } 

cout << "Number of vertices loaded: " << mesh.v.size() << endl;
cout << "Number of edges loaded: " << mesh.edges.size() << endl;
	return true;
}

