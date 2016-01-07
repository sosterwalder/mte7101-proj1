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

// Header file defining shader class.

#pragma once

using namespace std;

// Provides an interface for initializing, binding and releasing GLSL
// shaders.
//
// Example:
//      Shader* shader = new Shader();
//      shader->init(shadername, vsSource, fsSource);
//      shader->bind();
//      shader->release();
//      delete shader;
//
class Shader
{
public:
    // Constructor.
    Shader()  {}

    // Destructor.
    ~Shader() { cout << "Destructor of shader '" << name << "' called" << endl; }

    // Initializes a shader using ARB methods.
    // Sets the name of the shader, compiles the vertex
    // and fragment parts from given sources and creates and links
    // finally a program based on compiled vertex and fragment shader
    // parts.
    void init(string& name, const char* vsSource, const char* fsSource);

    // Releases a shader (and therefor the vertex and fragment shader as
    // well as the program).
    void release();

    // Tells the pipeline to use the program of the current shader.
    void bind();

    // Returns the name of this shader.
    const string getName() const { return name; };

    // Returns the ARB handle to the program of this shader.
    const GLhandleARB getProgram() const { return program; };

private:
    // Returns the (memory-) location of the attribute with the provided
    // name.
    GLint getAttribLocation(const  GLcharARB *name);

    // Returns the (memory-) location of the uniform variable with the provided
    // name.
    GLint getUniformLocation(const GLcharARB *name);

    // Indicates whether this shader has been used yet by binding it or
    // not.
    bool        hasBeenUsed;
    // Defines the name of this shader.
    string      name;
    // Holds an ARB handle to the compiled vertex shader source.
    GLhandleARB vertex;
    // Holds an ARB handle to the compiled fragment shader source.
    GLhandleARB fragment;
    // Holds an ARB handle to the linked program of this shader.
    GLhandleARB program;
};
