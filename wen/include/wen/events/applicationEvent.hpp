#pragma once

#include "wen/events/event.hpp"
#include <sstream>

namespace wen {
class windowResizeEvent : public event {
public:
    windowResizeEvent(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) {}

    uint32_t getWidth() const { return m_Width; }

    uint32_t getHeight() const { return m_Height; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "windowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(windowResize)
    EVENT_CLASS_CATEGORY(eventCategoryApplication)

private:
    uint32_t m_Width, m_Height;
};

class windowCloseEvent : public event {
public:
    windowCloseEvent() = default;

    EVENT_CLASS_TYPE(windowClose)
    EVENT_CLASS_CATEGORY(eventCategoryApplication)
};

class appTickEvent : public event {
public:
    appTickEvent() = default;

    EVENT_CLASS_TYPE(appTick)
    EVENT_CLASS_CATEGORY(eventCategoryApplication)
};

class appUpdateEvent : public event {
public:
    appUpdateEvent() = default;

    EVENT_CLASS_TYPE(appUpdate)
    EVENT_CLASS_CATEGORY(eventCategoryApplication)
};

}  // namespace wen