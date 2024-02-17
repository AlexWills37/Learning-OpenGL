#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLErrorManager.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Display.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Renderer.h"




int main(void)
{
    Display window;

    /* Initialize GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;




    /* ~~~~~~~~~~ Initialize scene ~~~~~~~~~~ */

    // verticies
    float positions[] = {
        -0.5f, -0.5f,   // 0 
         0.5f, -0.5f,   // 1
         0.5f,  0.5f,   // 2
        -0.5f,  0.5f,   // 3
    };
    
    // Index buffer to avoid duplicating verticies
    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };   // Now to send this to the GUP, we need a vertex buffer

    // Create vertex buffer
    VertexBuffer vertBuffer(positions, 4 * 2 * sizeof(float));

    // Connect the vertex buffer with a layout
    VertexBufferLayout layout;
    layout.Push<float>(2);

    // Set up Vertex Array Object
    VertexArrayObject va;
    va.AddBuffer(vertBuffer, layout);

    // Index buffer object (ibo) connects to active VAO
    IndexBuffer indexBuffer(indices, 6);

    // Parse the shader
    Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


    Renderer renderer;

    // Set up animation of color
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!window.WindowShouldClose())
    {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.6f, 0.9f, 1.0f);

        renderer.Draw(va, indexBuffer, shader);

        // animation
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.05f)
            increment = 0.05f;
        r += increment;

        // Draw buffer
        // glDrawArrays(GL_TRIANGLES, 0, 6);   // This method does not use an index buffer


        window.EndFrame();
    }

    return 0;
}