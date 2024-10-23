#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <string>
#include "InputManager.h"
#include "Timer.h"
#include "MeshletCullingScene.h"

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

int screenWidth = 800;
int screenHeight = 480;

std::string basePath;

void initialize_glfw()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void intialize_glad()
{
	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
}

GLFWwindow* create_window(const char* title, int& width, int& height, int monitorID = 0, const bool isFullScreen = false)
{
	if (isFullScreen)
	{
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		if (monitorID >= count)
		{
			monitorID = 0;
		}
		GLFWmonitor* monitor = monitors[monitorID];
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		width = mode->width;
		height = mode->height;
		int width_mm, height_mm;
		glfwGetMonitorPhysicalSize(monitor, &width_mm, &height_mm);
		std::cout << "Monitor Physical Size: " << width_mm << "mm x " << height_mm << "mm" << std::endl;
		return glfwCreateWindow(width, height, title, monitor, nullptr);
	}
	else
	{
		return glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
}

EventManager<int, int> windowResizeEvents;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	windowResizeEvents.Notify(width, height);
}

int main()
{
	
#ifdef SOLUTION_DIR
	basePath = STRINGIZE_VALUE_OF(SOLUTION_DIR);
#else
	basePath = "";
#endif


	initialize_glfw();

	GLFWwindow* window = create_window("FastCII", screenWidth, screenHeight);
	glfwMakeContextCurrent(window);
	
	intialize_glad();

	// Register callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, InputManager::KeyCallback);
	glfwSetCursorPosCallback(window, InputManager::MouseCallback);
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);
	glfwSetScrollCallback(window, InputManager::ScrollCallback);

	// デプステストを有効化
    glEnable(GL_DEPTH_TEST);

    // 背面カリングを有効化
    glEnable(GL_CULL_FACE);

	// Rendering Scope
	{
		auto scene = std::make_unique<MeshletCullingScene>();

		windowResizeEvents.Subscribe(
			[&scene](int width, int height) {scene->OnWindowResize(width, height);
		});
		framebuffer_size_callback(window, screenWidth, screenHeight);

		scene->Start();

		Timer timer;

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			if (InputManager::IsKeyPressed(GLFW_KEY_ESCAPE))
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			timer.Update(true);
			float deltaTime = timer.GetDeltaTime();
			InputManager::Update(deltaTime);

			scene->Update(deltaTime);

			glfwSwapBuffers(window);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}