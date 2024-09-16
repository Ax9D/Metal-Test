#include <iostream>

#include <Window.h>
#include <Game.h>
#include <Context.h>

#define WIDTH 1920
#define HEIGHT 1080

void checkDeviceFeatureSupport(MTL::Device* device) {
    assert(device->supportsFamily(MTL::GPUFamilyMetal3));
}
int main() {
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    checkDeviceFeatureSupport(device);

    Context::init(device);
    
    std::cout << "Created Metal Device" << std::endl;
    Window::create(device, WIDTH, HEIGHT);

    Game g(device);
    while(!Window::should_close()) {
        g.update(device);
        g.render(device);
        
        Window::update();
    }
    Window::destroy();
    Context::deinit();

    device->release();

    return 0;
}