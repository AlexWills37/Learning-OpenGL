#include "Display.h"
#include "GLErrorManager.h"

/*
 * Creates the m_Window and OpenGL context.
 */
Display::Display()
{
    /* Initialize the library */
    if (!glfwInit()) {
        ASSERT(false);
    }

    // Use OpenGL 3.3 with the CORE profile
    //  CORE - multiple VAOs
    //  COMPATABILITY (COMPAT) - single global default VAO
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode m_Window and its OpenGL context */
    m_Window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        ASSERT(false);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);    // Synchronize frame updates with V sync
}

Display::~Display()
{
    glfwTerminate();
}

bool Display::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Display::EndFrame()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);

    /* Poll for and process events */
    glfwPollEvents();
}
