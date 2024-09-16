#pragma once
#include <Metal/Metal.hpp>
#include <string>

class Texture {
    public:
        static std::shared_ptr<Texture> load(MTL::Device* device, const std::string& path, MTL::TextureDescriptor* td);
        static std::shared_ptr<Texture> create(MTL::Device* device, const void* data, MTL::TextureDescriptor* td);
        ~Texture();

        static const std::shared_ptr<Texture> black();
        static const std::shared_ptr<Texture> white();

        uint32_t width();
        uint32_t height();
        MTL::ResourceID gpu_resource_id() const;

    private:
        Texture() = default;
        MTL::Texture* mtl_texture;
};