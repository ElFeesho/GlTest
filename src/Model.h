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

#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLVAO.h"
#include "GLShader.h"

class Model {
public:
	Model(GLVAO vao);

	void setPosition(float x, float y, float z);
	void setRotation(float rx, float ry, float rz);
	void setScale(float scale);

	void draw(GLShader &shader, glm::vec3 camPosition, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 lightPos);

private:
	GLVAO _vao;
	GLuint _vertexCount;
	glm::mat4 _matrix;

	glm::vec3 _position;
	glm::vec3 _rotation;
	float _scale;
};

#endif // MODEL_H
