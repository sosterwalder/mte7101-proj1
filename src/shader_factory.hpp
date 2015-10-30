#pragma once

#include "shader.hpp"

using namespace std;

class ShaderFactory
{
public:
    static ShaderFactory& instance() { static ShaderFactory sf; return sf; }

    void init() {}
    void bindShader(const string& name);
    void unbindShader();
    Shader& getShader(const string& name);
    void loadShaders();
    void freeShaders();

private:
    ShaderFactory() = default;
    ShaderFactory(const ShaderFactory&) = delete;
    char *loadShaderSource(string& filename);
    map<string, Shader*> shaders;
};
