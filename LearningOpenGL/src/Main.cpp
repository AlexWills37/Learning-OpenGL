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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
        -50.0f, -50.0f, 0.0f, 0.0f,   // 0 
         50.0f, -50.0f, 1.0f, 0.0f,  // 1
         50.0f,  50.0f, 1.0f, 1.0f,  // 2
        -50.0f,  50.0f, 0.0f, 1.0f  // 3
    };
    
    // Index buffer to avoid duplicating verticies
    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };   // Now to send this to the GUP, we need a vertex buffer

    // Create vertex buffer
    int numVertices = 4;
    int numFloats = 4;
    VertexBuffer vertBuffer(positions, numVertices * numFloats * sizeof(float));

    // Connect the vertex buffer with a layout
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    // Set up Vertex Array Object
    VertexArrayObject va;
    va.AddBuffer(vertBuffer, layout);

    // Index buffer object (ibo) connects to active VAO
    IndexBuffer indexBuffer(indices, 6);

    // Projection matrix
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    // Camera matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

//    // Model matrix
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0.5f, 0));
//    // Model View Projection matrix
//    glm::mat4 mvp = proj * view * model;

    // Parse the shader
    Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    shader.SetUniform1i("u_Texture", 0);

    // Texture stetup
    Texture texture("res/textures/manatee.jpg");
    texture.Bind();

    Renderer renderer;

    /* Initialize ImGUI */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplOpenGL3_Init("#version 130");

    glm::vec3 translationA(200, 0, 0);
    glm::vec3 translationB(400, 0, 0);


    // Set up animation of color
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!window.WindowShouldClose())
    {
        /* Render here */
        renderer.Clear();

        // ImGui frame setup
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui window
        ImGui::Begin("Debug");

        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 1000.0f);
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 1000.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        // Change uniforms
        // Model matrix

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, indexBuffer, shader);
        }
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, indexBuffer, shader);
        }



        // animation
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.05f)
            increment = 0.05f;
        r += increment;

        // Draw buffer
        // glDrawArrays(GL_TRIANGLES, 0, 6);   // This method does not use an index buffer

        // Render ImGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.EndFrame();
    }

    // Destroy ImGUI things
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}