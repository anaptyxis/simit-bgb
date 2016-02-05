/***
Project: GLFW3 Basecode
Version: 0.5
Author : r3dux
Date   : 21/1/2014
Purpose: Basecode to setup an OpenGL context with FPS camera controls and draw some grids.
***/

#include <iostream>

// Define that we're using the static version of GLEW (glew32s) so that it gets built
// into our final executable.
// NOTE: This MUST be defined before importing GLEW!
#define GLEW_STATIC

// Include the GL Extension Wrangler. Note: GLEW should always be the very first include
#include <GL/glew.h>

#include <GLFW/glfw3.h>                 // Include GL Framework. Note: This pulls in GL.h for us.

// Include the GL Mathematics library
#define GLM_FORCE_RADIANS               // We must work in radians in newer versions of GLM...
#include <glm/glm.hpp>                  // ...so now that's defined we can import GLM itself.

// Include our custom classes
#include "Camera.h"
#include "Grid.h"
#include "Utils.h"

// Save ourselves some typing...
using std::cout;
using std::endl;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

// ---------- Global variables ----------

// Window and projection settings
GLsizei windowWidth       = 800;
GLsizei windowHeight      = 600;
float vertFieldOfViewDegs = 45.0f;
float nearClipDistance    = 1.0f;
float farClipDistance     = 2000.0f;

// Misc
int  frameCount = 0;              // How many frames we've drawn
int  frameRate  = 60;             // Target frame rate -we'll assume a 60Hz refresh for now
bool leftMouseButtonDown = false; // We'll only look around when the left mouse button is down

// Matricies
mat4 projectionMatrix; // The projection matrix is used to perform the 3D to 2D conversion i.e. it maps from eye space to clip space.
mat4 viewMatrix;       // The view matrix maps the world coordinate system into eye cordinates (i.e. world space to eye space)
mat4 modelMatrix;      // The model matrix maps an object's local coordinate system into world coordinates (i.e. model space to world space)

// Pointers to two grids
Grid *upperGrid, *lowerGrid;

// Camera. Params: location, rotation (degrees), window width & height
Camera camera(vec3(0.0f), vec3(0.0f), windowWidth, windowHeight);

// Callback function to resize the window and set the viewport to the correct size
void resizeWindow(GLFWwindow *window, GLsizei newWidth, GLsizei newHeight)
{
    // Keep track of the new width and height of the window
    windowWidth  = float(newWidth);
    windowHeight = float(newHeight);

    // Recalculate the projection matrix
    projectionMatrix = glm::perspective(vertFieldOfViewDegs, GLfloat(windowWidth) / GLfloat(windowHeight), nearClipDistance, farClipDistance);

    // Viewport is the entire window
    glViewport(0, 0, windowWidth, windowHeight);

    // Update the midpoint location in the camera class because it uses these values, too
    camera.updateWindowMidpoint(windowWidth, windowHeight);
}

// Callback function to handle keypresses
void handleKeypress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // User hit ESC? Set the window to close
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else
    {
        camera.handleKeypress(key, action);
    }
}

// Callback function to handle mouse movement
void handleMouseMove(GLFWwindow *window, double mouseX, double mouseY)
{
    // We'll only look around when the left mouse button is down
    if (leftMouseButtonDown)
    {
        camera.handleMouseMove(window, mouseX, mouseY);
    }
}

// Callback function to handle mouse button presses
void handleMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    // Button press involves left mouse button?
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        if (action == GLFW_PRESS)
        {
            glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);
            leftMouseButtonDown = true;
        }
        else // Action must be GLFW_RELEASE
        {
            leftMouseButtonDown = false;
        }
    }
}

