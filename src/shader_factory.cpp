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

    if (vsFilenames.size() != fsFilenames.size()) {
        throw FatalException("Vertex and fragment shader counts do not match!");
    }

    sort(vsFilenames.begin(), vsFilenames.end());
    sort(fsFilenames.begin(), fsFilenames.end());

    int shaderCount = (int)vsFilenames.size();

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
