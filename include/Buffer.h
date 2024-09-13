#pragma once
#include <Metal/Metal.hpp>
#include <Util.h>

enum ShaderDataType {
    Float,
    Float2,
    Float3,
    Float4, 
    UInt,
    UInt2
};
size_t shader_datatype_size(ShaderDataType ty);
MTL::VertexFormat shader_datatype_to_mtl_vertex_format(ShaderDataType ty);

class SharedBuffer {
    public:
        static std::shared_ptr<SharedBuffer> create(MTL::Device* device, size_t size);
        ~SharedBuffer();
        void* data();
        size_t len();
        MTL::Buffer* raw();
    private:
        SharedBuffer();
        MTL::Buffer* mtl_buffer;
};

struct VertexAttribute {
    uint32_t buffer_index;
    uint32_t byte_offset;
    uint32_t attribute_index;
    ShaderDataType format;
    bool operator==(const VertexAttribute& other) const;
};

class VertexLayout {
    public:
        VertexLayout();
        MTL::VertexDescriptor* allocate_mtl() const;
        bool operator==(const VertexLayout& other) const;
        friend class VertexLayoutBuilder;
        friend struct std::hash<VertexLayout>;
    private:
        ArrayVec<std::pair<uint32_t, ArrayVec<ShaderDataType, 16>>, 8> buffer_layouts;
        ArrayVec<VertexAttribute, 8> attributes;
        uint64_t hash;
};

template<>
struct std::hash<VertexLayout> {
    size_t operator()(const VertexLayout& x) const {
        return x.hash;
    }
};

class VertexLayoutBuilder {
    public:
        VertexLayoutBuilder& buffer(uint32_t buffer_ix, std::initializer_list<ShaderDataType> list);
        VertexLayoutBuilder& attribute(uint32_t attribute_ix, uint32_t buffer_ix, uint32_t byte_offset, ShaderDataType format);
        VertexLayout build();
    private:
        std::map<uint32_t, ArrayVec<ShaderDataType, 16>> buffer_layouts;
        std::map<uint32_t, std::tuple<uint32_t, uint32_t, ShaderDataType>> attributes;
};

