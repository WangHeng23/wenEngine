#include "wen/imgui/imguiLayer.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "wen/core/application.hpp"
#include "wen/debug/instrumentor.hpp"

namespace wen {
imguiLayer::imguiLayer() : layer("imguiLayer") {}

void imguiLayer::OnAttach() {
    WEN_PROFILE_FUNCTION();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(
        (GLFWwindow *)application::get().getWindow().getNativeWindow(), true);

    ImGui_ImplOpenGL3_Init("#version 410");
}

void imguiLayer::OnDetach() {
    WEN_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imguiLayer::begin() {
    WEN_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imguiLayer::end() {
    WEN_PROFILE_FUNCTION();
    ImGuiIO &io = ImGui::GetIO();
    application &app = application::get();
    io.DisplaySize = ImVec2((float)app.getWindow().getWidth(),
                            (float)app.getWindow().getHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

// void imguiLayer::OnImGuiRender() {
//     static bool show = true;
//     ImGui::ShowDemoWindow(&show);
// }
}  // namespace wen