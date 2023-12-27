#pragma once

#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace wen {
struct profileResult {
    std::string name;
    long long start, end;
    uint32_t threadID;
};

struct instrumentationSession {
    std::string name;
};

class instrumentor {
private:
    instrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;

public:
    instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

    void writeHeader() {
        m_OutputStream << R"({"otherData": {}, "traceEvents": [ )";
        m_OutputStream.flush();
    }

    void writeFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    void beginSession(const std::string& name,
                      const std::string& filepath = "results.json") {
        m_OutputStream.open(filepath);
        writeHeader();
        m_CurrentSession = new instrumentationSession{name};
    }

    void endSession() {
        writeFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void writeProfile(const profileResult& result) {
        if (m_ProfileCount++ > 0) {
            m_OutputStream << ",";
        }

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << R"("cat": "function",)";
        m_OutputStream << R"("dur": )" << (result.end - result.start) << ',';
        m_OutputStream << R"("name": ")" << name << "\",";
        m_OutputStream << R"("ph": "X",)";
        m_OutputStream << R"("pid": 0,)";
        m_OutputStream << R"("tid": )" << result.threadID << ",";
        m_OutputStream << R"("ts": )" << result.start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    static instrumentor& get() {
        static instrumentor instance;
        return instance;
    }
};

class instrumentorTimer {
private:
    const char* m_Name;
    bool m_Stopped;
    std::chrono::time_point<std::chrono::high_resolution_clock>
        m_StartTimePoint;

public:
    instrumentorTimer(const char* name) : m_Name(name), m_Stopped(false) {
        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }

    ~instrumentorTimer() {
        if (!m_Stopped) {
            stop();
        }
    }

    void stop() {
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(
                m_StartTimePoint)
                .time_since_epoch()
                .count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(
                            endTimePoint)
                            .time_since_epoch()
                            .count();

        uint32_t threadID =
            std::hash<std::thread::id>{}(std::this_thread::get_id());
        instrumentor::get().writeProfile({m_Name, start, end, threadID});

        m_Stopped = true;
    }
};
}  // namespace wen

#define WEN_CONCATENATE_IMPL(s1, s2) s1##s2
#define WEN_CONCATENATE(s1, s2) WEN_CONCATENATE_IMPL(s1, s2)
#define WEN_PROFILE 0
#if WEN_PROFILE
    #define WEN_PROFILE_BEGIN_SESSION(name, filepath) ::wen::instrumentor::get().beginSession(name, filepath)
    #define WEN_PROFILE_END_SESSION() ::wen::instrumentor::get().endSession()
    #define WEN_PROFILE_SCOPE(name) ::wen::instrumentorTimer WEN_CONCATENATE(timer, __LINE__)(name)
    #define WEN_PROFILE_FUNCTION() WEN_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define WEN_PROFILE_BEGIN_SESSION(name, filepath)
    #define WEN_PROFILE_END_SESSION()
    #define WEN_PROFILE_SCOPE(name)
    #define WEN_PROFILE_FUNCTION()
#endif