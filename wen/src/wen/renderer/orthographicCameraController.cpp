#include "pch.hpp"

#include "wen/core/input.hpp"
#include "wen/core/keyCodes.hpp"
#include "wen/renderer/orthographicCameraController.hpp"
#include "wen/debug/instrumentor.hpp"

namespace wen {
orthographicCameraController::orthographicCameraController(float aspectRatio,
                                                           bool rotation)
    : m_AspectRatio(aspectRatio),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
               -m_ZoomLevel, m_ZoomLevel),
      m_Rotation(rotation) {}

void orthographicCameraController::onUpdate(timeStep ts) {
    WEN_PROFILE_FUNCTION();
    if (input::IsKeyPressed(WEN_KEY_A)) {
        m_CameraPosition.x -=
            cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -=
            sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    } else if (input::IsKeyPressed(WEN_KEY_D)) {
        m_CameraPosition.x +=
            cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y +=
            sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (input::IsKeyPressed(WEN_KEY_W)) {
        m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) *
                              m_CameraTranslationSpeed * ts;
        m_CameraPosition.y +=
            cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    } else if (input::IsKeyPressed(WEN_KEY_S)) {
        m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) *
                              m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -=
            cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (m_Rotation) {
        if (input::IsKeyPressed(WEN_KEY_Q)) {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        } else if (input::IsKeyPressed(WEN_KEY_E)) {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }

        if (m_CameraRotation > 180.0f) {
            m_CameraRotation -= 360.0f;
        } else if (m_CameraRotation <= -180.0f) {
            m_CameraRotation += 360.0f;
        }

        m_Camera.setRotation(m_CameraRotation);
    }

    m_Camera.setPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void orthographicCameraController::onEvent(event& e) {
    WEN_PROFILE_FUNCTION();
    eventDispatcher dispatcher(e);
    dispatcher.dispatch<mouseScrolledEvent>(
        BIND_EVENT_FN(orthographicCameraController::onMouseScrolled));
    dispatcher.dispatch<windowResizeEvent>(
        BIND_EVENT_FN(orthographicCameraController::onWindowResized));
}

void orthographicCameraController::onResize(float width, float height) {
    WEN_PROFILE_FUNCTION();
    m_AspectRatio = width / height;
    m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel,
                           m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                           m_ZoomLevel);
}

bool orthographicCameraController::onMouseScrolled(mouseScrolledEvent& e) {
    WEN_PROFILE_FUNCTION();
    m_ZoomLevel -= e.getYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel,
                           m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                           m_ZoomLevel);
    return false;
}

bool orthographicCameraController::onWindowResized(windowResizeEvent& e) {
    WEN_PROFILE_FUNCTION();
    onResize((float)e.getWidth(), (float)e.getHeight());
    return false;
}
}  // namespace wen
