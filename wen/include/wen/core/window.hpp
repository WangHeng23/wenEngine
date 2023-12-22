#pragma once

#include "wen/events/event.hpp"

namespace wen {
struct windowProps {
    std::string title;
    uint32_t width;
    uint32_t height;

    windowProps(const std::string &title = "Wen Engine", uint32_t width = 1600,
                uint32_t height = 900)
        : title(title), width(width), height(height) {}
};

class window {
public:
    using eventCallbackFn = std::function<void(event &)>;
    virtual ~window() = default;

    virtual void onUpdate() = 0;

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void setEventCallback(const eventCallbackFn &callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;

    virtual void *getNativeWindow() const = 0;

    static window *create(const windowProps &props = windowProps());
};
}  // namespace wen
