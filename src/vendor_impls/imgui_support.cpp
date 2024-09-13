#include <imgui_support.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_metal.h>

#include <Window.h>

void ImGuiSupport::init(MTL::Device* device) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window::raw(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplMetal_Init(device);
}

void ImGuiSupport::pre_render(MTL::RenderPassDescriptor* desc) {
    // (Your code calls glfwPollEvents())
    // ...
    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(desc);
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)
}
void ImGuiSupport::post_render(MTL::CommandBuffer* cmd_buffer, MTL::RenderCommandEncoder* cmd) {
    // Rendering
    // (Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), cmd_buffer, cmd);
    // (Your code calls endEncoding, presentDrawable, commit etc.)
}
void ImGuiSupport::deinit() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}