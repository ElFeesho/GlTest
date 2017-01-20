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

GLVAO createParticle()
{
  GLVAO particle{6};

  particle.use([](){
    CGL::createBuffer(GL_ARRAY_BUFFER, std::vector<float>({
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
    }));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    CGL::createBuffer(GL_ARRAY_BUFFER, std::vector<float>({
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
    }));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    CGL::createBuffer(GL_ELEMENT_ARRAY_BUFFER, std::vector<GLuint>({
      0, 1, 2, 2, 3, 0
    }));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

  });

  return particle;
}

int main(int argc, char **argv)
{
  GLFWwindow *window = CGL::createWindow(640, 480, "gltest");

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  GLTexture shipTexture = CGL::loadTexture("cool_ship.png");
  GLTexture particleTexture = CGL::loadTexture("fire.png");

  GLVAO ship = CGL::loadWavefrontObj("cool_ship.obj");
  GLVAO particle = createParticle();

  Model shipModel(ship);
  shipModel.setPosition(0.0f, 0.0f, 15.0f);

  CGL::error(__FILE__, __LINE__);

  GLShader texturedShader = CGL::compileShaderProgram("src/vertex.glsl", "src/texture_fragment.glsl");
  CGL::error(__FILE__, __LINE__);

  GLShader particleShader = CGL::compileShaderProgram("src/particle_vertex.glsl", "src/particle_fragment.glsl");
  CGL::error(__FILE__, __LINE__);

  texturedShader.locateUniforms({"boundTexture", "projection", "view", "model", "lightPos"});
  CGL::error(__FILE__, __LINE__);

  particleShader.locateUniforms({"colour", "textureId", "projection", "modelView"});

  glm::mat4 projectionMatrix = glm::perspective(20.f, 4.0f/3.0f, 0.1f, 100.0f);
  glm::mat4 viewMatrix;
  
  glm::vec3 eye(0.0f, 0.5f, 0.0f);
  glm::vec3 centre(0.0f, 0.0f, 0.0f);

  float rotation = M_PI_2;


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

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
      particleShader = CGL::compileShaderProgram("src/particle_vertex.glsl", "src/particle_fragment.glsl");
      particleShader.locateUniforms({"colour", "textureId", "projection", "modelView"});
    }

    centre.x = eye.x + cos(rotation);
    centre.y = eye.y;
    centre.z = eye.z + sin(rotation);

    viewMatrix = glm::lookAt(eye, centre, glm::vec3(0.0f, 1.0f, 0.0f));

    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shipTexture.bind(0);
    shipModel.draw(texturedShader, projectionMatrix, viewMatrix, centre);

    particleShader.use([&](){
      particleShader.setUniform("colour", glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
      particleShader.setUniform("textureId", 0);
      particleShader.setUniform("projection", projectionMatrix);
      particleShader.setUniform("modelView", viewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));

      particle.use([&](){
        particleTexture.bind(0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, particle.vertexCount(), GL_UNSIGNED_INT, (void*)0);
        glDisable(GL_BLEND);
      });
    });
    
    CGL::error(__FILE__, __LINE__);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  return 0;
}