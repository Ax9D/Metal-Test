#pragma once
#include <Metal/Metal.hpp>
#include <Model.h>
#include <Renderer.h>

class Game {
    public:
        Game(MTL::Device* device);
        ~Game();
        void update(MTL::Device* device);
        void render(MTL::Device* device);
    private:
        float t = 0;
        Renderer renderer;
        std::vector<std::shared_ptr<Model>> test_models;
};