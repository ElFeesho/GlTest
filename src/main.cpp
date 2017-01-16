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

GLVAO createSquare() {
    GLVAO vao = CGL::createVAO();
    
    vao.use([]() {
      float vertices[] = {
	0.0f, 0.0f,		0.0f,
	1.0f, 0.0f, 		0.0f,
	0.0f, 1.0f, 		0.0f,
	1.0f, 1.0f, 		0.0f
      };
      
      float texels[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
      };

      GLuint indices[] = {
	  0, 1, 2,
	  2, 3, 1
      };
      
      CGL::createBuffer<float, 12>(GL_ARRAY_BUFFER, vertices);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      CGL::createBuffer<float, 8>(GL_ARRAY_BUFFER, texels);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
      
      CGL::createBuffer<GLuint, 6>(GL_ELEMENT_ARRAY_BUFFER, indices);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
    });
    return vao;
}

int main(int argc, char **argv)
{
  GLFWwindow *window = CGL::createWindow(640, 480, "gltest");

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  glEnable(GL_DEPTH_TEST);

  CGL::error(__FILE__, __LINE__);

  int shader = CGL::compileShaderProgram("src/vertex.glsl", "src/fragment.glsl");
  CGL::error(__FILE__, __LINE__);
  
  int texturedShader = CGL::compileShaderProgram("src/vertex.glsl", "src/texture_fragment.glsl");
  CGL::error(__FILE__, __LINE__);

  glm::mat4 projectionMatrix = glm::perspective(20.f, 4.0f/3.0f, 0.1f, 100.0f);
  glm::mat4 viewMatrix;
  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

  glm::vec3 eye(0.0f, 0.5f, 1.0f);
  glm::vec3 centre(0.0f, 0.0f, 0.0f);

  float rotation = 0.0f;

  GLVAO squareVao = createSquare();

  Model player(squareVao, 6);
  Model floor(squareVao, 6);
  glm::mat4 floorModel;

  floorModel = glm::rotate(glm::mat4(1.0f), (float)M_PI/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

  // floorModel = glm::scale(floorModel, glm::vec3(20.0f, 20.0f, 1.0f));
  floorModel = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
  floorModel = glm::scale(floorModel, glm::vec3(20.0f, 20.0f, 1.0f));

  floor.setMatrix(floorModel);

  floor.setColour(0.0f, 1.0f, 0.5f);

  player.setColour(1.0f, 0.5f, 0.0f);

  GLuint textureId = CGL::loadTexture("crate.png");
  
  while(!glfwWindowShouldClose(window))
  {
    if (glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
    {
      rotation -= 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
    {
      rotation += 0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
    {
      eye.x += cos(rotation)*0.1f;
      eye.z += sin(rotation)*0.1f;
    }

    if (glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
    {
      eye.x -= cos(rotation)*0.1f;
      eye.z -= sin(rotation)*0.1f;
    }

    centre.x = eye.x + cos(rotation);
    centre.y = eye.y;
    centre.z = eye.z + sin(rotation);

    viewMatrix = glm::lookAt(eye, centre, glm::vec3(0.0f, 1.0f, 0.0f));

    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    floor.draw(shader, projectionMatrix, viewMatrix);
    glBindTexture(GL_TEXTURE_2D, textureId);
    player.draw(texturedShader, projectionMatrix, viewMatrix);

    CGL::error(__FILE__, __LINE__);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  return 0;
}