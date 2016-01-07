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

// Implementation of a shader. For comments of the methods see the
// header file.

#include "main.hpp"

void Shader::init(string& name, const char* vsSource, const char* fsSource)
{
    this->name = name;
    this->hasBeenUsed = false;
    GLint isCompiled = 0;

    // Try to create and compile an ARB shader object for the vertex
    // shader part.
    this->vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    glShaderSourceARB(this->vertex, 1, &vsSource, NULL);
    glCompileShaderARB(this->vertex);
    glGetShaderiv(this->vertex, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        char log[2048];
        int len;
        glGetShaderInfoLog(this->vertex, 2048, (GLsizei*)&len, log);
        glDeleteShader(this->vertex);
        ostringstream buffer;
        buffer << "There was an error compiling the vertex shader '";
        buffer << name << "': " << endl << log << endl;
        cout << buffer.str() << endl;
        throw FatalException(buffer.str());
    }

    // Try to create and compile an ARB shader object for the fragment
    // shader part.
    this->fragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(this->fragment, 1, &fsSource, NULL);
    glCompileShaderARB(this->fragment);
    glGetShaderiv(this->fragment, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        char log[2048];
        int len;
        glGetShaderInfoLog(this->fragment, 2048, (GLsizei*)&len, log);
        glDeleteShader(this->fragment);
        ostringstream buffer;
        buffer << "There was an error compiling the fragment shader '";
        buffer << name << "': " << endl << log << endl;
        cout << buffer.str() << endl;
        throw FatalException(buffer.str());
    }

    // Try to create and link an ARB program object for the created
    // vertex and fragment shader objects.
    this->program = glCreateProgramObjectARB();
    glAttachObjectARB(this->program, this->vertex);
    glAttachObjectARB(this->program, this->fragment);
    glLinkProgramARB(this->program);

    // Check if the status of the created and linked ARB program is ok.
    // If not, throw a fatal exception.
    GLint pstatus;
    glGetProgramiv(this->program, GL_LINK_STATUS, &pstatus);
    if (pstatus != GL_TRUE) {
        char log[2048];
        int len;
        glGetProgramInfoLog(this->program, 2048, (GLsizei*)&len, log);
        glDeleteProgram(this->program);
        ostringstream buffer;
        buffer << "There was an error linking the shader '";
        buffer << name << "': " << endl << log << endl;
        cout << buffer.str() << endl;
        throw FatalException(buffer.str());
    }

    cout << "Successfully initialized shader '" << name << "'" << endl;
}

void Shader::release()
{
    glDeleteShader(this->vertex);
    glDeleteShader(this->fragment);
    glDeleteProgram(this->program);
    cout << "Successfully released shader '" << name << "'" << endl;
}

void Shader::bind()
{
    this->hasBeenUsed = true;
    glUseProgramObjectARB(this->program);
}

GLint Shader::getAttribLocation(const GLcharARB *name)
{
    GLint location;
    location = glGetAttribLocationARB(program, name);
    if (location == -1) {
        cout << "Cannot find attrib variable " << name << " in shader " << this->name << endl;
    } 
    return location;
}

GLint Shader::getUniformLocation(const GLcharARB *name)
{
    GLint location;
    location = glGetUniformLocationARB(program, name);
    if (location == -1) {
        cout << "Cannot find uniform variable " << name << " in shader " << this->name << endl;
    } 
    return location;
}