// Function to set up our OpenGL rendering context
void initGL(GLFWwindow *window)
{
    // ---------- Initialise GLEW ----------

    // Enable glewExperimental which ensures that all extensions with valid entry points will be exposed.
    glewExperimental = true;

    // Note: We MUST have an OpenGL rendering context open to initialise GLEW successfully!
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
    }
    cout << "GLEW intialised successfully. Using GLEW version: " << glewGetString(GLEW_VERSION) << endl << endl;

    // Depending on the OpenGL context settings, calling glewInit() can sometimes cause a GL_INVALID_ENUM error.
    // As this issue isn't really our code's fault, we'll check the error here to clear it.
    //
    // Cause: In a core profile context, GL_EXTENSIONS is an invalid constant to pass to glGetString (...). You
    // must use the new glGetStringi (...) function. GLEW does not do this by default, given a core context
    // without being informed to use glGetStringi (...), GLEW will use glGetString (...) and will cause GL to
    // generate a GL_INVALID_ENUM error. In order to get GLEW to use glGetStringi (...) (which you should ONLY
    // do in an OpenGL 3.0+ context), set glewExperimental = true; before calling glewInit (...).
    //
    // Source: http://stackoverflow.com/questions/19453439/solved-opengl-error-gl-invalid-enum-0x0500-while-glewinit
    checkGLError("glewInit - harmless / ignore");

    // ---------- Setup OpenGL Options ----------

    glViewport( 0, 0, GLsizei(windowWidth), GLsizei(windowHeight) ); // Viewport is entire window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                            // Clear to black with full alpha
    glEnable(GL_DEPTH_TEST);                                         // Enable depth testing
    glDepthFunc(GL_LEQUAL);                                          // Specify depth testing function
    glClearDepth(1.0);                                               // Clear the full extent of the depth buffer (default)
    glEnable(GL_CULL_FACE);                                          // Enable face culling
    glCullFace(GL_BACK);                                             // Cull back faces of polygons
    glFrontFace(GL_CCW);                                             // Counter-clockwise winding indicates a forward facing polygon (default)

    // ---------- Setup GLFW Callback Functions ----------

    glfwSetWindowSizeCallback(window, resizeWindow);                 // Register window resize functiom
    glfwSetKeyCallback(window, handleKeypress);                      // Register keyboard handler function
    glfwSetCursorPosCallback(window, handleMouseMove);               // Register mouse movement handler function
    glfwSetMouseButtonCallback(window, handleMouseButton);           // Register mouse button handler function

    // ---------- Setup GLFW Options ----------

    glfwSwapInterval(1);                                             // Swap buffers every frame (i.e. lock to VSync)
    glfwSetInputMode(window, GLFW_CURSOR_DISABLED, GL_FALSE);        // Do not hide the mouse cursor
    glfwSetWindowPos(window, 200, 200);                              // Push the top-left of the window out from the top-left corner of the screen
    glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);     // Move the mouse cursor to the centre of the window

    checkGLError("InitGL");
}

// Function to perform our drawing
void drawFrame()
{
    // Move the camera
    camera.move(1.0f/ frameRate);

    // ---------- Matrix operations ----------

    // Reset our View matrix
    viewMatrix = mat4(1.0f);

    // Perform camera rotation
    viewMatrix = glm::rotate(viewMatrix, camera.getXRotationRads(), X_AXIS);
    viewMatrix = glm::rotate(viewMatrix, camera.getYRotationRads(), Y_AXIS);

    // Translate to our camera position
    viewMatrix = glm::translate(viewMatrix, -camera.getLocation() );

    // Create an identity matrix for the model matrix
    modelMatrix = mat4(1.0f);

    // ---------- Drawing operations ----------

    mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    upperGrid->draw(mvpMatrix);
    lowerGrid->draw(mvpMatrix);
}

int main()
{
    // ----- Initialiise GLFW, specify window hints & open a context -----

    // IMPORTANT: glfwInit resets all window hints, so we must call glfwInit FIRST and THEN we supply window hints!
    if (!glfwInit())
    {
        cout << "glfwInit failed!" << endl;
        exit(-1);
    }

    // Further reading on GLFW window hints: http://www.glfw.org/docs/latest/window.html#window_hints

    // If we want to use a a core profile (i.e. no legacy fixed-pipeline functionality) or if we want to
    // use forward compatible mode (i.e. only non-deprecated features of a given OpenGL version available)
    // then we MUST specify the MAJOR.MINOR context version we want to use FIRST!
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Ask for 4x Anti-Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a window. Params: width, height, title, *monitor, *share
    GLFWwindow* window = glfwCreateWindow(GLsizei(windowWidth), GLsizei(windowHeight), "GLFW3 Basecode | Use WSAD to move & LMB to look around - Nov 2014 | r3dux.org ", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create window - bad context MAJOR.MINOR version?" << endl;
        getKeypressThenExit();
    }

    // Make the current OpenGL context active
    glfwMakeContextCurrent(window);

    // Display the details of our OpenGL window
    displayWindowProperties(window);

    // -------------- Set up our OpenGL settings ---------------

    initGL(window);

    // ---------- Set up our grids ----------

    // Instantiate our grids. Params: Width, Depth, level (i.e. location of y-axis), number of grid lines
    upperGrid = new Grid(1000.0f, 1000.0f,  200.0f, 20);
    lowerGrid = new Grid(1000.0f, 1000.0f, -200.0f, 20);

    // ---------- Set up our matricies ----------

    // Specify the projection matrix
    projectionMatrix = glm::perspective(vertFieldOfViewDegs, GLfloat(windowWidth) / GLfloat(windowHeight), nearClipDistance, farClipDistance);

    // Reset the view and model and view matrices to identity
    viewMatrix  = mat4(1.0f);
    modelMatrix = mat4(1.0f);

    // ---------- Main loop ----------

    while ( !glfwWindowShouldClose(window) )
    {
        frameCount++;

        // Clear the screen and depth buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Draw our frame
        drawFrame();

        // Swap the back and front buffers to display the frame we just rendered
        glfwSwapBuffers(window);

        // Poll for input
        glfwPollEvents();
    }

    // Check the final error state
    // NOTE: This MUST be called while we still have a valid rendering context (i.e. before we call glfwTerminate() )
    checkGLError("End");

    // Destroy the window and exit
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}