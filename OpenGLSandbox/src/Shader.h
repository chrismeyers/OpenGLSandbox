#pragma once

#include <string>
#include <map>

#include <glm/glm.hpp>

class Shader {
public:
  Shader(const std::string& filepath);
  ~Shader();

  void bind() const;
  void unbind() const;

  // Set uniforms
  void setUniform1i(const std::string& name, int value);
  void setUniform1f(const std::string& name, float value);
  void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
  std::map<std::string, std::string> parseShader(const std::string& filepath);
  unsigned int compileShader(unsigned int type, const std::string& source);
  unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

  int getUniformLocation(const std::string& name);
private:
  std::string mFilepath;
  unsigned int mRendererID;
  std::map<std::string, int> mUniformLocationCache;
};

