#include "RenderPass.h"
#include <Util.h>
#include <Pipeline.h>
#include <memory>

void PipelineRegistry::init(MTL::Device* device) {
    this->device = device;
}

MTL::RenderPipelineState* PipelineRegistry::create_pipeline(const std::shared_ptr<Shader>& shader, const PipelineState& pipeline_state, const RenderPassDescription& rp_desc) {
    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setInputPrimitiveTopology(pipeline_state.primitive_topology);
    
    MTL::VertexDescriptor* vertex_descriptor;
    if(vertex_layout_cache.find(pipeline_state.vertex_layout) != vertex_layout_cache.end()) {
        vertex_descriptor = vertex_layout_cache[pipeline_state.vertex_layout];
    } else {
       vertex_descriptor = pipeline_state.vertex_layout.allocate_mtl();
        vertex_layout_cache[pipeline_state.vertex_layout] = vertex_descriptor;
    };

    descriptor->setVertexDescriptor(vertex_descriptor);
    descriptor->setVertexFunction(shader->vertex_shader());
    descriptor->setFragmentFunction(shader->fragment_shader());

    for(size_t i = 0; i < rp_desc.attachments.len(); i++) {
        descriptor->colorAttachments()->object(i)->setPixelFormat(rp_desc.attachments[i].texture->pixelFormat());
    }

    NS::Error* error;
    MTL::RenderPipelineState* pso = device->newRenderPipelineState(descriptor, &error);
    if(error != nullptr) {
        ERROR("{}", error->description()->cString(NS::UTF8StringEncoding));
    }
    INFO("Allocated PSO: {}", (void*) pso);
    
    descriptor->release();

    return pso;
}
uint64_t pipeline_hash(const std::shared_ptr<Shader>& shader, const PipelineState& pipeline_state, const RenderPassDescription& rp_desc) {
    uint64_t hash = 0;
    hash_combine(hash, shader.get());
    hash_combine(hash, pipeline_state.primitive_topology);
    hash_combine(hash, pipeline_state.vertex_layout);
    for(int i = 0; i < rp_desc.attachments.len(); i++) {
        hash_combine(hash, rp_desc.attachments[i]);
    }

    return hash;
}
MTL::RenderPipelineState* PipelineRegistry::render_pipeline(const std::shared_ptr<Shader>& shader, const PipelineState& pipeline_state, const RenderPassDescription& rp_desc) {
    uint64_t hash = pipeline_hash(shader, pipeline_state, rp_desc);

    auto it = cache.find(hash);
    if(it == cache.end()) {
        auto new_pso = create_pipeline(shader, pipeline_state, rp_desc);
        cache[hash] = new_pso;
        return new_pso;
    } else {
        return it->second;
    }
}
PipelineRegistry::~PipelineRegistry() {
    for(auto [_, pipeline]: cache) {
        pipeline->release();
    }

    for(auto [_, descriptor]: vertex_layout_cache) {
        descriptor->release();
    }
}
