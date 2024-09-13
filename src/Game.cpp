
#include <RenderPass.h>
#include <Pipeline.h>
#include <Window.h>
#include <Game.h>
#include <iostream>
Game::Game(MTL::Device* device): renderer(device) {
    test_models = Model::load_gltf(device, "data/glTF-Sample-Assets/Models/Avocado/glTF/Avocado.gltf");
}
Game::~Game() {}
void Game::update(MTL::Device* device) {
}   
void Game::render(MTL::Device* device) {
    renderer.begin_frame();
    renderer.render_model(test_models[0]);
    renderer.end_frame();
}