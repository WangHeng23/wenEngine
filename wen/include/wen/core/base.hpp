#pragma once

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define WEN_CORE_ASSERT(x, ...)                                   \
    {                                                             \
        if (!(x)) {                                               \
            WEN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __debugbreak();                                       \
        }                                                         \
    }