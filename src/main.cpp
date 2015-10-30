#include "main.hpp"

using namespace std;


static long timeGetTime( void )
{
    struct timeval now;

    gettimeofday(&now, 0);

    return (long)((now.tv_sec*1000) + (now.tv_usec/1000));
}

int main(int argc, char *argv[])
{
    bool  done          = false;
    int   returnCode    = 0;
    float resolution[2] = {1024, 768};
    long  currentTime   = 0.0;

    if (!glfwInit()) {
        auto message = "Error initializing GLFW";
        cout << "*** ERROR: " << message << endl;
        return 1;
    }

    auto window     = glfwCreateWindow(
        resolution[0],
        resolution[1],
        "MTE7101 - project 1",
        nullptr,
        nullptr
    );

    try {
        glfwMakeContextCurrent(window);

        GLenum err = glewInit();
        if (err != GLEW_OK) {
            auto message = "Error initializing OpenGL: ";
            cout << message << glewGetErrorString(err) << endl;
            throw new FatalException(message);
        }

        auto renderer = glGetString(GL_RENDERER);
        auto version  = glGetString(GL_VERSION);
        cout << "Renderer: " << renderer << endl;
        cout << "Supported OpenGL version: " << version << endl;

        ShaderFactory::instance().loadShaders();
        currentTime = timeGetTime();
 
        while (!glfwWindowShouldClose(window) && !done) {
            Shader& shader = ShaderFactory::instance().getShader("sphere_tracer");
            shader.bind();

            GLhandleARB programId = shader.getProgram();

            int vec2Resolution = glGetUniformLocation(programId, "globalResolution");
            glUniform2fv(vec2Resolution, 1, resolution);
            int floatGlobalTime = glGetUniformLocation(programId, "globalTime");
            glUniform1f(floatGlobalTime, 0.001f * (timeGetTime() - currentTime));

            glRects(-1, -1, 1, 1);
            ShaderFactory::instance().unbindShader();

            glfwSwapBuffers(window);
            glfwPollEvents();

            int keyState = glfwGetKey(window, GLFW_KEY_ESCAPE);
            if (keyState == GLFW_PRESS) {
                cout << "Escape pressed.." << endl;
                done = true;
            }
        }

    }
    catch (const exception &e) {
        cout << "*** ERROR: " << e.what() << endl;

        if (window) {
            glfwTerminate();
            cout << "Terminated GLFW.." << endl;
        }

        returnCode = 1;
    }

    ShaderFactory::instance().freeShaders();

    if (window) {
        glfwTerminate();
        cout << "Terminated GLFW.." << endl;
    }

    return returnCode;
}
