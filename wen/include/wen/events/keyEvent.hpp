#pragma once

#include "wen/core/keyCodes.hpp"
#include "wen/events/event.hpp"
#include <sstream>

namespace wen {
class keyEvent : public event {
public:
    keyEvent(uint32_t key) : m_Key(key) {}

    uint32_t getKeyCode() const { return m_Key; }

    EVENT_CLASS_CATEGORY(eventCategoryKeyboard | eventCategoryInput)
protected:
    uint32_t m_Key;
};

class keyPressedEvent : public keyEvent {
public:
    keyPressedEvent(uint32_t key, bool isRepeat = false)
        : keyEvent(key), m_IsRepeat(isRepeat) {}

    bool isRepeat() const { return m_IsRepeat; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "keyPressedEvent: " << m_Key << " (" << m_IsRepeat << ")";
        return ss.str();
    }

    EVENT_CLASS_TYPE(keyPressed)
private:
    bool m_IsRepeat;
};

class keyReleasedEvent : public keyEvent {
public:
    keyReleasedEvent(uint32_t key) : keyEvent(key) {}

    std::string toString() const override {
        std::stringstream ss;
        ss << "keyReleasedEvent: " << m_Key;
        return ss.str();
    }

    EVENT_CLASS_TYPE(keyReleased)
};

class keyTypedEvent : public keyEvent {
public:
    keyTypedEvent(uint32_t key) : keyEvent(key) {}

    std::string toString() const override {
        std::stringstream ss;
        ss << "keyTypedEvent: " << m_Key;
        return ss.str();
    }

    EVENT_CLASS_TYPE(keyTyped)
};

}  // namespace wen