#include "pch.hpp"
#include "platform/windows/windowsInput.hpp"
#include "wen/core/application.hpp"

#include <GLFW/glfw3.h>

namespace wen {
input* input::s_Instance = new windowsInput();

bool windowsInput::IsKeyPressedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(application::get().getWindow().getNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool windowsInput::IsMouseButtonPressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(application::get().getWindow().getNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> windowsInput::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(application::get().getWindow().getNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {static_cast<float>(xpos), static_cast<float>(ypos)};
}

float windowsInput::GetMouseXImpl() {
    auto [x, y] = GetMousePositionImpl();
    return x;
}

float windowsInput::GetMouseYImpl() {
    auto [x, y] = GetMousePositionImpl();
    return y;
}
}