#pragma once

#include "wen/core/mouseCodes.hpp"
#include "wen/events/event.hpp"
#include <sstream>

namespace wen {
class mouseMovedEvent : public event {
public:
    mouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    float getX() const { return m_MouseX; }

    float getY() const { return m_MouseY; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "mouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(mouseMoved)
    EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)

private:
    float m_MouseX, m_MouseY;
};

class mouseScrolledEvent : public event {
public:
    mouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}

    float getXOffset() const { return m_XOffset; }

    float getYOffset() const { return m_YOffset; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "mouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
        return ss.str();
    }

    EVENT_CLASS_TYPE(mouseScrolled)
    EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput)

private:
    float m_XOffset, m_YOffset;
};

class mouseButtonEvent : public event {
public:
    int getMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(eventCategoryMouse | eventCategoryInput |
                         eventCategoryMouseButton)

protected:
    mouseButtonEvent(int button) : m_Button(button) {}

    mouseCode m_Button;
};

class mouseButtonPressedEvent : public mouseButtonEvent {
public:
    mouseButtonPressedEvent(mouseCode button) : mouseButtonEvent(button) {}

    std::string toString() const override {
        std::stringstream ss;
        ss << "mouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(mouseButtonPress)
};

class mouseButtonReleasedEvent : public mouseButtonEvent {
public:
    mouseButtonReleasedEvent(mouseCode button) : mouseButtonEvent(button) {}

    std::string toString() const override {
        std::stringstream ss;
        ss << "mouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(mouseButtonRelease)
};
}  // namespace wen