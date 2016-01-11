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

// Represents the main part of the application and is used to tie all
// files together.

#include "main.hpp"

using namespace std;


// Returns the current point in time as long.
static long timeGetTime()
{
    struct timeval now;

    gettimeofday(&now, 0);

    return (long)((now.tv_sec*1000) + (now.tv_usec/1000));
}

static void handleCursorMovement(GLFWwindow* window, double xPosition, double yPosition) 
{
    cout << "Mouse movement: " << xPosition << ":" << yPosition << endl;
}

// Main entry point for the application.
//
// Retrurns 0 upon successful exceution, 1 upon an error.
int main(int argc, char *argv[])
{
    bool  done          = false;
    int   returnCode    = 0;
    float resolution[2] = {1024, 768};
    long  currentTime   = 0.0;
    double xPosition    = 0.0;
    double yPosition    = 0.0;

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

            // Set the variables 'resolution' and 'floatGlobalTime' and
            // pass them to the shader. 'resolution' holds the currently
            // set window resolution as two dimensional vector.
            // 'floatGlobalTime' holds the current point in time as
            // float.
            int vec2Resolution = glGetUniformLocation(programId, "globalResolution");
            glUniform2fv(vec2Resolution, 1, resolution);
            int floatGlobalTime = glGetUniformLocation(programId, "globalTime");
            glUniform1f(floatGlobalTime, 0.001f * (timeGetTime() - currentTime));
            int vec2MousePosition = glGetUniformLocation(programId, "globalMousePosition");
            float mousePosition[2] = {(float)xPosition, (float)yPosition};
            glUniform2fv(vec2MousePosition, 1, mousePosition);

            glRects(-1, -1, 1, 1);
            ShaderFactory::instance().unbindShader();

            glfwSwapBuffers(window);
            glfwPollEvents();

            // glfwSetCursorPosCallback(window, handleCursorMovement);

            // Handle pressed keys
            int keyState = glfwGetKey(window, GLFW_KEY_ESCAPE);
            if (keyState == GLFW_PRESS) {
                cout << "Escape pressed.." << endl;
                done = true;
            }

            // Handle mouse movement
            glfwGetCursorPos(window, &xPosition, &yPosition);
            if (xPosition < 0.0 || yPosition < 0.0) {
                xPosition = 0.0;
                yPosition = 0.0;
            }
            if (xPosition > (double)resolution[0] || yPosition > (double)resolution[1]) {
                xPosition = 0.0;
                yPosition = 0.0;
            }

            cout << "Mouse movement: " << xPosition << ":" << yPosition << endl;
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
