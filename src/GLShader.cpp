#include "GLShader.h"
#include <algorithm>

GLShader::GLShader(GLuint shaderId) : _shaderId{shaderId} {

}

void GLShader::use(std::function<void()> bindingContext) {
	glUseProgram(_shaderId);
	bindingContext();
	glUseProgram(0);
}

void GLShader::locateUniforms(std::vector<std::string> uniformNames)
{
	std::for_each(uniformNames.begin(), uniformNames.end(), [&](std::string &uniform) {
		locateUniform(uniform);
	});
}

void GLShader::locateUniform(std::string &uniformName) {
	_uniformLocations[uniformName] = glGetUniformLocation(_shaderId, uniformName.c_str());
}

void GLShader::setUniform(const std::string &uniformName, glm::vec3 value){
	glUniform3fv(_uniformLocations[uniformName], 1, glm::value_ptr(value));
}

void GLShader::setUniform(const std::string &uniformName, glm::vec4 value){
	glUniform4fv(_uniformLocations[uniformName], 1, glm::value_ptr(value));
}

void GLShader::setUniform(const std::string &uniformName, glm::mat4 value){
	glUniformMatrix4fv(_uniformLocations[uniformName], 1, GL_FALSE, glm::value_ptr(value));
}

void GLShader::setUniform(const std::string &uniformName, float x, float y, float z){
	glUniform3f(_uniformLocations[uniformName], x, y, z);
}

void GLShader::setUniform(const std::string &uniformName, GLuint value){
	glUniform1i(_uniformLocations[uniformName], value);
}
