#include "TestTexture2D.h"
#include "GLErrorManager.h"
#include "imgui/imgui.h"


#include "Renderer.h"

#include "glm/gtc/matrix_transform.hpp"


/*
 * TestTexture2D
 * Demonstration of loading a texture into OpenGL and creating 2 instances of the texture on the screen
 */

namespace test {
	TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))), 
        m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        // Verticies for our model
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
        }; 

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Shader will map texture to pixels using the vertex attribute for texture coordinates
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.vert", "res/shaders/Basic.frag");

        // VAO to hold the vertex attributes and index buffer
        m_VAO = std::make_unique<VertexArrayObject>();

        int numVertices = 4;
        int numFloats = 4;
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, numVertices * numFloats * sizeof(float));

        // Define vertex with 2 attributes that have 2 floats each
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        // Connect the vertices to the VAO
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // Create index buffer to group verticies into triangles
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);


        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Add texture to the shader as a uniform
        m_Texture = std::make_unique<Texture>("res/textures/manatee.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        /* Render instance A */
        {
            // Create MVP matrix transformation with updated translation
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            
            // Add MVP to shader as a uniform
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            // Render the VAO
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        /* Render instance B */
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 1000.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 1000.0f);
	}
}