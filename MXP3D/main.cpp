#define GLEW_STATIC

#include <glew.h>
#include <glfw3.h>
#include <thread>
#include "../Configs/Config.h"

int main() {
	// define variables from the config
	bool fullscreen = FULLSCREEN;
	int resX = RESOLUTION_X;
	int resY = RESOLUTION_Y;

	glfwInit();//intitialize glfw

	//set window parameters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//create window pointer
	GLFWwindow* window;

	//set window to windowed or fullscreen
	if (!fullscreen) {
		window = glfwCreateWindow(resX, resY, "OpenGL", nullptr, nullptr); // Windowed
	}
	else {
		window = glfwCreateWindow(resX, resY, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
	}

	//set context
	glfwMakeContextCurrent(window);

	//hold window open
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (fullscreen) {
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}
	}

	//initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//glfwTerminate();
}