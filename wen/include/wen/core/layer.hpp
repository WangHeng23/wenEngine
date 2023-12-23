#pragma once
#include "wen/events/event.hpp"
#include "wen/core/timeStep.hpp"

namespace wen {
class layer {
public:
    layer(const std::string& name = "layer");
    virtual ~layer() = default;

    // Called when the layer is attached to the layer stack
    virtual void OnAttach() {}

    // Called when the layer is detached from the layer stack
    virtual void OnDetach() {}

    // Called when the layer is updated
    virtual void OnUpdate(timeStep ts) {}

    // Called when the layer is updated
    virtual void OnImGuiRender() {}

    // Called when the layer receives an event
    virtual void OnEvent(event& event) {}

    inline const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};
}  // namespace wen