#pragma once

#include <Util.h>
#include <Metal/Metal.hpp>
#include <memory>

class Shader {
    public:
        static std::shared_ptr<Shader> create(MTL::Device* device, const std::string& path);
        ~Shader();

        MTL::Function* vertex_shader();
        MTL::Function* fragment_shader();
    private:
        Shader();
        MTL::Library* library;
        MTL::Function* vertex;
        MTL::Function* fragment;
};