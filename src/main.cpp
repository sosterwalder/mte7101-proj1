#include "main.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    bool done       = false;
    int  returnCode = 0;

    if (!glfwInit()) {
        auto message = "Error initializing GLFW";
        cout << "*** ERROR: " << message << endl;
        return 1;
    }

    auto window     = glfwCreateWindow(1024, 768, "MTE7101 - project 1", nullptr, nullptr);

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

        // TODO: Load shaders

        while (!glfwWindowShouldClose(window) && !done) {
            // TODO: Get shader, bind and unbind

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

    if (window) {
        glfwTerminate();
        cout << "Terminated GLFW.." << endl;
    }

    return returnCode;
}
