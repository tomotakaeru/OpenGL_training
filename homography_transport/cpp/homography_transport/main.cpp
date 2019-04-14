#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
//#include <Eigen/Core> //not in use, path check


static void OnKeyEvent(GLFWwindow *window, int key,
	int scancode, int action, int mods) {
	printf("Key: %c\n", key);
}

static void OnMouseEvent(GLFWwindow *window, int button,
	int action, int mods) {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		printf("Mouse: %f %f\n", x, y);
	}
}


int main(void)
{
	const int windowWidth = 640;
	const int windowHeight = 480;


	// Initialize GLFW
	if (!glfwInit()) {
		return -1;
	}

	// Select OpenGL Version 3.2 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Make window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -2;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	// Set interval for vertical syncronization
	glfwSwapInterval(1);

	// Initialize GLEW
	if ((glewInit() != GLEW_OK)) {
		return -3;
	}

	// Set events
	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetMouseButtonCallback(window, OnMouseEvent);

	// Set background color
	glClearColor(0, 1, 1, 0.1); //G+B

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		// Render
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw here

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
