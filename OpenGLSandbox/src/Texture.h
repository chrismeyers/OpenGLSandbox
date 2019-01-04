#pragma once

#include "Renderer.h"

class Texture {
public:
  Texture(const std::string& path);
  ~Texture();

  void bind(unsigned int slot = 0) const;
  void unbind() const;

  inline int getWidth() const { return mWidth; }
  inline int getHeight() const { return mHeight; }
private:
  unsigned int mRendererID;
  std::string mFilePath;
  unsigned char* mLocalBuffer;
  int mWidth, mHeight, mBPP;
};

