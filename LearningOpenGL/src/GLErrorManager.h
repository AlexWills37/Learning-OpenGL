#pragma once

#include <GL/glew.h>

// Macro for error checking
// Wrap OpenGL functions in GLCall() to detect errors from GL Calls
// In modern versions of OpenGL, there is a debug callback function that can do this
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);