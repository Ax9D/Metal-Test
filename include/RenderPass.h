#pragma once
#include "Metal/Metal.hpp"
#include <Util.h>

struct ColorAttachment {
    MTL::LoadAction load_action;
    MTL::StoreAction store_action;
    MTL::ClearColor clear_color;
    MTL::Texture* texture;
    ColorAttachment() {
        load_action = MTL::LoadActionClear;
        store_action = MTL::StoreActionStore;
        clear_color = MTL::ClearColor();
    }
};


template<>
struct std::hash<ColorAttachment> {
    size_t operator()(const ColorAttachment& x) {
        uint64_t state = 0;
        hash_combine(state, x.load_action);
        hash_combine(state, x.store_action);
        hash_combine(state, x.clear_color.red);
        hash_combine(state, x.clear_color.green);
        hash_combine(state, x.clear_color.blue);
        hash_combine(state, x.clear_color.alpha);
        hash_combine(state, x.texture);

        return state;
    }
};


struct RenderPassDescription {
    ArrayVec<ColorAttachment, 4> attachments;
    MTL::RenderPassDescriptor* alloc_mtl_descriptor() const;
};