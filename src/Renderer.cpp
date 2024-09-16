#include <Renderer.h>
#include <Window.h>
#include <QuartzCore/QuartzCore.hpp>
#include <imgui_support.h>
#include <simd/vector.h>
#include <Material.h>

struct GlobalUBOData {
    simd::float3 color;
};
struct PerMeshUBO {
    simd::float4x4 transform;
    ShaderMaterial material;
};

Renderer::Renderer(MTL::Device* device): cmd_buf(device) {
    triangle_shader = Shader::create(device, std::string("shaders/triangle.metal"));
    global_ubo = SharedBuffer::create(device, sizeof(GlobalUBOData));

    VertexLayoutBuilder builder;

    builder
    .buffer(2, {ShaderDataType::Float3}) // Positions
    .buffer(3, {ShaderDataType::Float3}) // Normals
    .buffer(4, {ShaderDataType::Float2}) // UVs
    
    .attribute(0, 2, 0, ShaderDataType::Float3)
    .attribute(1, 3, 0, ShaderDataType::Float3)
    .attribute(2, 4, 0, ShaderDataType::Float2);

    vertex_layout = builder.build();
    ImGuiSupport::init(device);
}
Renderer::~Renderer() {}

void Renderer::begin_frame() {
    cmd_buf.new_frame();
    CA::MetalDrawable* drawable = Window::drawable();

    ColorAttachment attachment = ColorAttachment();
    attachment.texture = drawable->texture();

    RenderPassDescription desc;
    desc.attachments = {attachment};

    static float t = 0.0;
    auto global_data = (GlobalUBOData*) global_ubo->data();
    global_data->color = simd::make_float3(1.0, simd::sin(t), simd::cos(t));
    t += 0.001;

    render_cmd = cmd_buf.begin_renderpass(desc);
    ImGuiSupport::pre_render(render_cmd.raw_renderpass_descriptor());
    
    auto pipeline_state = PipelineState {
        .primitive_topology = MTL::PrimitiveTopologyClass::PrimitiveTopologyClassTriangle,
        .vertex_layout = this->vertex_layout
    };
    
    render_cmd.set_pipeline_state(pipeline_state);
    render_cmd.set_shader(triangle_shader); 
    render_cmd.set_fragment_buffer(global_ubo, 0, 0);
}

void Renderer::render_model(const std::shared_ptr<Model> &model) {
    for(auto& mesh: model->meshes()) {
            PerMeshUBO model_ubo = {
                .transform = mesh.transform.matrix(),
            };
        render_cmd.set_vertex_bytes(&model_ubo, sizeof(model_ubo), 1);

        render_cmd.set_vertex_buffer(mesh.positions, 0, 2);
        render_cmd.set_vertex_buffer(mesh.normals, 0, 3);
        render_cmd.set_vertex_buffer(mesh.uvs, 0, 4);
        
        render_cmd.draw_primitive_indexed(MTL::PrimitiveTypeTriangle, mesh.indices, MTL::IndexTypeUInt32, mesh.indices->len() / sizeof(uint32_t), 0);
    }
}

void Renderer::end_frame() {
    ImGuiSupport::post_render(cmd_buf.raw(), render_cmd.raw_encoder());
    cmd_buf.end_renderpass(render_cmd);
    cmd_buf.present(Window::drawable());
    cmd_buf.submit();
    cmd_buf.wait_for_gpu();
}