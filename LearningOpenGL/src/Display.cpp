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
    m_Window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        ASSERT(false);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);    // Synchronize frame updates with V sync

    // How OpenGL handles writing to a pixel that already has a color value
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
}


Display::~Display()
{
    glfwTerminate();
    /* Because this object should be created first, it will be destroyed last.
     * This way, this method is called after the OpenGL objects are destroyed.
     */
}

/*
 * Indicates if the window should close.
 * Returns false until the window is closed.
 */
bool Display::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

/*
 * Calls window functions that occur at the end of a frame.
 *  - Swaps the buffers to display the frame
 *  - Polls for events (like closing the window)
 */
void Display::EndFrame()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);

    /* Poll for and process events */
    glfwPollEvents();
}
