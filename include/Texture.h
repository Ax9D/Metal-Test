#pragma once
#include <Metal/Metal.hpp>
#include <string>

class Texture {
    public:
        static std::shared_ptr<Texture> load(MTL::Device* device, const std::string& path, MTL::TextureDescriptor* td);
        ~Texture();

        uint32_t width();
        uint32_t height();
    private:
        Texture();
        MTL::Texture* mtl_texture;
};