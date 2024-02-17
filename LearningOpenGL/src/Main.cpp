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

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

/*
 * Reads in the shader files and return the strings with their source code.
 * 
 * @input vertexFilepath - filepath to the vertex shader
 * @input fragFilepath - filepath to the fragment shader
 */
static ShaderProgramSource ParseShader(const std::string& vertexFilepath, const std::string& fragFilepath) 
{
    std::stringstream strings[2];   // stringstream creates a mutable string to my understanding (it acts as a buffer, like cout)
    
    // Copy files into the strings
    std::ifstream stream(vertexFilepath);   
    strings[0] << stream.rdbuf();
    stream.close();

    stream.open(fragFilepath);
    strings[1] << stream.rdbuf();
    stream.close();

    // Return a struct with the two strings
    return { strings[0].str(), strings[1].str() };
}

/*
 * Creates an individual shader for a program
 */
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    
    unsigned int id = glCreateShader(type); // Create space for the shader
    const char* src = source.c_str();   // Get char* version of string
    glShaderSource(id, 1, &src, nullptr);   // Create the shader
    glCompileShader(id);    // Compile shader

    // Handle errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);  // Place compile status in 'result'
    if (result == GL_FALSE) {
        // Get the length of the status
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // Allocate a space for the message and get the actual message
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);

        // Print error message
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" :  // specify which shader is having the issue
                (type == GL_FRAGMENT_SHADER ? "fragment" : "other type of")) 
            << " shader." << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

/*
 * Creates the shader program for OpenGL from source code.
 */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Create space for a shader program
    unsigned int program = glCreateProgram();
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Create the shader program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Clean up shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



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
    // Set up Vertex Array Object
    VertexArrayObject va;

    // Connect the vertex buffer with a layout
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vertBuffer, layout);

    // Index buffer object (ibo) connects to active VAO
    IndexBuffer indexBuffer(indices, 6);




    // Parse the shader
    ShaderProgramSource sources = ParseShader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    unsigned int shader = CreateShader(sources.VertexSource, sources.FragmentSource);
    glUseProgram(shader);
    
    // Now that a shader is bound, we can add uniform variables to send data to shaders
    int location = glGetUniformLocation(shader, "u_Color"); // In modern OpenGL, we can assign the location, but this works too
    ASSERT(location != -1); // happens when spelling is wrong, or when uniform is unused + erased by compilation

    // Set up animation of color
    float r = 0.0f;
    float increment = 0.05f;




    /* Loop until the user closes the window */
    while (!window.WindowShouldClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        va.Bind();
        indexBuffer.Bind();

        GLCall(glUniform4f(location, r, 0.6f, 0.9f, 1.0f));

        // animation
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.05f)
            increment = 0.05f;
        r += increment;

        // Draw buffer
        // glDrawArrays(GL_TRIANGLES, 0, 6);   // This method does not use an index buffer

        // With an index buffer, we are drawing 6 indices. Since the buffer is currently bound, we do not need a pointer to it
        // The vertices are bound to the ARRAY_BUFFER slot
        // The indices are bound to the ELEMENT_ARRAY_BUFFER slot
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


        window.EndFrame();
    }

    return 0;
}