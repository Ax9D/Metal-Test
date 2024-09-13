#pragma once
#include <Metal/Metal.hpp>
#include <Pipeline.h>
#include <Command.h>
#include <Model.h>

class Renderer {
    public:
        Renderer(MTL::Device* device);
        ~Renderer();
        void begin_frame();
        void render_model(const std::shared_ptr<Model>& model);
        void end_frame();
    private:
        CommandBuffer cmd_buf;
        VertexLayout vertex_layout;
        RenderCommands render_cmd;
        std::shared_ptr<SharedBuffer> global_ubo;
        std::shared_ptr<Shader> triangle_shader;
};