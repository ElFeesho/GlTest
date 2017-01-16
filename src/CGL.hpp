#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

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

  template<typename type>
  GLuint createBuffer(GLuint bufferType, std::vector<type> items)
  {
    GLuint buf;   
    glGenBuffers(1, &buf);
    glBindBuffer(bufferType, buf);
    glBufferData(bufferType, sizeof(type)*items.size(), items.data(), GL_STATIC_DRAW);
    return buf;
  }
  
  GLVAO createVAO(GLuint vertexCount) {
    return GLVAO(vertexCount);
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

  std::vector<std::string> splitString(std::string &value, char delimeter)
  {
    std::stringstream strstream(value);
    std::vector<std::string> result;

    while(!strstream.eof())
    {
      std::string comp;
      std::getline(strstream, comp, delimeter);
      result.push_back(comp);
    }

    return result;
  }

  void copyLinesStartingWith(std::vector<std::string> &source, std::vector<std::string> &destination, const std::string &start)
  {
    std::copy_if(source.begin(), source.end(), std::back_inserter(destination), [=](std::string &element) -> bool {
      return element.substr(0, start.length()) == start;
    });
  }

  GLVAO loadWavefrontObj(const std::string &file)
  {
    std::string contents = readFile(file);

    std::vector<std::string> fileLines = splitString(contents, '\n');

    std::vector<std::string> vertexLines;
    std::vector<std::string> texelLines;
    std::vector<std::string> normalLines;
    std::vector<std::string> faceLines;

    copyLinesStartingWith(fileLines, vertexLines, "v ");
    copyLinesStartingWith(fileLines, normalLines, "vn ");
    copyLinesStartingWith(fileLines, texelLines, "vt ");
    copyLinesStartingWith(fileLines, faceLines, "f ");

    std::vector<float> rawVertices;
    std::vector<float> rawNormals;
    std::vector<float> rawTexels;

    for (std::string vertLine : vertexLines)
    {
      std::stringstream vertexComponents(vertLine.substr(2));
      float x;
      float y;
      float z;
      vertexComponents >> x;
      vertexComponents >> y;
      vertexComponents >> z;

      std::cout << "Read vertex " << x << " " << y << " " << z << std::endl;

      rawVertices.push_back(x);
      rawVertices.push_back(y);
      rawVertices.push_back(z);
    }

    for (std::string normalLine : normalLines)
    {
      std::stringstream normalComponents(normalLine.substr(3));
      float x;
      float y;
      float z;
      normalComponents >> x;
      normalComponents >> y;
      normalComponents >> z;

      rawNormals.push_back(x);
      rawNormals.push_back(y);
      rawNormals.push_back(z);
    }

    for (std::string texelLine : texelLines)
    {
      std::stringstream texelComponents(texelLine.substr(3));
      float x;
      float y;
      texelComponents >> x;
      texelComponents >> y;

      rawTexels.push_back(x);
      rawTexels.push_back(y);
    }

    std::vector<float> orderedVertices;
    std::vector<float> orderedNormals;
    std::vector<float> orderedTexels;
    std::vector<GLuint> indices;

    GLuint index = 0;
    for (std::string faceLine : faceLines)
    {
      std::string components = faceLine.substr(2);
      std::vector<std::string> faceComps = splitString(components, ' ');

      assert(faceComps.size() == 3);

      std::vector<std::string> firstFace = splitString(faceComps[0], '/');
      std::vector<std::string> secondFace = splitString(faceComps[1], '/');
      std::vector<std::string> thirdFace = splitString(faceComps[2], '/');

      int vpos = std::stol(firstFace[0])-1;
      int tpos = std::stol(firstFace[1])-1;
      int npos = std::stol(firstFace[2])-1;
      
      orderedVertices.push_back(rawVertices[vpos*3 + 0]);
      orderedVertices.push_back(rawVertices[vpos*3 + 1]);
      orderedVertices.push_back(rawVertices[vpos*3 + 2]);
      
      orderedTexels.push_back(rawTexels[tpos*2 + 0]);
      orderedTexels.push_back(rawTexels[tpos*2 + 1]);

      orderedNormals.push_back(rawNormals[npos*3 + 0]);
      orderedNormals.push_back(rawNormals[npos*3 + 1]);
      orderedNormals.push_back(rawNormals[npos*3 + 2]);

      indices.push_back(index);
      index++;

      vpos = std::stol(secondFace[0])-1;
      tpos = std::stol(secondFace[1])-1;
      npos = std::stol(secondFace[2])-1;
      
      orderedVertices.push_back(rawVertices[vpos*3 + 0]);
      orderedVertices.push_back(rawVertices[vpos*3 + 1]);
      orderedVertices.push_back(rawVertices[vpos*3 + 2]);
      
      orderedTexels.push_back(rawTexels[tpos*2 + 0]);
      orderedTexels.push_back(rawTexels[tpos*2 + 1]);

      orderedNormals.push_back(rawNormals[npos*3 + 0]);
      orderedNormals.push_back(rawNormals[npos*3 + 1]);
      orderedNormals.push_back(rawNormals[npos*3 + 2]);

      indices.push_back(index);
      index++;

      vpos = std::stol(thirdFace[0])-1;
      tpos = std::stol(thirdFace[1])-1;
      npos = std::stol(thirdFace[2])-1;
      
      orderedVertices.push_back(rawVertices[vpos*3 + 0]);
      orderedVertices.push_back(rawVertices[vpos*3 + 1]);
      orderedVertices.push_back(rawVertices[vpos*3 + 2]);
      
      orderedTexels.push_back(rawTexels[tpos*2 + 0]);
      orderedTexels.push_back(rawTexels[tpos*2 + 1]);

      orderedNormals.push_back(rawNormals[npos*3 + 0]);
      orderedNormals.push_back(rawNormals[npos*3 + 1]);
      orderedNormals.push_back(rawNormals[npos*3 + 2]);

      indices.push_back(index);
      index++;
    }


    GLVAO vao(indices.size());

    vao.use([&]{
      createBuffer(GL_ARRAY_BUFFER, orderedVertices);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      createBuffer(GL_ARRAY_BUFFER, orderedNormals);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
      createBuffer(GL_ARRAY_BUFFER, orderedTexels);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
      createBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);
    });

    return vao;
  }
}