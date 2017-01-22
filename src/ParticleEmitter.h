#pragma once

#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLTexture.h"
#include "GLShader.h"
#include "GLVAO.h"

class ParticleEmitter {
  class Particle {
  public:
    Particle(glm::vec3 startPosition, glm::vec3 speed, int tick, int maxTick, float rotationSpeed);

    void tick();

    glm::vec3 &position();

    float rotation();

  private:
    glm::vec3 _startPosition;
    glm::vec3 _position;
    glm::vec3 _speed;

    int _tick;
    int _maxTick;

    float _rotation;
    float _rotationSpeed;
  };

public:
  ParticleEmitter(glm::vec3 direction, float spread, int totalParticles, float travelDistance);

  void draw(GLVAO &particleVao, GLShader &particleShader, GLTexture &particleTexture, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 origin);

private:
  std::vector<Particle> _particles;

  glm::vec3 _direction;
  float _spread;
  float _travelDistance;
};