#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

#include "pngloader.hpp"
#include "GLVAO.h"

namespace CGL {

  std::string readFile(const std::string &file) {
    std::ifstream reader(file);
    reader >> std::noskipws;
    std::string contents;
    std::copy(
      std::istream_iterator<char>(reader), 
      std::istream_iterator<char>(), 
      std::back_inserter(contents));
    return contents;
  }

  void error(const std::string &name, int line) {
    std::map<GLuint, std::string> errorMap;

    errorMap[GL_INVALID_ENUM] = "GL_INVALID_ENUM";
    errorMap[GL_INVALID_VALUE] = "GL_INVALID_VALUE";
    errorMap[GL_STACK_OVERFLOW] = "GL_STACK_OVERFLOW";
    errorMap[GL_STACK_UNDERFLOW] = "GL_STACK_UNDERFLOW";
    errorMap[GL_OUT_OF_MEMORY] = "GL_OUT_OF_MEMORY";
    
    GLuint error = GL_NO_ERROR;
    while((error = glGetError()) != GL_NO_ERROR) {
      std::cerr << "Error ("<<name <<":" << line << "): (" << std::hex << error << std::dec << ") " << errorMap[error] << std::endl;
      throw std::domain_error("GL error occurred");
    }
  }

  GLFWwindow *createWindow(int width, int height, const std::string &windowTitle)
  {
    std::cout << "GLFW Version " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR<< "." << GLFW_VERSION_REVISION << std::endl;

    if (!glfwInit()) {
	    throw std::domain_error("Could not initialise glfw");
    }

    glfwSetErrorCallback([](int error, const char* description) -> void {
      std::cerr << "Error: " << error << " : " << description << std::endl;
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr /*glfwGetPrimaryMonitor()*/, nullptr);	

    glfwMakeContextCurrent(window);
    error(__FILE__, __LINE__);
    glewExperimental = GL_TRUE; 
    glewInit();
    glGetError();// Drain INVALID_ENUM error glew makes

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    std::cout << "W: " << w << " H: " << h << std::endl;
    glViewport(0.0f, 0.0f, w, h);

    return window;
  }

  GLuint compileShader(GLuint shaderType, const std::string &programSource)
  {
  GLuint shaderId = glCreateShader(shaderType);
  const char *c_str = programSource.c_str();
  glShaderSource(shaderId, 1, &c_str, nullptr);
  glCompileShader(shaderId);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
    std::cout << "Shader Compilation Error:\n" << infoLog << std::endl;
  }

  return shaderId;
  }

  GLuint linkShaderProgram(GLuint vertexId, GLuint fragmentId)
  {
    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, fragmentId);
    glAttachShader(shaderProgram, vertexId);

    glBindAttribLocation(shaderProgram, 0, "position");

    glLinkProgram(shaderProgram);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
	glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
	    std::cout << "Program link error:\n" << infoLog << std::endl;
    }
    error(__FILE__, __LINE__);
    return shaderProgram;
  }

  GLuint compileShaderProgram(const std::string &vertex, const std::string &fragment) {
    std::string vertexProgram = readFile(vertex);
    std::string fragmentProgram = readFile(fragment);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexProgram);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentProgram);;

    return linkShaderProgram(vertexShader, fragmentShader);
  }

  template<typename type, unsigned int count>
  GLuint createBuffer(GLuint bufferType, type elements[])
  {
    GLuint buf;		
    glGenBuffers(1, &buf);
    glBindBuffer(bufferType, buf);
    glBufferData(bufferType, sizeof(type)*count, elements, GL_STATIC_DRAW);
    return buf;
  }
  
  GLVAO createVAO() {
    return GLVAO();
  }

  GLuint loadTexture(const std::string &pngFile){
    Texture texture = textureData(pngFile);
    
    std::cout << "Loaded texture " << texture.width << " x " << texture.height << std::endl;
    
    GLuint textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    error(__FILE__, __LINE__);
    return textureId;
  }
}