#include <Model.h>
#include <Util.h>

Model::Model() {}

std::shared_ptr<SharedBuffer> load_buffer(MTL::Device* device, const std::string& name, int expected_accessor_type, const tinygltf::Primitive& primitive, const tinygltf::Model& model) {
    const auto& accessor_ix = primitive.attributes.at(name); 
    const tinygltf::Accessor& accessor = model.accessors.at(accessor_ix);
    const tinygltf::BufferView& buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[buffer_view.buffer];
    
    assert(accessor.type == expected_accessor_type);

    const void* data_offset = buffer.data.data() + buffer_view.byteOffset;
    auto gpu_buffer = SharedBuffer::create(device, buffer_view.byteLength);

    memcpy(gpu_buffer->data(), data_offset, gpu_buffer->len());
    
    return gpu_buffer;
}
std::shared_ptr<SharedBuffer> load_indices(MTL::Device* device, const tinygltf::Primitive& primitive, const tinygltf::Model& model) {
    const auto& accessor_ix = primitive.indices;
    const tinygltf::Accessor& accessor = model.accessors.at(accessor_ix);
    const tinygltf::BufferView& buffer_view = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[buffer_view.buffer];

    const void* data_offset = buffer.data.data() + buffer_view.byteOffset;
    size_t component_size = tinygltf::GetComponentSizeInBytes(accessor.componentType);
    size_t n_components = buffer_view.byteLength / component_size;

    auto gpu_buffer = SharedBuffer::create(device, sizeof(uint32_t) * n_components);

    for(size_t i = 0; i < n_components; i++) {
        uint32_t index;
        if(component_size == 4) {
            index = ((uint32_t*) data_offset)[i];
        } else if(component_size == 2) {
            index = ((uint16_t*) data_offset)[i];
        } else {
            ERROR("Unsupported index component type");
        }

        uint32_t* index_out = (uint32_t*) gpu_buffer->data();
        index_out[i] = index;
    }

    return gpu_buffer;
}
Mesh load_mesh(MTL::Device* device, const tinygltf::Model& model, const tinygltf::Mesh& mesh, size_t model_index) {
    Mesh ret;
    for(auto& primitive: mesh.primitives) {
        ret.positions = load_buffer(device, "POSITION", TINYGLTF_TYPE_VEC3, primitive, model);
        ret.normals = load_buffer(device, "NORMAL", TINYGLTF_TYPE_VEC3, primitive, model);
        ret.uvs = load_buffer(device, "TEXCOORD_0",TINYGLTF_TYPE_VEC2, primitive, model);
        ret.indices = load_indices(device, primitive, model);
    }

    for(auto& node: model.nodes) {
        if(node.mesh == model_index) {
            if(!node.translation.empty()) {
                ret.transform.position = simd::make_float3(node.translation[0], node.translation[1], node.translation[2]);
            }
            if(!node.rotation.empty()) {
                ret.transform.rotation = simd::make_float4(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
            }
            if(!node.scale.empty()) { 
                ret.transform.scale = simd::make_float3(node.scale[0], node.scale[1], node.scale[2]);
            }
        }
    }
    
    return ret;
}
std::shared_ptr<Model> Model::load_model(MTL::Device* device, const tinygltf::Mesh& gltf_mesh, const tinygltf::Model& gltf_model, size_t model_index) {
    std::shared_ptr<Model> model(new Model());

    Mesh mesh = load_mesh(device, gltf_model, gltf_mesh, model_index);
    model->m_meshes.push_back(mesh);

    return model;
}
std::vector<std::shared_ptr<Model>> Model::load_gltf(MTL::Device* device, const std::string& path) {

    tinygltf::TinyGLTF loader;
    tinygltf::Model gltf_model;
    std::string error;
    std::string warnings;
    bool success = loader.LoadASCIIFromFile(&gltf_model, &error, &warnings, path);

    if (!warnings.empty()) {
        WARN("{}", warnings);
    }

    if (!error.empty() || !success) {
        ERROR("Failed to load {}: {}", path, error);
    }

    std::vector<std::shared_ptr<Model>> models;
    for(size_t i = 0; i < gltf_model.meshes.size(); i++) {
        auto& gltf_mesh = gltf_model.meshes[i];
        models.push_back(Model::load_model(device, gltf_mesh, gltf_model, i));
    }
    return models;
}

const std::vector<Mesh>& Model::meshes() const {
    return m_meshes;
}
const Transform& Model::transform() const {
    return m_transform;
}