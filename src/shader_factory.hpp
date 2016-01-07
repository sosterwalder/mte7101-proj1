// The MIT License (MIT)
// 
// Copyright (c) 2015-2016 Sven Osterwalder
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Header file defining a shader factory.

#pragma once

#include "shader.hpp"

using namespace std;

// Provides an interface for handling GLSL shaders.
//
// Loads all GLSL shaders in a directory called 'data'starting from the
// directory where the application is launched. Each shader must have
// each a .vs-file for the vertex shader part and a .fs-file for the
// fragment shader part. Both parts must have the same basename.
//
// Uses the factory method to handle the shader objects:
//
// "Define an interface for creating an object, but let subclasses
// decide which class to instantiate. The Factory method lets a class
// defer instantiation it uses to subclasses."(Gang Of Four)
//
// See: https://en.wikipedia.org/wiki/Factory_method_pattern
//
// Usage example:
//        ShaderFactory::instance().loadShaders();
//        Shader& shader = ShaderFactory::instance().getShader(
//            "some-shader-name"
//        );
//        shader.bind();
//        ShaderFactory::instance().unbindShader();
//        ...
//        ShaderFactory::instance().freeShaders();
class ShaderFactory
{
public:
    // Returns a static instance of a shader factory.
    static ShaderFactory& instance() { static ShaderFactory sf; return sf; }

    // Initializes the shader factory.
    void init() {}

    // Tries to bind the shader by the given name.
    void bindShader(const string& name);

    // Unbinds all currently bound shaders.
    void unbindShader();

    // Returns a shader object 
    Shader& getShader(const string& name);

    // Loads all shaders in the 'data' subdirectory.
    void loadShaders();

    // Frees all loaded shaders.
    void freeShaders();

private:
    // Set the constructor as private and set it default so it may not
    // be instantiated directly.
    ShaderFactory() = default;
    ShaderFactory(const ShaderFactory&) = delete;

    // Load shader source from given file
    char *loadShaderSource(string& filename);

    // Internal data structure holding loaded, valid shaders.
    map<string, Shader*> shaders;
};
