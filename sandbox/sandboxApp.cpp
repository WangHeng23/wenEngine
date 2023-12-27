#include "exampleLayer.hpp"
#include "sandbox2d.hpp"
#include "game01/gameLayer.hpp"

class snadboxApp : public wen::application {
public:
    snadboxApp() { 
        // pushLayer(new exampleLayer()); 
        pushOverlay(new sandbox2d());
        // pushLayer(new gameLayer());
    }

    ~snadboxApp() {}
};

wen::application* wen::CreateApplication() {
    return new snadboxApp();
}

int main(int argc, char** argv) {
    wen::log::Init();

    WEN_INFO("---main startup---");
    WEN_PROFILE_BEGIN_SESSION("Startup", "startup.json");
    auto app = wen::CreateApplication();
    WEN_PROFILE_END_SESSION();

    WEN_INFO("---main before Runtime---");
    WEN_PROFILE_BEGIN_SESSION("Runtime", "runtime.json");
    app->Run();
    WEN_PROFILE_END_SESSION();

    WEN_INFO("---main after Runtime---");
    WEN_PROFILE_BEGIN_SESSION("Startup", "shutdown.json");
    delete app;
    WEN_PROFILE_END_SESSION();

    WEN_INFO("---main exit---");

    return 0;
}