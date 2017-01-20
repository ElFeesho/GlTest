#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GLShader {
public:
	GLShader(GLuint shaderId);

	void use(std::function<void()> bindingContext);

	void locateUniforms(std::vector<std::string> uniformNames);

	void setUniform(const std::string &uniformName, glm::vec3 value);
	void setUniform(const std::string &uniformName, glm::vec4 value);
	void setUniform(const std::string &uniformName, glm::mat4 value);
	void setUniform(const std::string &uniformName, float x, float y, float z);
	void setUniform(const std::string &uniformName, GLuint value);
private:

	void locateUniform(std::string &uniformName);
	GLuint _shaderId;
	std::map<std::string, GLuint> _uniformLocations;
};