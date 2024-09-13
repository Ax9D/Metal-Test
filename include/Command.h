#pragma once 
#include <Shader.h>
#include <Pipeline.h>
#include <RenderPass.h>
#include <Buffer.h>
#include <Metal/Metal.hpp>
#include <functional>

class RenderCommands {
    public:
        void draw_primitive(MTL::PrimitiveType pritimive_ty, uint32_t vertex_start, uint32_t vertex_count);
        void draw_primitive_indexed(MTL::PrimitiveType pritimive_ty, const std::shared_ptr<SharedBuffer>& buffer, MTL::IndexType index_type, uint32_t index_count, uint32_t index_offset);
        void set_pipeline_state(const PipelineState& state);
        void set_shader(const std::shared_ptr<Shader>& shader);
        void set_vertex_buffer(const std::shared_ptr<SharedBuffer>& buffer, uint32_t offset, uint32_t index);
        void set_vertex_bytes(const void* p_data, size_t nbytes, uint32_t index);
        void set_fragment_buffer(const std::shared_ptr<SharedBuffer>& buffer, uint32_t offset, uint32_t index);
        MTL::RenderCommandEncoder* raw_encoder();
        MTL::RenderPassDescriptor* raw_renderpass_descriptor();
    private:
        void init(MTL::CommandBuffer* cmd_buffer, const RenderPassDescription& rp_desc, PipelineRegistry& pipe_registry);
        void begin(MTL::RenderPassDescriptor* desc);
        void set_pipeline();
        void end();

        bool is_pipeline_dirty = false;
        PipelineState pipe_state = {};
        std::shared_ptr<Shader> shader = nullptr;
        MTL::RenderPassDescriptor* mtl_rp_descriptor = nullptr;
        RenderPassDescription rp_desc = {};

        MTL::RenderCommandEncoder* cmd_encoder;
        PipelineRegistry* pipeline_registry;
        friend class CommandBuffer;
};
class CommandBuffer {
    public:
        CommandBuffer(MTL::Device* device);
        ~CommandBuffer();
        MTL::CommandBuffer* raw();
        void new_frame();
        void record_renderpass(const RenderPassDescription& desc, std::function<void(RenderCommands&)> record_fn);
        RenderCommands begin_renderpass(const RenderPassDescription& desc);
        void end_renderpass(RenderCommands& pass);

        void submit();
        void present(MTL::Drawable* drawable);
        void wait_for_gpu();

    private:
        MTL::CommandQueue* cmd_queue;
        MTL::CommandBuffer* cmd_buffer;
        PipelineRegistry pipe_registry;
};