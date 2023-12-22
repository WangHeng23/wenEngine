#include "platform/opengl/openglContext.hpp"
#include "wen/core/base.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace wen {
openglContext::openglContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle) {
    WEN_CORE_ASSERT(windowHandle, "Window handle is null!");
}

void openglContext::Init() {
    glfwMakeContextCurrent(m_windowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    WEN_CORE_ASSERT(status, "Failed to initialize Glad!");

    WEN_CORE_INFO("OpenGL Info:");
    WEN_CORE_INFO("Vendor: {0}", (char*)glGetString(GL_VENDOR));
    WEN_CORE_INFO("Renderer: {0}", (char*)glGetString(GL_RENDERER));
    WEN_CORE_INFO("Version: {0}", (char*)glGetString(GL_VERSION));
}

void openglContext::SwapBuffers() {
    glfwSwapBuffers(m_windowHandle);
}
}  // namespace wen