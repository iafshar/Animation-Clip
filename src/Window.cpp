#include "Window.h"

bool Window::skelExists = false;
bool Window::skinExists = false;
bool Window::animExists = false;

int Window::currentJoint;
int Window::numJoints;
// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Skeleton* Window::skel;
Skin* Window::skin;
Animation* Window::clip;
Player* Window::player;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects(const char *file1, const char *file2, const char *file3) {
    //TODO: check files and handle.
    // Create a cube
    //std::cout << std::strlen(file1) << std::endl;
    if (std::strlen(file2) < 5)
    {
        if (file1[std::strlen(file1)-5] == '.' && file1[std::strlen(file1)-4] == 's' && file1[std::strlen(file1)-3] == 'k' && file1[std::strlen(file1)-2] == 'e' && file1[std::strlen(file1)-1] == 'l')
        {
            skelExists = true;
        }
        else if (file1[std::strlen(file1)-5] == '.' && file1[std::strlen(file1)-4] == 's' && file1[std::strlen(file1)-3] == 'k' && file1[std::strlen(file1)-2] == 'i' && file1[std::strlen(file1)-1] == 'n')
        {
            skinExists = true;
        }
    }
    else if (strlen(file3) < 5)
    {
        skinExists = true;
        skelExists = true;
    }
    else 
    {
        skinExists = true;
        skelExists = true;
        animExists = true;
    }
    
    if (skelExists)
    {
        skel = new Skeleton();
        skel->Load(file1);
        Window::numJoints = skel->getNumJoints();
    }
    else if (skinExists && !skelExists)
    {
        skin = new Skin();
        skin->Load(file1);
        Window::numJoints = 0;
    }
    if (skinExists && skelExists)
    {
        skin = new Skin();
        skin->Load(file2);
        if (animExists)
        {
            clip = new Animation();
            clip->load(file3);
            player = new Player();
            player->SetClip(clip);
            player->skel = skel;
        }  
    }
    
    
    // skel = new Skeleton();
    // skel->Load(file1);
    // skin = new Skin();
    // skin->Load(file2);
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    Window::currentJoint = 0;
    
    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    if (skelExists)
    {
        delete skel;
    }
    
    if (skinExists)
    {
        delete skin;
    }

    if (animExists)
    {
        delete clip;
        delete player;
    }
    
    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    if (skelExists)
    {
        skel->Update();
        if (skinExists)
        {
            skin->Update(skel);
        } 
    }
    //cout << animExists << endl;
    if (animExists)
    {   
        player->skel = skel;
        //Joint &tempJoint = skel->Root;
        //cout << skel->getX(currentJoint,tempJoint) << endl;
        player->update(0.008);
        // skel->Update();
        // skin->Update(skel);
        //cout << skel->Root.DOFs[0].getValue() << endl;
    }
    
    
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    //cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    if (skinExists)
    {
        skin->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
        //skel->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    if (skelExists && !skinExists)
    {
        skel->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    
    // skin->Draw(Cam->GetViewProjectMtx() * skel->getWorldMatrix(0), Window::shaderProgram);
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            case GLFW_KEY_R: {
                resetCamera();
                break;
            }
            case GLFW_KEY_DOWN: {
                if (skelExists)
                {
                    Window::currentJoint += 1;
                    if (Window::currentJoint >= Window::numJoints)
                    {
                        Window::currentJoint = Window::numJoints - 1;
                    }
                    std::cout << skel->getName(currentJoint) << std::endl;
                }
                break;
            }
            case GLFW_KEY_UP: {
                if (skelExists)
                {
                
                    Window::currentJoint -= 1;
                    if (Window::currentJoint < 0)
                    {
                        Window::currentJoint = 0;
                    }
                    std::cout << skel->getName(currentJoint) << std::endl;
                }
                break;
            }
            case GLFW_KEY_S: {
                if (skelExists)
                {
                    Joint &tempJoint = skel->Root;
                    int currentTempJoint = currentJoint;
                    skel->increaseX(currentTempJoint,tempJoint);
                    //cout << skel->getX(currentJoint,tempJoint) << endl;
                }
                break;
            }
            case GLFW_KEY_X: {
                if (skelExists)
                {
                    int currentTempJoint = currentJoint;
                    Joint &tempJoint = skel->Root;
                    skel->decreaseX(currentTempJoint,tempJoint);
                    
                }
                break;
            }
            case GLFW_KEY_Y: {
                if (skelExists)
                {
                    int currentTempJoint = currentJoint;
                    Joint &tempJoint = skel->Root;
                    skel->increaseY(currentTempJoint,tempJoint);
                }
                break;
            }
            case GLFW_KEY_H: {
                if (skelExists)
                {
                    int currentTempJoint = currentJoint;
                    Joint &tempJoint = skel->Root;
                    skel->decreaseY(currentTempJoint,tempJoint);
                }
                break;
            }
            case GLFW_KEY_A: {
                if (skelExists)
                {
                    int currentTempJoint = currentJoint;
                    Joint &tempJoint = skel->Root;
                    skel->increaseZ(currentTempJoint,tempJoint);
                }
                break;
            }
            case GLFW_KEY_Z: {
                if (skelExists)
                {
                    int currentTempJoint = currentJoint;
                    Joint &tempJoint = skel->Root;
                    skel->decreaseZ(currentTempJoint,tempJoint);
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}