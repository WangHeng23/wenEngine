#include "platform/windows/windowsWindow.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/events/keyEvent.hpp"
#include "wen/events/mouseEvent.hpp"
#include "platform/opengl/openglContext.hpp"
#include "wen/debug/instrumentor.hpp"

namespace wen {
static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char *description) {
    WEN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

window *window::create(const windowProps &props) {
    return new windowsWindow(props);
}

windowsWindow::windowsWindow(const windowProps &props) {
    WEN_PROFILE_FUNCTION();
    init(props);
}

windowsWindow::~windowsWindow() {
    WEN_PROFILE_FUNCTION();
    shutdown();
}

void windowsWindow::init(const windowProps &props) {
    WEN_PROFILE_FUNCTION();
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    WEN_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width,
                  props.height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        WEN_CORE_ASSERT(success, "Could not init glfw!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
        WEN_PROFILE_SCOPE("glfwCreateWindow");
        m_Window = glfwCreateWindow((int)props.width, (int)props.height,
                                m_Data.title.c_str(), nullptr, nullptr);
    }

    s_GLFWWindowCount++;

    m_Context = new openglContext(m_Window);
    m_Context->Init();
    glfwSetWindowUserPointer(m_Window, &m_Data);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        WEN_CORE_INFO("Could not initialize Glad!");
        return;
    }
    setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(
        m_Window, [](GLFWwindow *window, int width, int height) {
            windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            windowResizeEvent event(width, height);
            data.eventCallback(event);
        });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
        windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
        windowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode,
                                    int action, int mods) {
        windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                keyPressedEvent event(key, 0);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                keyReleasedEvent event(key);
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                keyPressedEvent event(key, 1);
                data.eventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow *window, uint32_t keycode) {
        windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
        keyTypedEvent event(keycode);
    });

    glfwSetMouseButtonCallback(
        m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    mouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    mouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

    glfwSetScrollCallback(
        m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
            windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
            mouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);
        });

    glfwSetCursorPosCallback(
        m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            windowData &data = *(windowData *)glfwGetWindowUserPointer(window);
            mouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });
}

void windowsWindow::shutdown() {
    WEN_PROFILE_FUNCTION();
    glfwDestroyWindow(m_Window);
    --s_GLFWWindowCount;
    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void windowsWindow::onUpdate() {
    WEN_PROFILE_FUNCTION();
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void windowsWindow::setVSync(bool enabled) {
    WEN_PROFILE_FUNCTION();
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
    m_Data.vSync = enabled;
}

bool windowsWindow::isVSync() const {
    return m_Data.vSync;
}
}  // namespace wen