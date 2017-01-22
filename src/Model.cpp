/*
 * Copyright 2017 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "Model.h"
#include "GLShader.h"

#include <string>

Model::Model(GLVAO vao) : _vao(vao), _vertexCount(vao.vertexCount()), _position{0.0f, 0.0f, 0.0f}, _rotation{0.0f,0.0f,0.0f}, _scale{1.0f} {}

void Model::setPosition(float x, float y, float z) {
  _position.x = x;
  _position.y = y;
  _position.z = z;
}

void Model::setRotation(float rx, float ry, float rz) {
  _rotation.x = rx;
  _rotation.y = ry;
  _rotation.z = rz;
}

void Model::setScale(float scale) {
  _scale = scale;
}

void Model::draw(GLShader &shader, glm::vec3 camPosition, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 lightPos) {

  _matrix = glm::translate(glm::mat4(1.0f), _position);
  _matrix = glm::scale(_matrix, glm::vec3(_scale, _scale, _scale));
  _matrix = glm::rotate(_matrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  _matrix = glm::rotate(_matrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  _matrix = glm::rotate(_matrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

  shader.use([&](){
    _vao.use([&](){
      
      shader.setUniform("boundTexture", 0);
      shader.setUniform("lightPos", lightPos);
      shader.setUniform("camPos", camPosition);
      
      shader.setUniform("projection", projectionMatrix);
      shader.setUniform("view", viewMatrix);
      shader.setUniform("model", _matrix);
      
      glDrawElements(GL_TRIANGLES, _vao.vertexCount(), GL_UNSIGNED_INT, (void*)0);
    });
  });
  
}
