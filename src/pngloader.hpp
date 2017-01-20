#pragma once

#include <string>

struct Texture {
  ~Texture() {
    delete[] data;
  }
  unsigned char *data;
  unsigned int width;
  unsigned int height;
  unsigned int format;
};

Texture textureData(const std::string &file);