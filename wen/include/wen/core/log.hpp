#pragma once

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>

namespace wen {
class log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    }

    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}  // namespace wen

// Core log macros
#define WEN_CORE_TRACE(...) ::wen::log::GetCoreLogger()->trace(__VA_ARGS__)
#define WEN_CORE_INFO(...) ::wen::log::GetCoreLogger()->info(__VA_ARGS__)
#define WEN_CORE_WARN(...) ::wen::log::GetCoreLogger()->warn(__VA_ARGS__)
#define WEN_CORE_ERROR(...) ::wen::log::GetCoreLogger()->error(__VA_ARGS__)
#define WEN_CORE_CRITICAL(...) ::wen::log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define WEN_TRACE(...) ::wen::log::GetClientLogger()->trace(__VA_ARGS__)
#define WEN_INFO(...) ::wen::log::GetClientLogger()->info(__VA_ARGS__)
#define WEN_WARN(...) ::wen::log::GetClientLogger()->warn(__VA_ARGS__)
#define WEN_ERROR(...) ::wen::log::GetClientLogger()->error(__VA_ARGS__)
#define WEN_CRITICAL(...) ::wen::log::GetClientLogger()->critical(__VA_ARGS__)