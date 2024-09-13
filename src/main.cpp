#include <iostream>

#include <Window.h>
#include <Game.h>

#define WIDTH 1920
#define HEIGHT 1080

int main() {
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    std::cout << "Created Metal Device" << std::endl;
    Window::create(device, WIDTH, HEIGHT);

    Game g(device);
    while(!Window::should_close()) {
        g.update(device);
        g.render(device);
        
        Window::update();
    }
    Window::destroy();
    device->release();

    return 0;
}