#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if(!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if(!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if(glewInit() != GLEW_OK) {
    std::cout << "GLEW init failure" << std::endl;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  {
    float positions[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, // 0
       0.5f, -0.5f, 1.0f, 0.0f, // 1
       0.5f,  0.5f, 1.0f, 1.0f, // 2
      -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    //unsigned int vao; // vertex array object
    //GLCall(glGenVertexArrays(1, &vao));
    //GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(-0.75f, 0.75f, -1.8f, 1.8f, -1.0f, 1.0f);

    Shader shader("res/shaders/Logo.shader");
    shader.bind();
    shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    shader.setUniformMat4f("u_MVP", proj);

    Texture texture("res/textures/meyers-logo-green.png");
    texture.bind();
    shader.setUniform1i("u_Texture", 0);

    // Unbind all - bind in loop
    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.clear();

      shader.bind();
      shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

      renderer.draw(va, ib, shader);

      if(r > 1.0f) {
        increment = -0.05f;
      }
      else if(r < 0.0f) {
        increment = 0.05f;
      }

      r += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}