#pragma once

using namespace std;

class Shader
{
public:
    Shader()  {}
    ~Shader() { cout << "Destructor of shader '" << name << "' called" << endl; }

    void init(string& name, const char* vsSource, const char* fsSource);
    void release();
    void bind();

    const string getName() const { return name; };
    const GLhandleARB getProgram() const { return program; };

private:
    GLint getAttribLocation(const  GLcharARB *name);
    GLint getUniformLocation(const GLcharARB *name);

    bool        hasBeenUsed;
    string      name;
    GLhandleARB vertex;
    GLhandleARB fragment;
    GLhandleARB program;
};
