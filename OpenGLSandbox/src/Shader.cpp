#include <fstream>
#include <iostream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
  : mFilepath(filepath), mRendererID(0)
{
  std::map<std::string, std::string> source = parseShader(filepath);
  mRendererID = createShader(source["vertex"], source["fragment"]);
}

Shader::~Shader() {
  GLCall(glDeleteProgram(mRendererID));
}

std::map<std::string, std::string> Shader::parseShader(const std::string& filepath) {
  std::ifstream in(filepath);

  std::string line;
  std::string type("none");
  std::map<std::string, std::string> programs;

  while(std::getline(in, line)) {
    if(line.find("#shader") != std::string::npos) {
      int spaceIndex = line.find(" ");
      if(spaceIndex != std::string::npos) {
        // TODO: Account for extra whitespace (trim)?
        type = line.substr(spaceIndex + 1);
      }
      continue;
    }
    else if(type != "none") {
      if(programs.count(type)) {
        programs[type] = programs[type].append(line + "\n");
      }
      else {
        programs[type] = line + "\n";
      }
    }
  }

  in.close();

  return programs;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if(result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << std::endl;
    std::cout << "  " << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

void Shader::bind() const {
  GLCall(glUseProgram(mRendererID));
}

void Shader::unbind() const {
  GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value) {
  GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value) {
  GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) {
  if(mUniformLocationCache.find(name) != mUniformLocationCache.end()) {
    return mUniformLocationCache[name];
  }

  GLCall(int location = glGetUniformLocation(mRendererID, name.c_str()));
  if(location == -1) {
    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
  }

  mUniformLocationCache[name] = location;
  return location;
}