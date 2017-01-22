#include "ParticleEmitter.h"

ParticleEmitter::Particle::Particle(glm::vec3 startPosition, glm::vec3 speed, int tick, int maxTick, float rotationSpeed) : _startPosition{startPosition}, _position{startPosition}, _speed{speed}, _tick{tick}, _maxTick{maxTick}, _rotation{0.0f}, _rotationSpeed{rotationSpeed} {

}

void ParticleEmitter::Particle::tick() {
  _position += _speed;
  _rotation += _rotationSpeed;
  _tick++;
  if (_tick > _maxTick)
  {
    _tick = 0;
    _position = _startPosition;
  }
}

glm::vec3 &ParticleEmitter::Particle::position() {
  return _position;
}

float ParticleEmitter::Particle::rotation() {
  return _rotation;
}

ParticleEmitter::ParticleEmitter(glm::vec3 direction, float spread, int totalParticles, float travelDistance) {
  for(int i = 0; i < totalParticles; i++)
  {
    glm::vec3 dir(direction.x, direction.y, direction.z);
    dir.x += spread;
    dir.y += spread;
    dir.z += spread;

    _particles.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), dir/10.f, i, totalParticles, (rand()*360.f)/360.f * M_PI);
  }
}

void ParticleEmitter::draw(GLVAO &particleVao, GLShader &particleShader, GLTexture &particleTexture, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 origin) {
  particleShader.use([&](){
    particleShader.setUniform("colour", glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
    particleShader.setUniform("textureId", 0);
    particleShader.setUniform("projection", projectionMatrix);
    particleTexture.bind(0);

    // glDisable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // particleVao.use([&](){
    //   std::for_each(_particles.begin(), _particles.end(), [&](Particle &particle) {
    //     particle.tick();
    //     particleShader.setUniform("modelView", viewMatrix * glm::translate(origin, particle.position()) * glm::rotate(glm::mat4(1.0f), particle.rotation(), glm::vec3(0.0f, 0.0f, 1.0f)));
    //     glDrawElements(GL_TRIANGLES, particleVao.vertexCount(), GL_UNSIGNED_INT, (void*)0);
    //   });
    // });
    // glDisable(GL_BLEND);
    // glEnable(GL_DEPTH_TEST);
  });
}
