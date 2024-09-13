#include <Buffer.h>
#include <Util.h>

SharedBuffer::SharedBuffer() {}

SharedBuffer::~SharedBuffer() {
    mtl_buffer->release();
}

std::shared_ptr<SharedBuffer> SharedBuffer::create(MTL::Device* device, size_t size) {
    std::shared_ptr<SharedBuffer> buffer(new SharedBuffer());

    buffer->mtl_buffer = device->newBuffer(size, MTL::ResourceStorageModeShared);

    return buffer;
}

MTL::Buffer* SharedBuffer::raw() {
    return mtl_buffer;
}

void* SharedBuffer::data() {
    return mtl_buffer->contents();
}

size_t SharedBuffer::len() {
    return mtl_buffer->length();
}

size_t shader_datatype_size(ShaderDataType ty) {
    switch(ty) {
        case ShaderDataType::Float: 
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::UInt:
            return 4;
        case ShaderDataType::UInt2:
            return 4 * 2;
        default:
            ERROR("Unimplemented");
    }
}
MTL::VertexFormat shader_datatype_to_mtl_vertex_format(ShaderDataType ty) {
    switch(ty) {
        case ShaderDataType::Float: 
            return MTL::VertexFormatFloat;
        case ShaderDataType::Float2:
            return MTL::VertexFormatFloat2;
        case ShaderDataType::Float3:
            return MTL::VertexFormatFloat3;
        case ShaderDataType::Float4:
            return MTL::VertexFormatFloat4;
        case ShaderDataType::UInt:
            return MTL::VertexFormatUInt;
        case ShaderDataType::UInt2:
            return  MTL::VertexFormatUInt2;
        default:
            ERROR("Unimplemented");
    }
}

VertexLayoutBuilder& VertexLayoutBuilder::buffer(uint32_t buffer_ix, std::initializer_list<ShaderDataType> items) {
   ArrayVec<ShaderDataType, 16> layout;

   for(auto& item: items) {
        layout.push(item);
   }

   buffer_layouts[buffer_ix] = layout; 

   return *this;
}

VertexLayoutBuilder& VertexLayoutBuilder::attribute(uint32_t attribute_ix, uint32_t buffer_ix, uint32_t byte_offset, ShaderDataType format) {
    assert(buffer_layouts.find(buffer_ix) != buffer_layouts.end());

    attributes[attribute_ix] = std::make_tuple(buffer_ix, byte_offset, format);

    return *this;
}

size_t buffer_layout_size(const ArrayVec<ShaderDataType, 16>& layout) {
    size_t total_size = 0;
    for(size_t i = 0; i < layout.len(); i++) {
        total_size += shader_datatype_size(layout[i]);
    }

    return total_size;
} 
uint64_t buffer_layout_hash(const ArrayVec<ShaderDataType, 16>& layout) {
    uint64_t hash = 0;
    for(size_t i = 0; i < layout.len(); i++) {
        hash_combine(hash, layout[i]);
    }

    return hash;
} 

VertexLayout VertexLayoutBuilder::build() {
    VertexLayout vertex_layout;
    for(auto [buffer_ix, layout]: buffer_layouts) {
        hash_combine(vertex_layout.hash, buffer_ix);
        hash_combine(vertex_layout.hash, MTL::VertexStepFunctionPerVertex);
        hash_combine(vertex_layout.hash, 1);
        hash_combine(vertex_layout.hash, buffer_layout_hash(layout));

        vertex_layout.buffer_layouts.push(std::make_pair(buffer_ix, layout));
    }

    for(auto [attribute_ix, attribute]: attributes) {
        hash_combine(vertex_layout.hash, attribute_ix);
        auto [buffer_ix, byte_offset, format] = attribute;
        hash_combine(vertex_layout.hash, buffer_ix);
        hash_combine(vertex_layout.hash, byte_offset);
        hash_combine(vertex_layout.hash, format);

        std::cout << "Attrib Index" << attribute_ix << std::endl;
        vertex_layout.attributes.push(VertexAttribute {.attribute_index = attribute_ix, .buffer_index = buffer_ix, .byte_offset = byte_offset, .format = format});
    }
    return vertex_layout;
}
MTL::VertexDescriptor* VertexLayout::allocate_mtl() const {
    MTL::VertexDescriptor* descriptor = MTL::VertexDescriptor::alloc()->init();

    for(size_t i = 0; i < buffer_layouts.len(); i++) {
        const auto& [buffer_index,layout]= buffer_layouts[i];
        auto mtl_layout = descriptor->layouts()->object(buffer_index);
        mtl_layout->setStepFunction(MTL::VertexStepFunctionPerVertex);
        mtl_layout->setStepRate(1);
        auto stride = buffer_layout_size(layout);
        INFO("{}", stride);
        mtl_layout->setStride(stride);
    }

    for(size_t i = 0; i < attributes.len(); i++) {
        const auto& attribute= attributes[i];
        auto mtl_attribute = descriptor->attributes()->object(attribute.attribute_index);
        mtl_attribute->setBufferIndex(attribute.buffer_index);
        mtl_attribute->setOffset(attribute.byte_offset);
        mtl_attribute->setFormat(shader_datatype_to_mtl_vertex_format(attribute.format));
    }

    return descriptor;
}
bool VertexLayout::operator==(const VertexLayout& other) const {
    return this->buffer_layouts == other.buffer_layouts &&
           this->attributes == other.attributes;
}

VertexLayout::VertexLayout() {
    hash = 0;
}
bool VertexAttribute::operator==(const VertexAttribute& other) const {
    return this->format == other.format &&
           this->attribute_index == other.attribute_index &&
           this->buffer_index == other.buffer_index &&
           this->byte_offset == other.byte_offset;
}