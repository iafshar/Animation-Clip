#pragma once

#include "Vertex.h"
#include "Triangle.h"
#include "Tokenizer.h"
#include "../src/Skeleton.cpp"
#include "core.h"
#include <vector>
using namespace std;

class Skin {
    private:
        GLuint VAO;
        GLuint VBO_positions, VBO_normals, EBO;

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        glm::mat4 model;
        glm::vec3 color;

    public:
        vector<glm::mat4> Bs;
        vector<Vertex*> vertices;
        vector<Triangle*> triangles;

        bool Load(const char *file);
        void Update(Skeleton *skel);
        void Draw(const glm::mat4& viewProjMtx, GLuint shader);
};