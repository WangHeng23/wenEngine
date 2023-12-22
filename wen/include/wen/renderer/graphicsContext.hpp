#pragma once

namespace wen {
class graphicsContext {
public:
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
};
}