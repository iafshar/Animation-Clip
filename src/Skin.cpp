#include "Skin.h"

//I think the problem is not multiplying viewProjMatrix by W in proj but dont know how to get W/which W to use

bool Skin::Load(const char *file) {
    // Model matrix.
    model = glm::mat4(1.0f);

    // The color of the Skin. Try setting it to something else!
    color = glm::vec3(1.0f, 1.0f, 1.0f);


    Tokenizer token;
    token.Open(file);

    token.FindToken("positions");
    int nVertices = token.GetInt();
    token.SkipLine();
    for (int i = 0; i < nVertices; i++)
    {
        Vertex *vrt = new Vertex;
        glm::vec3 pos = glm::vec3(token.GetFloat(),token.GetFloat(),token.GetFloat());
        vrt->position = pos;

        positions.push_back(pos);
        vertices.push_back(vrt);
    }
    token.FindToken("normals");
    token.SkipLine();
    for (int i = 0; i < vertices.size(); i++)
    {
        glm::vec3 norm = glm::vec3(token.GetFloat(),token.GetFloat(),token.GetFloat());
        vertices[i]->normal = norm;

        normals.push_back(norm);
    }
    token.FindToken("skinweights");
    token.SkipLine();
    for (int i = 0; i < vertices.size(); i++)
    {
        int nAttachments = token.GetInt();
        //cout << nAttachments << endl;
        for (int j = 0; j < nAttachments; j++)
        {
            int jIndex = token.GetInt();
            float weight = token.GetFloat();
            std::pair<int,float> attachment;
            attachment.first = jIndex;
            attachment.second = weight;
            vertices[i]->attachments.push_back(attachment);
        }
        
    }

    token.FindToken("triangles");
    int nTriangles = token.GetInt();
    token.SkipLine();
    for (int i = 0; i < nTriangles; i++)
    {
        Triangle *tri = new Triangle;
        tri->vertices = glm::vec3(token.GetInt(),token.GetInt(),token.GetInt());

        indices.push_back(tri->vertices.x);
        indices.push_back(tri->vertices.y);
        indices.push_back(tri->vertices.z);
        triangles.push_back(tri);
    }

    token.FindToken("bindings");
    int nBs = token.GetInt();
    token.SkipLine();
    for (int i = 0; i < nBs; i++)
    {
        token.FindToken("matrix");
        token.SkipLine();
        glm::mat4 currB = glm::mat4(token.GetFloat(),token.GetFloat(),token.GetFloat(),0,
                                    token.GetFloat(),token.GetFloat(),token.GetFloat(),0,
                                    token.GetFloat(),token.GetFloat(),token.GetFloat(),0,
                                    token.GetFloat(),token.GetFloat(),token.GetFloat(),1);
        Bs.push_back(currB); // are the binding matrices in .skin really flipped?
    }                           

    token.Close();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void Skin::Update(Skeleton *skel){
    vector<glm::mat4> Ms;
    for (int i = 0; i < Bs.size(); i++)
    {
        glm::mat4 W = skel->getWorldMatrix(i);
        
        glm::mat4 M = W * glm::inverse(Bs[i]);
        //cout << glm::to_string(W) << endl;
        Ms.push_back(M);
    }
    
    for (int i = 0; i < vertices.size(); i++)
    {
        glm::vec3 newPos = glm::vec3(0,0,0);
        glm::vec3 newNorm = glm::vec3(0,0,0);
        for (int j = 0; j < vertices[i]->attachments.size(); j++)
        {
            float wi = vertices[i]->attachments[j].second;
            glm::mat4 Mi = Ms[vertices[i]->attachments[j].first];

            glm::vec3 currPos = wi * (Mi * glm::vec4(vertices[i]->position,1));
            newPos += currPos;
            //cout << wi << endl;

            glm::vec3 currNorm = wi * (glm::inverse(glm::transpose(Mi))) * glm::vec4(vertices[i]->normal,0);
            newNorm += currNorm;

        }
        // if (vertices[i]->position.x != newPos.x || vertices[i]->position.y != newPos.y || vertices[i]->position.z != newPos.z)
        // {
        //     cout << "yes" << endl;
        // }
        
        // vertices[i]->position = newPos;
        // vertices[i]->normal = glm::normalize(newNorm);

        positions[i] = newPos;
        normals[i] = glm::normalize(newNorm);
    }
    model = glm::mat4(1.0f);


    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW);
    

    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);


}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
    
}
