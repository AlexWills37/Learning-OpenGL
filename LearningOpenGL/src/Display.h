#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Display
{
private:
	GLFWwindow* m_Window;
public:
	Display();
	~Display();

	bool WindowShouldClose();

	void EndFrame();
};