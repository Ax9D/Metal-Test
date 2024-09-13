#pragma once
#include <memory>
#include <Texture.h>
#include <Buffer.h>
#include <simd/simd.h>
#include <AAPLMathUtilities.h>
#include <tiny_gltf.h>

struct Transform {
    simd::float3 position;
    simd::float3 scale;
    quaternion_float rotation;

    Transform() {
       position = simd::make_float3(0.0, 0.0, 0.0); 
       scale = simd::make_float3(1.0, 1.0, 1.0);
       rotation = quaternion_identity();
    }
    simd::float4x4 matrix() const {
        simd::float4x4 matrix =  simd_mul(
            simd_mul(matrix4x4_translation(position), matrix4x4_scale(scale))
            ,  matrix4x4_from_quaternion(rotation));

        return matrix;
    }
};

struct Mesh {
    std::shared_ptr<SharedBuffer> positions;
    std::shared_ptr<SharedBuffer> uvs;
    std::shared_ptr<SharedBuffer> normals;
    std::shared_ptr<SharedBuffer> indices;
    Transform transform;
};

class Model {
    public:
        static std::vector<std::shared_ptr<Model>> load_gltf(MTL::Device* device, const std::string& path);
        const std::vector<Mesh>& meshes() const;
        const Transform& transform() const;
    private:
        static std::shared_ptr<Model> load_model(MTL::Device* device, const tinygltf::Mesh& gltf_mesh, const tinygltf::Model& gltf_model, size_t model_ix);
        Model();
        Transform m_transform;
        std::vector<Mesh> m_meshes;
};