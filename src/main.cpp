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

#include "ParticleEmitter.h"

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


GLVAO createSkybox()
{
  GLVAO particle{36};

  particle.use([](){
    CGL::createBuffer(GL_ARRAY_BUFFER, std::vector<float>({
       1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,
       1.0f, 1.0f, -1.0f,
       1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, -1.0
    }));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    CGL::createBuffer(GL_ELEMENT_ARRAY_BUFFER, std::vector<GLuint>({
      2-1, 4-1, 1-1,
      8-1, 6-1, 5-1,
      5-1, 2-1, 1-1,
      6-1, 3-1, 2-1,
      3-1, 8-1, 4-1,
      1-1, 8-1, 5-1,
      2-1, 3-1, 4-1,
      8-1, 7-1, 6-1,
      5-1, 6-1, 2-1,
      6-1, 7-1, 3-1,
      3-1, 7-1, 8-1,
      1-1, 4-1, 8-1
    }));

    glEnableVertexAttribArray(0);

  });

  return particle;
}

int main(int argc, char **argv)
{
  GLFWwindow *window = CGL::createWindow(640, 480, "gltest");

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  GLCubemapTexture skyboxTexture = CGL::loadCubemapTexture("space_u.png", "space_d.png", "space_l.png", "space_r.png", "space_f.png", "space_b.png");
  GLTexture shipTexture = CGL::loadTexture("cool_ship.png");
  GLTexture particleTexture = CGL::loadTexture("fire.png");

  GLVAO ship = CGL::loadWavefrontObj("cool_ship.obj");
  GLVAO particle = createParticle();

  Model shipModel(ship);
  shipModel.setPosition(0.0f, 0.0f, 15.0f);

  CGL::error(__FILE__, __LINE__);

  GLShader skyboxShader = CGL::compileShaderProgram("src/skybox_vertex.glsl", "src/skybox_fragment.glsl");
  CGL::error(__FILE__, __LINE__);

  skyboxShader.locateUniforms({"projection", "view", "skybox"});
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

  ParticleEmitter emitter{glm::vec3(0.0f, 1.0f, 0.0f), 0.15f, 100, 20.0f};

  GLVAO skyboxVao = createSkybox();

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
    shipModel.draw(texturedShader, eye, projectionMatrix, viewMatrix, centre);

    emitter.draw(particle, particleShader, particleTexture, projectionMatrix, viewMatrix, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    
    glDepthFunc(GL_LEQUAL);
    skyboxTexture.bind(0);
    skyboxShader.use([&](){
      skyboxVao.use([&](){
        skyboxShader.setUniform("skybox", 0);
        skyboxShader.setUniform("projection", projectionMatrix);
        skyboxShader.setUniform("view", glm::mat4(glm::mat3(viewMatrix)) * glm::scale(glm::mat4(1.0f), glm::vec3(10.f, 10.f, 10.f)));
        glDrawElements(GL_TRIANGLES, skyboxVao.vertexCount(), GL_UNSIGNED_INT, (void*)0);
      });
    });
    glDepthFunc(GL_LESS);

    CGL::error(__FILE__, __LINE__);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  return 0;
}