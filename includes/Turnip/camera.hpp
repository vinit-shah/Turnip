#pragma once

#include <vector>

#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

enum CameraDirection {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera
{
  public:
    Camera(vec3 Pos, vec3 Front, vec3 Up)
    {
      cameraPos = Pos;
      cameraFront = Front;
      cameraUp = Up;
    }
    mat4 getViewMatrix()
    {
      return glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    }
    void ProcessKeys (CameraDirection direction, GLfloat deltaTime)
    {
      GLfloat CameraVelocity = movementspeed*deltaTime;
      if (direction == FORWARD)
      {
        cameraPos += CameraVelocity*cameraFront;
      }
      if (direction == BACKWARD)
      {
        cameraPos -= CameraVelocity*cameraFront;
      }
      if (direction == RIGHT)
      {
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp))*CameraVelocity;
      }
      if(direction == LEFT)
      {
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp))*CameraVelocity;
      }
    }

  private:
    GLfloat movementspeed = 5.0f;
    mat4 view;
    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;
};
