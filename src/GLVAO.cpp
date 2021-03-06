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

#include "GLVAO.h"

GLVAO::GLVAO(GLuint vertexCount) : _vertexCount{vertexCount} {
    glGenVertexArrays(1, &_vao);
}

void GLVAO::use(std::function<void()> action)
{
  glBindVertexArray(_vao);
  action();
  glBindVertexArray(0);
}

GLuint GLVAO::vertexCount() {
	return _vertexCount;
}