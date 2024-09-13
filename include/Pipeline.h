#pragma once

#include <unordered_map>

#include <Shader.h>
#include <RenderPass.h>
#include <Buffer.h>

struct PipelineState {
    MTL::PrimitiveTopologyClass primitive_topology;
    VertexLayout vertex_layout;
};

class PipelineRegistry {
    public:
        ~PipelineRegistry();
        void init(MTL::Device* device);
        MTL::RenderPipelineState* render_pipeline(const std::shared_ptr<Shader>& shader, const PipelineState& pipeline_state, const RenderPassDescription& rp_desc);
    private:
        MTL::RenderPipelineState* create_pipeline(const std::shared_ptr<Shader>& shader, const PipelineState& pipeline_state, const RenderPassDescription& rp_desc);
        MTL::Device* device;
        std::unordered_map<VertexLayout, MTL::VertexDescriptor*> vertex_layout_cache;
        std::unordered_map<uint64_t, MTL::RenderPipelineState*> cache;
};