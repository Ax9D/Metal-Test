#include "Context.h"
#include <Metal/Metal.hpp>
#include <CoreFoundation/CoreFoundation.h>
#include <Texture.h>
#include <simd/simd.h>
#include <stb_image.h>
#include <Util.h>
#include <utility>

void* load_image_data(const char* path, uint32_t* width, uint32_t* height) {
    int channels = 0;
    int width_ = 0;
    int height_ = 0;
    stbi_uc* img = stbi_load(path, &width_, &height_, &channels, STBI_rgb);
    
    if(img == nullptr) {
        ERROR("Failed to load image: {}", path);
    }

    *width = width_;
    *height = height_;

    return img;
}
void free_image_data(void* data) {
    stbi_image_free(data);
}
std::shared_ptr<Texture> Texture::load(MTL::Device* device, const std::string& path, MTL::TextureDescriptor* td) {
    uint32_t width = 0;
    uint32_t height = 0;
    void* data = load_image_data(path.c_str(), &width, &height);

    assert(td->width() == width);
    assert(td->height() == height);
    
    std::shared_ptr<Texture> texture = Texture::create(device, data, td);
    
    free_image_data(data);
    return texture;
}
std::shared_ptr<Texture> Texture::create(MTL::Device* device, const void* data, MTL::TextureDescriptor* td) {
    std::shared_ptr<Texture> texture(new Texture());
    texture->mtl_texture = device->newTexture(td);
    
    assert(nbytes % 4 == 0);
    MTL::Region region = MTL::Region::Make2D(0, 0, td->width(), td->height());
    texture->mtl_texture->replaceRegion(region, 0, data, 4 * td->width());
    
    return texture;
}
Texture::~Texture() {
    mtl_texture->release();
}

std::shared_ptr<Texture> create_texture_solid_color(MTL::Device* device, simd::float4 color) {
    auto td = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatRGBA8Unorm, 1, 1, false);
    return Texture::create(device, (const void*) &color, td);
}

const std::shared_ptr<Texture> Texture::white() {
    static const std::shared_ptr<Texture> s_WHITE_TEXTURE = create_texture_solid_color(Context::device(), simd::make_float4(1.0));

    return s_WHITE_TEXTURE;
}

const std::shared_ptr<Texture> Texture::black() {
    static const std::shared_ptr<Texture> s_BLACK_TEXTURE = create_texture_solid_color(Context::device(), simd::make_float4(0.0));

    return s_BLACK_TEXTURE;
}

uint32_t Texture::width() {
    return mtl_texture->width();
}
uint32_t Texture::height() {
    return mtl_texture->height();
}

MTL::ResourceID Texture::gpu_resource_id() const {
    return mtl_texture->gpuResourceID();
}