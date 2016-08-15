#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
  public:
    GLuint Program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  vShaderFile.exceptions(std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::badbit);
  try
  {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "ERROR, shader file unsuccessfully read" << std::endl;
  }

  const GLchar* vShaderCode = vertexCode.c_str();
  const GLchar* fShaderCode = fragmentCode.c_str();

  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];
  //vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex,1,&vShaderCode, nullptr);
  glCompileShader(vertex);

  glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
  if(!success)
  {
    glGetShaderInfoLog(vertex,512, nullptr, infoLog);
    std::cout << "vertex compilation failed " << infoLog << std::endl;
  }
  //fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment,1, &fShaderCode, nullptr);
  glCompileShader(fragment);

  glGetShaderiv(fragment,GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
    std::cout << "fragment compilation failed " << infoLog << std::endl;
  }

  //Shader Program
  this->Program = glCreateProgram();
  glAttachShader(this->Program,vertex);
  glAttachShader(this->Program,fragment);
  glLinkProgram(this->Program);
  glGetShaderiv(this->Program,GL_LINK_STATUS,&success);
  if(!success)
  {
    glGetShaderInfoLog(this->Program, 512, nullptr, infoLog);
    std::cout << "program compilation failed " <<infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::Use()
{
  glUseProgram(this->Program);
}
#endif
