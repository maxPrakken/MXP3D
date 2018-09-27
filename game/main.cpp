#include "../common/renderer.h"

int main(void)
{
	Renderer::getInstance();

	do {
		Renderer::getInstance()->update();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(Renderer::getInstance()->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(Renderer::getInstance()->getWindow()) == 0);

	Renderer::getInstance()->destroyWindow();

	return 0;
}
