#include <QuartzCore/QuartzCore.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include <Window.h>


static GLFWwindow* s_glfw_window;
static CAMetalLayer* s_metal_layer;
static CA::MetalDrawable* s_current_drawable;

void Window::create(MTL::Device* device, uint32_t width, uint32_t height) {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto window = glfwCreateWindow(width, height, "Metal Engine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    s_glfw_window = window;

    int width_ = width;
    int height_ = height;
    glfwGetFramebufferSize(s_glfw_window, &width_, &height_);
    glfwSetFramebufferSizeCallback(s_glfw_window, [](GLFWwindow* window, int width, int height) {
        s_metal_layer.drawableSize = CGSizeMake(width, height);
    });
    
    NSWindow* ns_window = glfwGetCocoaWindow(s_glfw_window);
    s_metal_layer = [CAMetalLayer layer];
    s_metal_layer.device = (__bridge id<MTLDevice>)device;
    s_metal_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    s_metal_layer.drawableSize = CGSizeMake(width_, height_);
    ns_window.contentView.layer = s_metal_layer;
    ns_window.contentView.wantsLayer = YES;

    update();
}
GLFWwindow* Window::raw() {
    return s_glfw_window;
}
void Window::destroy() {
    glfwDestroyWindow(s_glfw_window);
}

CA::MetalDrawable* Window::drawable() {
    return s_current_drawable;
}

uint32_t Window::width() {
    int32_t width, height;
    glfwGetWindowSize(s_glfw_window, &width, &height);
    return width;
}

uint32_t Window::height() {
    int32_t width, height;
    glfwGetWindowSize(s_glfw_window, &width, &height);
    return height;
}
void Window::update() {
    glfwPollEvents();
    s_current_drawable = (__bridge CA::MetalDrawable*)[s_metal_layer nextDrawable];
}
bool Window::should_close() {
    return glfwWindowShouldClose(s_glfw_window);
}