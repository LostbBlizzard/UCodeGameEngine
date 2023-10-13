#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../RenderAPINamespace.h"

RenderAPIStart
enum class InputAction
{
	Press,
	Release,
	Repeat,
};
class Input
{
public:
	static void Set_InputCallBacks(GLFWwindow* window);
private:
	static void focused_callback(GLFWwindow* window, int focused);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void FramebufferSize_callback(GLFWwindow* window, int width, int height);
private:

};
RenderAPIEnd

