#pragma once
#include <Metal/Metal.hpp>

namespace ImGuiSupport {
    void init(MTL::Device* device); 
    void pre_render(MTL::RenderPassDescriptor* desc);
    void post_render(MTL::CommandBuffer* cmd_buffer, MTL::RenderCommandEncoder* cmd);
    void deinit();
}