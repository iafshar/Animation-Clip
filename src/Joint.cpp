#include "core.h"
#include "Tokenizer.h"
#include "Cube.h" 
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>

using namespace std;
//rotate may or may not use radians. Right now it's in radians

class DOF {
    private: 
        float value; 
        float min;
        float max;
        

    public:
        void setValue(float v) {
            if (v < min) {
                value = min;
            }
            else if (v > max) {
                value = max;
            }
            else {
                value = v;
            }
        }
        float getValue() {
            return value;
        }
        void setMin(float mi) {
            min = mi;
        }
        void setMax(float ma) {
            max = ma;
        }
        void update(float change) {
            value += change;
            if (value < min) {
                value = min;
            }
            else if (value > max) {
                value = max;
            }
        }
};

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
        //char* name;
        std::string name;

        Joint(char n[] = "a") {
            L = glm::mat4(1.0f);
            W = glm::mat4(1.0f);
            Offset = glm::vec3(0,0,0);
            Boxmin = glm::vec3(-0.1,-0.1,-0.1);
            Boxmax = glm::vec3(0.1,0.1,0.1);

            DOF x;
            DOF y;
            DOF z;
            x.setMin(-10000);
            x.setMax(10000);
            x.setValue(0);

            y.setMin(-10000);
            y.setMax(10000);
            y.setValue(0);

            z.setMin(-10000);
            z.setMax(10000);
            z.setValue(0);

            DOFs.push_back(x);
            DOFs.push_back(y);
            DOFs.push_back(z);

            parent = glm::mat4(1.0f);
            int array_size = sizeof(n)/ sizeof(char);  // getting the size of character array
            name = "";  // initialising our result with an empty string

            for(int i = 0; i < array_size; i++)
            {

            name = name +n[i];
            }

            //std::cout << name << std::endl;

        }
        void setParent(glm::mat4 newParent) {
            parent = newParent;
        }
        void Update(glm::mat4 dad = glm::mat4(1.0f)) {
            glm::mat4 Rx = glm::rotate(DOFs[0].getValue(), glm::vec3(1,0,0));
            glm::mat4 Ry = glm::rotate(DOFs[1].getValue(), glm::vec3(0,1,0));
            glm::mat4 Rz = glm::rotate(DOFs[2].getValue(), glm::vec3(0,0,1));

            //DOFs[2].update(0.05);

            //std::cout << DOFs[2].getValue() << std::endl;
            glm::mat4 R = Rz * Ry * Rx;
            
            glm::mat4 T = glm::translate(Offset);
            L = T * R;
            setParent(dad);
            W = dad * L;
            
            for (int i = 0; i < children.size(); i++)
            {
                children[i].Update(W);
            }
        }

        void AddChild(Joint j) {
            children.push_back(j);
        }

        bool Load(Tokenizer &token) {
            token.FindToken("{");
            while(1) {
                char temp[256];
                token.GetToken(temp);
                if(strcmp(temp,"offset")==0) {
                    Offset.x=token.GetFloat();
                    Offset.y=token.GetFloat();
                    Offset.z=token.GetFloat();
                }
                else if(strcmp(temp,"boxmin")==0) {
                    Boxmin.x=token.GetFloat();
                    Boxmin.y=token.GetFloat();
                    Boxmin.z=token.GetFloat();
                }
                else if(strcmp(temp,"boxmax")==0) {
                    Boxmax.x=token.GetFloat();
                    Boxmax.y=token.GetFloat();
                    Boxmax.z=token.GetFloat();
                }
                else if(strcmp(temp,"rotxlimit")==0) {
                    DOFs[0].setMin(token.GetFloat());
                    DOFs[0].setMax(token.GetFloat());
                }
                else if(strcmp(temp,"rotylimit")==0) {
                    DOFs[1].setMin(token.GetFloat());
                    DOFs[1].setMax(token.GetFloat());
                }
                else if(strcmp(temp,"rotzlimit")==0) {
                    DOFs[2].setMin(token.GetFloat());
                    DOFs[2].setMax(token.GetFloat());
                }
                else if(strcmp(temp,"pose")==0) {
                    DOFs[0].setValue(token.GetFloat());
                    DOFs[1].setValue(token.GetFloat());
                    DOFs[2].setValue(token.GetFloat());
                }
                else if(strcmp(temp,"balljoint")==0) {
                    char n[256];
                    token.GetToken(n);
                    
                    Joint *jnt=new Joint(n);
                    jnt->Load(token);
                    AddChild(*jnt);
                }
                else if(strcmp(temp,"}")==0) return true;
                else token.SkipLine(); // Unrecognized token
            }
        }

        void Draw(glm::mat4 viewMatrix,GLuint shaderProgram) {
            jointCube = new Cube(Boxmin,Boxmax);
            jointCube->draw(viewMatrix * W,shaderProgram);
            for (int i = 0; i < children.size(); i++)
            {
                children[i].Draw(viewMatrix,shaderProgram);
            }
            
        }

};