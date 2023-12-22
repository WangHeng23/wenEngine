#include "platform/windows/windowsWindow.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/events/keyEvent.hpp"
#include "wen/events/mouseEvent.hpp"

namespace wen {
static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char *description) {
    WEN_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

window *window::create(const windowProps &props) {
    return new windowsWindow(props);
}

windowsWindow::windowsWindow(const windowProps &props) {
    init(props);
}

windowsWindow::~windowsWindow() {
    shutdown();
}

void windowsWindow::init(const windowProps &props) {
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    WEN_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width,
                  props.height);

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        if (!success) {
            WEN_CORE_ERROR("Could not initialize GLFW!");
            return;
        }
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    m_Window = glfwCreateWindow((int)props.width, (int)props.height,
                                m_Data.title.c_str(), nullptr, nullptr);

    s_GLFWWindowCount++;

    glfwMakeContextCurrent(m_Window);
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
    glfwDestroyWindow(m_Window);
    --s_GLFWWindowCount;
    if (s_GLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void windowsWindow::onUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void windowsWindow::setVSync(bool enabled) {
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