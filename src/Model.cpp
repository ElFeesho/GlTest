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

Model::Model(GLVAO vao, GLuint vertexCount) : _vao(vao), _vertexCount(vertexCount), _colour(1.0f, 1.0f, 1.0f, 1.0f) {}

void Model::setMatrix(glm::mat4 matrix) {
  _matrix = matrix;
}

void Model::setColour(float r, float g, float b)
{
  _colour.x = r;
  _colour.y = g;
  _colour.z = b;
}

void Model::draw(GLuint shader, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {

  GLuint colour = glGetUniformLocation(shader, "colour");
  GLuint texture = glGetUniformLocation(shader, "boundTexture");
  GLuint projection = glGetUniformLocation(shader, "projection");
  GLuint view = glGetUniformLocation(shader, "view");
  GLuint model = glGetUniformLocation(shader, "model");

  glUseProgram(shader);
  _vao.use([=](){
    
    if (colour != -1){
      glUniform4fv(colour, 1, glm::value_ptr(_colour));
    }
    if (texture != -1){
      glUniform1i(texture, 0);
    }
    
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(_matrix));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
  });
}
