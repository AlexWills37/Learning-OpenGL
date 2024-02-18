#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/*
 * Display.h
 * Display handles the window and OpenGL context through GLFW.
 * 
 * Usage:
 *		Create a Display object to create a window + OpenGL context. The context
 *		must exist before calling OpenGL functions, so this should be the first action.
 * 
 *		Create a while loop that runs until WindowShouldClose() returns false for a loop that keeps the window open.
 *		Call EndFrame() at the end of this loop to swap buffers (display thed frame) and poll for events.
 * 
 *		When this object is destroyed (out of scope), it will call glfwTerminate().
 *		Because this object was created before the OpenGL objects (hopefully), it will be destroyed
 *		last when the program ends.
 * 
 * @author Alex Wills
 * @credit @p9malino267 YouTube comment (https://www.youtube.com/watch?v=bTHqmzjm2UI&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=14)
 * @date 16 Februrary 2024
 */
class Display
{
private:
	GLFWwindow* m_Window;
public:
	Display();
	~Display();

	bool WindowShouldClose();
	void EndFrame();

	inline GLFWwindow* GetWindow() { return m_Window; }
};