#include <Command.h>

void  RenderCommands::init(MTL::CommandBuffer* cmd_buffer, const RenderPassDescription& rp_desc, PipelineRegistry& pipe_registry) {
    this->mtl_rp_descriptor = rp_desc.alloc_mtl_descriptor();
    this->cmd_encoder = cmd_buffer->renderCommandEncoder(mtl_rp_descriptor);
    this->rp_desc = rp_desc;
    this->pipeline_registry = &pipe_registry;
}
MTL::RenderPassDescriptor* RenderCommands::raw_renderpass_descriptor() {
    return mtl_rp_descriptor;
}
MTL::RenderCommandEncoder* RenderCommands::raw_encoder() {
    return this->cmd_encoder;
}
void RenderCommands::end() {
    mtl_rp_descriptor->release();
    cmd_encoder->endEncoding();
    cmd_encoder->release();
}
void RenderCommands::set_pipeline() {
    if(is_pipeline_dirty) {
        assert(rp_desc.n_attachments);
        auto pso = pipeline_registry->render_pipeline(shader, pipe_state, rp_desc);
        cmd_encoder->setRenderPipelineState(pso);
        is_pipeline_dirty = false;
    }
}
void RenderCommands::draw_primitive(MTL::PrimitiveType pritimive_ty, uint32_t vertex_start, uint32_t vertex_count) {
    set_pipeline();
    cmd_encoder->drawPrimitives(pritimive_ty, vertex_start, vertex_count);
}
void RenderCommands::draw_primitive_indexed(MTL::PrimitiveType pritimive_ty, const std::shared_ptr<SharedBuffer>& buffer, MTL::IndexType index_type, uint32_t index_count, uint32_t index_offset) {
    set_pipeline();
    cmd_encoder->drawIndexedPrimitives(pritimive_ty, index_count, index_type, buffer->raw(), index_offset);
}
void RenderCommands::set_pipeline_state(const PipelineState& state) {
    is_pipeline_dirty = true;
    this->pipe_state = state;
}
void RenderCommands::set_shader(const std::shared_ptr<Shader>& shader) {
    if(this->shader == shader) {
        return;
    }

    this->shader = shader;
    is_pipeline_dirty = true;
}
void RenderCommands::set_vertex_buffer(const std::shared_ptr<SharedBuffer>& buffer, uint32_t offset, uint32_t index) {
    cmd_encoder->setVertexBuffer(buffer->raw(), offset, index);
}
void RenderCommands::set_vertex_bytes(const void* p_data, size_t nbytes, uint32_t index) {
    cmd_encoder->setVertexBytes(p_data, nbytes, index);
}
void RenderCommands::set_fragment_buffer(const std::shared_ptr<SharedBuffer>& buffer, uint32_t offset, uint32_t index) {
    cmd_encoder->setFragmentBuffer(buffer->raw(), offset, index);
}

CommandBuffer::CommandBuffer(MTL::Device* device) {
    cmd_buffer = nullptr;
    cmd_queue = device->newCommandQueue();
    pipe_registry.init(device);
}
MTL::CommandBuffer* CommandBuffer::raw() {
    return cmd_buffer;
}
void CommandBuffer::new_frame() {
    if(cmd_buffer) {
        cmd_buffer->release();
    }
    cmd_buffer = cmd_queue->commandBuffer();
}
void CommandBuffer::record_renderpass(const RenderPassDescription& desc, std::function<void(RenderCommands&)> record_fn) {
    auto encoder = begin_renderpass(desc);
    record_fn(encoder);
}
RenderCommands CommandBuffer::begin_renderpass(const RenderPassDescription& desc) {
    RenderCommands encoder;
    encoder.init(cmd_buffer, desc, pipe_registry);

    return encoder;
}
void CommandBuffer::end_renderpass(RenderCommands& pass) {
    pass.end();
}
void CommandBuffer::present(MTL::Drawable* drawable) {
    cmd_buffer->presentDrawable(drawable);
}
void CommandBuffer::submit() {
    cmd_buffer->commit();
}
void CommandBuffer::wait_for_gpu() {
    cmd_buffer->waitUntilCompleted();
}
CommandBuffer::~CommandBuffer() {
    cmd_queue->release();
}