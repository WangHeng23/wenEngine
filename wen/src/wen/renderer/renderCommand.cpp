#include "wen/renderer/renderCommand.hpp"
#include "platform/opengl/openglRendererAPI.hpp"

namespace wen {
rendererAPI *renderCommand::s_RendererAPI = new openglRendererAPI;
}