#include "core.h"
#include "Tokenizer.h"
#include "Cube.h" 
#include "DOF.h"
#include <glm/gtx/string_cast.hpp>

using namespace std;

class Joint {
     public:
        glm::mat4 L;
        glm::mat4 W;
        glm::vec3 Offset;
        glm::vec3 Boxmin;
        glm::vec3 Boxmax;
        std::vector<DOF> DOFs;
        std::vector<Joint> children;
        glm::mat4 parent;
        Cube* jointCube;
        std::string name;

        Joint();
        void setParent(glm::mat4 newParent);
        void Update(glm::mat4 dad = glm::mat4(1.0f));
        void AddChild(Joint j);
        bool Load(Tokenizer &token);
        void Draw(glm::mat4 viewMatrix,GLuint shaderProgram);

};