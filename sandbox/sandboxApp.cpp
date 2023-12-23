#include "exampleLayer.hpp"
#include "sandbox2d.hpp"

class snadboxApp : public wen::application {
public:
    snadboxApp() { 
        // pushLayer(new exampleLayer()); 
        pushOverlay(new sandbox2d());
    }

    ~snadboxApp() {}
};

wen::application* wen::CreateApplication() {
    return new snadboxApp();
}

int main() {
    wen::log::Init();
    auto app = wen::CreateApplication();
    app->Run();
    delete app;
    return 0;
}