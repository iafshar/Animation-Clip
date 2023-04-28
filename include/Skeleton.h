#pragma once
#include "Joint.h"

using namespace std;

class Skeleton
{
private:
    vector<Joint> getJoints(Joint joint);
public:
    Joint Root;

    bool Load(const char *file);
    void Update();
    void Draw(glm::mat4 viewMatrix,GLuint shaderProgram);
    glm::mat4 getWorldMatrix(int joint);
    string getName(int joint);
    int getNumJoints();
    void increaseX(int j, Joint& joint);
    void decreaseX(int j, Joint& joint);
    void increaseY(int j, Joint& joint);
    void decreaseY(int j, Joint& joint);
    void increaseZ(int j, Joint& joint);
    void decreaseZ(int j, Joint& joint);
};

