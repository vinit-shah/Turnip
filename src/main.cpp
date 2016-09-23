//  main.cpp
//  Turnip
#include <iostream>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//Shader
#include "../includes/shaders/shaders.h"
//Camera
#include "../includes/Turnip/camera.hpp"

#include "../includes/Turnip/mesh.hpp"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovements();

GLfloat deltaTime = 0.0f;
GLfloat lasttime = 0.0f;
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

bool keys[1024];
Camera myCamera(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,1.0f,1.0f));

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    Shader myShader("./includes/shaders/shader.vs", "./includes/shaders/shaders.frag");

    std::vector<GLfloat> vertices1 = {
     0.5f, -0.5f, 0.5f, // Bottom Right //0
    -0.5f, -0.5f, 0.5f,  // Bottom Left // 1
     0.5f, 0.5f, 0.5f, //top right 2
     -0.5f, 0.5f, 0.5f, //top left 3
     0.5f, -0.5f, -0.5f, //4
     -0.5f, -0.5f, -0.5f, //5
     0.5f, 0.5f, -0.5f, //6
     -0.5f, 0.5f, -0.5f //7
    };

    std::vector<GLuint> indices = {
      0,1,2,
      2,3,1,
      4,0,6,
      6,2,0,
      5,6,4,
      6,7,5,
      1,7,5,
      3,7,1,
      2,3,6,
      6,7,3,
      0,1,4,
      4,5,1
    };
    Mesh myMesh(vertices1, indices);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentTime = glfwGetTime();
        deltaTime= currentTime - lasttime;
        lasttime = currentTime;
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        doMovements();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw triangle
        myShader.Use();

        //matrix transformations s
        glm::mat4 view;
        view = myCamera.getViewMatrix();
        glm::mat4 model;
        model = glm::rotate(model,(GLfloat)glfwGetTime()*40.0f, glm::vec3(1.0f,1.0f,1.0f));
        glm::mat4 projection;
        projection = glm::perspective(40.0f, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        glm::mat4 transform = projection*view*model;
        GLint transformLoc = glGetUniformLocation(myShader.Program,"transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        myMesh.Draw();
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    myMesh.Delete();


    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key>=0 && key < 1024)
    {
      if (action == GLFW_PRESS)
        keys[key] = true;
      else if (action == GLFW_RELEASE)
        keys[key] = false;
    }
}

void doMovements()
{
  if (keys[GLFW_KEY_W])
  {
    myCamera.ProcessKeys(FORWARD, deltaTime);
  }
  if (keys[GLFW_KEY_S])
  {
    myCamera.ProcessKeys(BACKWARD, deltaTime);
  }
  if(keys[GLFW_KEY_D])
  {
    myCamera.ProcessKeys(RIGHT, deltaTime);
  }
  if(keys[GLFW_KEY_A])
  {
    myCamera.ProcessKeys(LEFT, deltaTime);
  }
}
