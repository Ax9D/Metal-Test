#include <Metal/Metal.hpp>
#include <Texture.h>
#include <stb_image.h>
#include <Util.h>
#include <utility>
Texture::Texture() {}

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
    std::shared_ptr<Texture> texture(new Texture());
    texture->mtl_texture = device->newTexture(td);

    uint32_t width = 0;
    uint32_t height = 0;
    void* data = load_image_data(path.c_str(), &width, &height);

    assert(td->width() == width);
    assert(td->height() == height);
    
    MTL::Region region = MTL::Region::Make2D(0, 0, width, height);
    texture->mtl_texture->replaceRegion(region, 0, data, 4 * width);
    
    free_image_data(data);
    return texture;
}
Texture::~Texture() {
    mtl_texture->release();
}
uint32_t Texture::width() {
    return mtl_texture->width();
}
uint32_t Texture::height() {
    return mtl_texture->height();
}