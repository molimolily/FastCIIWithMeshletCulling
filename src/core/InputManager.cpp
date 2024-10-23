#include "InputManager.h"

bool InputManager::keys[1024] = { false };
bool InputManager::firstMouse = true;
bool InputManager::leftMousePressed = false;
bool InputManager::middleMousePressed = false;
float InputManager::mouseRotateSensitivity = 0.1f;
float InputManager::mouseTranslateSensitivity = 0.025f;
float InputManager::lastX = 0.0f;
float InputManager::lastY = 0.0f;
float InputManager::xOffset = 0.0f;
float InputManager::yOffset = 0.0f;
float InputManager::movementSpeed = 4.0f;
float InputManager::scrollOffset = 0.0f;
float InputManager::scrollSensitivity = 0.4f;

EventManager<float, float, float, float> InputManager::camreaEventManager;

void InputManager::Init(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	lastX = width / 2.0f;
	lastY = height / 2.0f;
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool InputManager::IsKeyPressed(int key)
{
	if(key >= 0 && key < 1024)
	{
		return keys[key];
	}
	return false;
}

void InputManager::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if (leftMousePressed || middleMousePressed)
	{
		xOffset += xpos - lastX;
		yOffset += lastY - ypos;
	}

	lastX = xpos;
	lastY = ypos;
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			leftMousePressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			leftMousePressed = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			middleMousePressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			middleMousePressed = false;
		}
	}
}

void InputManager::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	scrollOffset = yOffset;
}

void InputManager::Update(float deltaTime)
{
	camreaEventManager.Notify(deltaTime, xOffset, yOffset, scrollOffset);

	xOffset = 0;
	yOffset = 0;
	scrollOffset = 0;
}

void InputManager::RegisterCamera(Camera& camera)
{
	camreaEventManager.Subscribe([&camera](float deltaTime, float xOffset, float yOffset, float scrollOffset) {
		if (leftMousePressed)
		{
			camera.Rotate(xOffset * mouseRotateSensitivity, yOffset * mouseRotateSensitivity);
		}
		else if (middleMousePressed)
		{
			camera.MoveRight(-xOffset * mouseTranslateSensitivity);
			camera.MoveUp(-yOffset * mouseTranslateSensitivity);
		}

		float velocity = movementSpeed * deltaTime;
		if (keys[GLFW_KEY_W])
		{
			camera.MoveUp(velocity);
		}
		if (keys[GLFW_KEY_S])
		{
			camera.MoveUp(-velocity);
		}
		if (keys[GLFW_KEY_A])
		{
			camera.MoveRight(-velocity);
		}
		if (keys[GLFW_KEY_D])
		{
			camera.MoveRight(velocity);
		}

		camera.MoveForward(scrollOffset * scrollSensitivity);
	});
}