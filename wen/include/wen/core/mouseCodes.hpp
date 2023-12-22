#pragma once
#include <cstdint>

namespace wen {
using mouseCode = uint16_t;

namespace mouse {
enum : mouseCode {
    // From glfw3.h
    button0 = 0,
    button1 = 1,
    button2 = 2,
    button3 = 3,
    button4 = 4,
    button5 = 5,
    button6 = 6,
    button7 = 7,

    buttonLast = button7,
    buttonLeft = button0,
    buttonRight = button1,
    buttonMiddle = button2
};
}  // namespace mouse
}  // namespace wen