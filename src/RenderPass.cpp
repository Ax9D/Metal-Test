#include <RenderPass.h>

MTL::RenderPassDescriptor* RenderPassDescription::alloc_mtl_descriptor() const {
    MTL::RenderPassDescriptor* descriptor = MTL::RenderPassDescriptor::alloc()->init();
        
    for(size_t i = 0; i < attachments.len(); i++) {
        auto attachment = descriptor->colorAttachments()->object(i);
        attachment->setLoadAction(attachments[i].load_action);
        attachment->setStoreAction(attachments[i].store_action);
        attachment->setClearColor(attachments[i].clear_color);
        attachment->setTexture(attachments[i].texture);
    }

    return descriptor;
}