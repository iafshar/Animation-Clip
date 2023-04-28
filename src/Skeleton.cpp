#pragma once
#include "Joint.cpp"

using namespace std;

class Skeleton {
    public:
        Joint Root;
        bool Load(const char *file) {
            Tokenizer token;
            token.Open(file);
            token.FindToken("balljoint");
            char n[256];
            token.GetToken(n);
            // Parse tree
            // Root=new Joint();
            Root=Joint((char*)n);
            Root.Load(token);
            // Finish
            token.Close();
            cout << Root.name << endl;
            return true;
        }
        void Update() {
            Root.Update(Root.parent);
        }
        void Draw(glm::mat4 viewMatrix,GLuint shaderProgram) {
            Root.Draw(viewMatrix,shaderProgram);
        }
        vector<Joint> getJoints(Joint joint) {
            vector<Joint> joints = {joint};
            if (joint.children.size() == 0)
            {
                return joints; 
            }
            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    vector<Joint> recursive = getJoints(joint.children[i]);
                    joints.insert(joints.end(),recursive.begin(),recursive.end());
                }
                return joints;
            } 
        }
        glm::mat4 getWorldMatrix(int joint) {
            return getJoints(Root)[joint].W;
        } 
        string getName(int joint) {
            return getJoints(Root)[joint].name;
        }
        int getNumJoints() {
            return getJoints(Root).size();
        }
        float getX(int j, Joint& joint) {
            if (j == 0)
            {
                
                //cout << "X: " << joint.DOFs[0].getValue() << endl;

                return joint.DOFs[0].getValue();
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    getX(j, joint.children[i]);
                }
                
            }
        }
        void setX(int& j, float f, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[0].setValue(f);
                //cout << "X: " << joint.DOFs[0].getValue() << endl;

                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    setX(j,f, joint.children[i]);
                }
                
            }
        }
        void setY(int& j, float f, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[1].setValue(f);
                //cout << "Y: " << joint.DOFs[0].getValue() << endl;

                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    setY(j,f, joint.children[i]);
                }
                
            }
        }
        void setZ(int& j, float f, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[2].setValue(f);
                //cout << "X: " << joint.DOFs[0].getValue() << endl;

                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    setZ(j,f, joint.children[i]);
                }
                
            }
        }
        void increaseX(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[0].update(0.05);
                cout << "X: " << joint.DOFs[0].getValue() << endl;

                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    increaseX(j,joint.children[i]);
                }
                
            }
        }
        void decreaseX(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[0].update(-0.05);
                //std::cout << &joint.name << std::endl;
                cout << "X: " << joint.DOFs[0].getValue() << endl;
                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    decreaseX(j,joint.children[i]);
                }
                
            }
        }

        void increaseY(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[1].update(0.05);
                cout << "Y: " << joint.DOFs[1].getValue() << endl;
                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    increaseY(j,joint.children[i]);
                }
                
            }
        }
        void decreaseY(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[1].update(-0.05);
                cout << "Y: " << joint.DOFs[1].getValue() << endl;
                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    decreaseY(j,joint.children[i]);
                }
                
            }
        }
        void increaseZ(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[2].update(0.05);
                cout << "Z: " << joint.DOFs[2].getValue() << endl;
                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    increaseZ(j,joint.children[i]);
                }
                
            }
        }
        void decreaseZ(int& j, Joint& joint) {
            if (j == 0)
            {
                joint.DOFs[2].update(-0.05);
                cout << "Z: " << joint.DOFs[2].getValue() << endl;
                return;
            }

            else {
                for (int i = 0; i < joint.children.size(); i++)
                {
                    j--;
                    decreaseZ(j,joint.children[i]);
                }
                
            }
        }
};


//[root,head,hipL,kneeL,hipR,kneeR]