#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string.h>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <iterator>
#include "CGL.hpp"
#include <math.h>
#include "Model.h"

int main(int argc, char **argv)
{
  GLFWwindow *window = CGL::createWindow(640, 480, "gltest");

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  CGL::error(__FILE__, __LINE__);

  GLShader texturedShader = CGL::compileShaderProgram("src/vertex.glsl", "src/texture_fragment.glsl");
  CGL::error(__FILE__, __LINE__);

  texturedShader.locateUniforms({"boundTexture", "projection", "view", "model", "lightPos"});
  CGL::error(__FILE__, __LINE__);

  glm::mat4 projectionMatrix = glm::perspective(20.f, 4.0f/3.0f, 0.1f, 100.0f);
  glm::mat4 viewMatrix;
  
  glm::vec3 eye(0.0f, 0.5f, 0.0f);
  glm::vec3 centre(0.0f, 0.0f, 0.0f);

  float rotation = M_PI_2;

  GLVAO ship = CGL::loadWavefrontObj("cool_ship.obj");
  Model shipModel(ship);

  GLTexture shipTexture = CGL::loadTexture("cool_ship.png");

  shipModel.setPosition(0.0f, 0.0f, 15.0f);

  while(!glfwWindowShouldClose(window))
  {
    if (glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
    {
      rotation -= 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
    {
      rotation += 0.05f;
    }

    if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
    {
      eye.x += cos(rotation) * 0.1f;
      eye.z += sin(rotation) * 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
    {
      eye.x -= cos(rotation) * 0.1f;
      eye.z -= sin(rotation) * 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE)==GLFW_PRESS)
    {
      eye.y += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_Z)==GLFW_PRESS)
    {
      eye.y -= 0.1f;
    }

    centre.x = eye.x + cos(rotation);
    centre.y = eye.y;
    centre.z = eye.z + sin(rotation);

    viewMatrix = glm::lookAt(eye, centre, glm::vec3(0.0f, 1.0f, 0.0f));

    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shipTexture.bind(0);
    shipModel.draw(texturedShader, projectionMatrix, viewMatrix, centre);

    CGL::error(__FILE__, __LINE__);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  return 0;
}