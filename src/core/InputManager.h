#pragma once
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "Camera.h"

class InputManager
{
public:
	static void Init(GLFWwindow* window);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void Update(float deltaTime);
	static void RegisterCamera(Camera& camera);
	static bool IsKeyPressed(int key);

	static EventManager<float, float, float, float> camreaEventManager;

	static float mouseRotateSensitivity;
	static float mouseTranslateSensitivity;
	static float movementSpeed;
	static float scrollSensitivity;
private:
	static bool keys[1024];
	static float lastX, lastY, xOffset, yOffset;
	static bool firstMouse;
	static bool leftMousePressed;
	static bool middleMousePressed;
	static float scrollOffset;
};