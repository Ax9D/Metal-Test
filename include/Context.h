#pragma once
#include <Metal/Metal.hpp>

namespace Context {
    void init(MTL::Device* device);
    MTL::Device* device();
    void deinit();
};