#include "GLErrorManager.h"
#include <iostream>

/*
 * Clears all logged errors
 */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/*
 * Prints an error to the console
 */
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (0x" << std::hex << error << "): " <<
            function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}