#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh{
  public:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
    {
      this->vertices=vertices;
      this->indices=indices;
      SetupMesh();
    }

    void Draw ()
    {
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
      glBindVertexArray(0);
    }

    void Delete()
    {
      glDeleteVertexArrays(1, &VAO);
      glDeleteBuffers(1,&VBO);
    }
  private:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    void SetupMesh(){
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);
      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER,VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices.at(0), GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices.at(0), GL_STATIC_DRAW);
      glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }
};
