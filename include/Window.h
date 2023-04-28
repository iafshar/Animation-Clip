#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "Skin.h"
#include "../src/Skeleton.cpp"
#include "Player.h"
#include "core.h"

class Window {
public:

    static bool skinExists;
    static bool skelExists;
    static bool animExists;
    
    static int currentJoint;
    static int numJoints;
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Objects to render
    static Skeleton* skel;
    static Skin* skin;
    static Animation* clip;
    static Player* player;


    // Shader Program
    static GLuint shaderProgram;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects(const char *file1, const char *file2, const char *file3);
    static void cleanUp();

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};