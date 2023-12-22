#pragma once

#include "wen/core/base.hpp"
#include <functional>
#include <string>

namespace wen {
enum class eventType {
    none = 0,
    windowClose,
    windowResize,
    windowFocus,
    windowLostFocus,
    windowMoved,
    appTick,
    appUpdate,
    appRender,
    keyPressed,
    keyReleased,
    keyTyped,
    mouseButtonPress,
    mouseButtonRelease,
    mouseMoved,
    mouseScrolled
};

enum eventCategory {
    none = 0,
    eventCategoryApplication = BIT(0),
    eventCategoryInput = BIT(1),
    eventCategoryKeyboard = BIT(2),
    eventCategoryMouse = BIT(3),
    eventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                        \
    static eventType getStaticType() {                \
        return eventType::type;                       \
    }                                                 \
    virtual eventType getEventType() const override { \
        return getStaticType();                       \
    }                                                 \
    virtual const char* getName() const override {    \
        return #type;                                 \
    }

#define EVENT_CLASS_CATEGORY(category)              \
    virtual int getCategoryFlags() const override { \
        return category;                            \
    }

class event {
public:
    virtual ~event() = default;

    bool handled = false;

    virtual eventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual int getCategoryFlags() const = 0;

    virtual std::string toString() const { return getName(); }

    inline bool isInCategory(eventCategory category) {
        return getCategoryFlags() & category;
    }
};

class eventDispatcher {
    template <typename T>
    using eventFn = std::function<bool(T&)>;

public:
    eventDispatcher(event& e) : m_Event(e) {}

    template <typename T>
    bool dispatch(eventFn<T> func) {
        if (m_Event.getEventType() == T::getStaticType()) {
            m_Event.handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }

private:
    event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const event& e) {
    return os << e.toString();
}
}  // namespace wen