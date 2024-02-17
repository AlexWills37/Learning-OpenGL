#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Macro for error checking
// Wrap OpenGL functions in GLCall() to detect errors from GL Calls
// In modern versions of OpenGL, there is a debug callback function that can do this
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/*
* Open GL tutorial, following along with The Cherno's OpenGL tutorial series.
* Tutorial link: https://youtu.be/W3gAzLwfIP0?si=b7KxNgQduwsQ-sth
* 
* 
* @author Alex Wills
* @date started Feb 15, 2024
* @date edited Feb 16, 2024
*/

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (0x" << std::hex << error << "): " <<
            function << " " << file << ": "<< line << std::endl;
        return false;
    }
    return true;
}


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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Use OpenGL 3.3 with the CORE profile
    //  CORE - multiple VAOs
    //  COMPATABILITY (COMPAT) - single global default VAO
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);    // Synchronize frame updates with V sync

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


    // Before making buffers, lets set up the VAO (Vertex Array Object)
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));


    // Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  // 'Select' the buffer to modify

    // index: This attribute will be index 0
    // size: The attribute has 2 components (x, y)
    // type: It will be defined as floats
    // normalized: We do not want to normalize the values
    // stride: to get from one vertex to the next, this is the size to jump (2 floats)
    // pointer: now that we are in the vertex, we offset 0 bytes to get to the beginning of this attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);   // Enable this attribute
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);    // Add data// Create buffer

    // Index buffer object (ibo)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);  // 'Select' the buffer to modify
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);    // Add data

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
    

    /* Why use Vertex Arrays?

    // Unlink everything!! :(
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    

    // Rebind everything! :) this would happen every frame if we had multiple buffers/objects to render
    GLCall(glUseProgram(shader));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // These 2 lines! the layout might have changed for different draws
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    GLCall(glEnableVertexAttribArray(0));   
    *
    * Those last 2 lines are why we use vertex arrays; it binds the attribute layout to the vertex buffer
    * Also, in the core OpenGL, Vertex arrays are a requirement
    * 
    * With VAO, you don't need to bind the buffer, or redefine the attribute pointer, or enable the attribute pointer
    *    GLCall(glBindVertexArray(vao))
    * 
    * How does it connect?
    * When you bind the buffer (both of them! vertex and index),
    *   and when you create the attribute pointer,
    *   it does those operations to the currently bound VAO
    */




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}