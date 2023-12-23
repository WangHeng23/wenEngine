#pragma once

#include "wen/events/mouseEvent.hpp"
#include "wen/events/applicationEvent.hpp"
#include "wen/renderer/orthographicCamera.hpp"
#include "wen/core/timeStep.hpp"

namespace wen {
class orthographicCameraController {
public:
    orthographicCameraController(float aspectRatio, bool rotation = false);

    void onUpdate(timeStep ts);
    void onEvent(event& e);

    void onResize(float width, float height);

    orthographicCamera& getCamera() { return m_Camera; }
    const orthographicCamera& getCamera() const { return m_Camera; }

    float getZoomLevel() const { return m_ZoomLevel; }
    void setZoomLevel(float level) { m_ZoomLevel = level; }

private:
    bool onMouseScrolled(mouseScrolledEvent& e);
    bool onWindowResized(windowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    orthographicCamera m_Camera;

    bool m_Rotation;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    float m_CameraRotation = 0.0f;
    float m_CameraTranslationSpeed = 5.0f;
    float m_CameraRotationSpeed = 180.0f;
};
}