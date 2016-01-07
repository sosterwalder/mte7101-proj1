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

// Implementation of a shader factory. For comments of the methods see the
// header file.

#include "main.hpp"
#include "shader_factory.hpp"

namespace bfs = boost::filesystem;

void ShaderFactory::bindShader(const string& name)
{
    shaders[name]->bind();
}

void ShaderFactory::unbindShader()
{
    glUseProgramObjectARB(0);
}

Shader& ShaderFactory::getShader(const string& name)
{
    if (shaders.find(name) == shaders.end()) {
        cout << "*** WARN: Cannot find shader " << name << endl;
    }
    return *shaders[name];
}

void ShaderFactory::loadShaders()
{
    string shaderPath = "data/";
    boost::regex vsFilter("(.*\\.vs$)");
    boost::regex fsFilter("(.*\\.fs$)");

    vector<string> vsFilenames;
    vector<string> fsFilenames;

    if (!bfs::exists(shaderPath)) {
        throw FatalException("Failed to find shaderPath");
    }

    bfs::directory_iterator end_itr;

    // Iterate the shader path and search for vertex and fragment
    // shaders.
    for (bfs::directory_iterator itr(shaderPath); itr != end_itr; ++itr) {
        if (!bfs::is_regular_file(itr->status())) {
            continue;
        }

        if (boost::regex_search(itr->path().string(), vsFilter)) {
            vsFilenames.push_back(itr->path().filename().string());
        }
        else if (boost::regex_search(itr->path().string(), fsFilter)) {
            fsFilenames.push_back(itr->path().filename().string());
        }
    }

    // Make sure that at least the number of each type matches.
    if (vsFilenames.size() != fsFilenames.size()) {
        throw FatalException("Vertex and fragment shader counts do not match!");
    }

    // Sort the filenames ascending.
    sort(vsFilenames.begin(), vsFilenames.end());
    sort(fsFilenames.begin(), fsFilenames.end());

    int shaderCount = (int)vsFilenames.size();

    // Load all found shader files into memory.
    for (int i = 0; i < shaderCount; ++i) {
        string shadername = vsFilenames[i].substr(0, vsFilenames[i].length() - 3);

        string vsPath = shaderPath + vsFilenames[i];
        string fsPath = shaderPath + fsFilenames[i];

        char* vsSource = loadShaderSource(vsPath);
        char* fsSource = loadShaderSource(fsPath);

        if (vsSource != 0 && fsSource != 0) {
            Shader* shader = new Shader();
            shader->init(shadername, vsSource, fsSource);
            shaders[shadername] = shader;
            delete[] vsSource;
            delete[] fsSource;
            cout << "Successfully loaded shader #" << i << ": " << shadername << " from " << vsPath << " and " << fsPath << endl;
        }
        else {
            throw FatalException("Could not load shader");
        }
    }
}

void ShaderFactory::freeShaders()
{
    map<string, Shader*>::iterator itr;
    for (itr = this->shaders.begin(); itr != this->shaders.end(); ++itr) {
        Shader* s = (*itr).second;
        s->release();
        delete s;
        cout << "Freed shader '" << s->getName() << "'" << endl;
    }
    this->shaders.clear();
}

char* ShaderFactory::loadShaderSource(string& filename)
{
    ifstream file(filename.c_str(), ios::in|ios::binary|ios::ate);

    if (file.is_open()) {
        char* data = 0;
        int size = file.tellg();
        data = new char[size + 1];
        file.seekg(0, ios::beg);
        file.read(data, size);
        file.close();
        data[size] = 0;

        return data;
    }
    else {
        throw FatalException("Could not open shader source file!");

        return 0;
    }
}
