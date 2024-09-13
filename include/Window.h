#pragma once
#include <memory>

#include <Util.h>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.hpp>
#include <GLFW/glfw3.h>

namespace Window {
    void create(MTL::Device* device, uint32_t width, uint32_t height);
    GLFWwindow* raw();
    uint32_t width();
    uint32_t height();
    CA::MetalDrawable* drawable();
    bool should_close();
    void update();
    void destroy();
}