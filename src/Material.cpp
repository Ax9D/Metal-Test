#include <Material.h>


std::shared_ptr<Material> Material::create() {
    std::shared_ptr<Material> material(new Material());

    return material;
}
ShaderMaterial Material::into_shader_material() {
    ShaderMaterial mat = {};

    mat.albedo = Texture::black()->gpu_resource_id();
    mat.metallic = Texture::black()->gpu_resource_id();
    mat.roughness = Texture::white()->gpu_resource_id();
    mat.normal = Texture::black()->gpu_resource_id();

    if(albedo) {
        mat.albedo = albedo->gpu_resource_id();
    }
    if(metallic) {
        mat.metallic = metallic->gpu_resource_id();
    }
    if(roughness) {
        mat.roughness = roughness->gpu_resource_id();
    }
    if(normal) {
        mat.normal = normal->gpu_resource_id();
    }

    return mat;
}