#include "Util.h"
#include <Shader.h>

Shader::Shader() {}

std::shared_ptr<Shader> Shader::create(MTL::Device* device, const std::string& path) {
    std::shared_ptr<Shader> shader(new Shader());
    std::string code = read_to_string(path);
    NS::Error* error = nullptr;
    shader->library = device->newLibrary(NS::String::string(code.c_str(), NS::UTF8StringEncoding),nullptr,  &error);
    if(error) {
        ERROR(error->description()->utf8String());
    }
    shader->vertex = shader->library->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding));
    shader->fragment = shader->library->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding));

    assert(shader->vertex);
    assert(shader->fragment);

    return shader;
}

MTL::Function* Shader::vertex_shader() {
    return vertex;
}
MTL::Function* Shader::fragment_shader() {
    return fragment;
}

Shader::~Shader() {
    vertex->release();
    fragment->release();
    library->release();
}