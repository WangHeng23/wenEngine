#pragma once

#include "wen/renderer/graphicsContext.hpp"

struct GLFWwindow;

namespace wen {
class openglContext : public graphicsContext {
public:
    openglContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* m_windowHandle;
};
}
