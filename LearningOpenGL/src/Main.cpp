#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
* Open GL tutorial, following along with The Cherno's OpenGL tutorial series.
* Tutorial link: https://youtu.be/W3gAzLwfIP0?si=b7KxNgQduwsQ-sth
* 
* 
* @author Alex Wills
* @date started Feb 15, 2024
* @date edited Feb 16, 2024
*/

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
    std::stringstream strings[2];
    
    std::ifstream stream(vertexFilepath);
    strings[0] << stream.rdbuf();
    stream.close();

    stream.open(fragFilepath);
    strings[1] << stream.rdbuf();
    stream.close();

    return { strings[0].str(), strings[1].str() };
}

/*
 * Creates an individual shader for a program
 */
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // Create space for the shader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();   // Get char* version of string
    glShaderSource(id, 1, &src, nullptr);   // Create the shader
    glCompileShader(id);

    // Handle errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);

        // Print error message
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : 
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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;

    /* Initialize scene */

    // Triangle verticies
    float positions[6] = {
        -0.5f, -0.5f, 
         0.0f,  0.5f, 
         0.5f, -0.5f
    };

    // Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);  // 'Select' the buffer to modify
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);    // Add data

    // index: This attribute will be index 0
    // size: The attribute has 2 components (x, y)
    // type: It will be defined as floats
    // normalized: We do not want to normalize the values
    // stride: to get from one vertex to the next, this is the size to jump (2 floats)
    // pointer: now that we are in the vertex, we offset 0 bytes to get to the beginning of this attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);   // Enable this attribute

    // Parse the shader
    ShaderProgramSource sources = ParseShader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    

    unsigned int shader = CreateShader(sources.VertexSource, sources.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);   // This method does not use an index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}