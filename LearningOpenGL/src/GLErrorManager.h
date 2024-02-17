#pragma once
#include <GL/glew.h>

/*
 * GLErrorManager.h
 * Adds macros for debugging OpenGL.
 * 
 * Usage:
 *      Wrap every OpenGL method in GLCall() to cause errors to print / breakpoint the code.
 *      Wrap a conditional in ASSERT() to create a debug break if something is false.
 * 
 * @credit The Cherno
 * @credit @superpb600 in YouTube comments for abstracting this to its own class (https://www.youtube.com/watch?v=jjaTTRFXRAk&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=17)
 * @date 16 February 2024
 */

// Macro for error checking
// Wrap OpenGL functions in GLCall() to detect errors from GL Calls
// In modern versions of OpenGL, there is a debug callback function that can do this
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);