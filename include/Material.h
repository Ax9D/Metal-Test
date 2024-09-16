#pragma once

#include <Texture.h>


struct ShaderMaterial {
    MTL::ResourceID albedo;
    MTL::ResourceID normal;
    MTL::ResourceID roughness;
    MTL::ResourceID metallic;
};

struct Material {
        static std::shared_ptr<Material> create();
        ShaderMaterial into_shader_material();
        std::shared_ptr<Texture> albedo;
        std::shared_ptr<Texture> normal;
        std::shared_ptr<Texture> roughness;
        std::shared_ptr<Texture> metallic;
};