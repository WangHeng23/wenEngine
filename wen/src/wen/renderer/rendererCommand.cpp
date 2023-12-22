#include "wen/renderer/rendererCommand.hpp"
#include "platform/opengl/openglRendererAPI.hpp"

namespace wen {
rendererAPI *rendererCommand::s_RendererAPI = new openglRendererAPI;
}