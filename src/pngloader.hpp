#pragma once

#include <string>

struct Texture {
  ~Texture() {
    delete[] data;
  }
  unsigned char *data;
  unsigned int width;
  unsigned int height;
};

Texture textureData(const std::string &file);