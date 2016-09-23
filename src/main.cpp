//  main.cpp
//  Turnip
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//Shader
#include "../shaders/shaders.h"
//Camera
#include "../includes/Turnip/camera.hpp"
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
const GLuint WIDTH = 1200, HEIGHT = 1000;

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


    Shader myShader("./shaders/shader.vs", "./shaders/shaders.frag");

    GLfloat vertices1[] = {
     0.5f, -0.5f, 0.0f, // Bottom Right
    -0.5f, -0.5f, 0.0f,  // Bottom Left
     0.0f,  0.5f, -0.25,    // Top
     0.0f,  -0.5f, -1.0f, //back
    };

    GLuint indices[] = {
      0,1,2,
      0,1,3,
      0,3,2,
      1,3,2
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);
    //position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //color
    // glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentTime = glfwGetTime();
        deltaTime = 0.0f;
        lasttime = 0.0f;
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

        glm::mat4 view;
        view = myCamera.getViewMatrix();

        glm::mat4 model;
        model = glm::rotate(model,(GLfloat)glfwGetTime()*40.0f, glm::vec3(1.0f,1.0f,1.0f));

        glm::mat4 projection;
        projection = glm::perspective(40.0f, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);

        glm::mat4 transform = projection*view*model;
        GLint transformLoc = glGetUniformLocation(myShader.Program,"transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,3);
        // glBindVertexArray(0);
        glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1,&VBO);


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
