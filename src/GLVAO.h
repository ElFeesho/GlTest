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

#ifndef GLVAO_H
#define GLVAO_H

#include <GL/glew.h>

#include <functional>

class GLVAO
{
public:
  GLVAO(GLuint vertexCount);
  void use(std::function<void()> action);

  GLuint vertexCount();
private:
  GLuint _vertexCount;
  GLuint _vao;
};

#endif // GLVAO_H
