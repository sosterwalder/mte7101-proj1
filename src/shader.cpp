#include "main.hpp"

void Shader::init(string& name, const char* vsSource, const char* fsSource)
{
    this->name = name;
    this->hasBeenUsed = false;
    GLint isCompiled = 0;

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

    this->program = glCreateProgramObjectARB();
    glAttachObjectARB(this->program, this->vertex);
    glAttachObjectARB(this->program, this->fragment);
    glLinkProgramARB(this->program);

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
