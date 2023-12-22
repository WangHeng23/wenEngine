#pragma once

#include "wen/core/input.hpp"

namespace wen {
class windowsInput : public input {
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};
}  // namespace wen