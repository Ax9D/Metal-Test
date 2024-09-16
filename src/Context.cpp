#include <Context.h>

static MTL::Device* s_device;
void Context::init(MTL::Device* device) {
    s_device = device;
} 

MTL::Device* Context::device() {
    assert(s_device);
    return s_device;
}

void Context::deinit() {

}