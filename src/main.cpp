#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype265MT.lib")

#include <sstream>

#include "..\include\simulation.h"
#include "..\include\utils.h"

namespace {
	bool check_running(GLFWwindow* window, int duration) {
		return !glfwWindowShouldClose(window) && utils::elapsed_time() < duration;
	}

	float calc_fps() {
		static float elapsed = 0.f;
		static float fps = 0.f;
		static float start = utils::elapsed_time();

		elapsed = utils::elapsed_time();
		fps = 1.f / (elapsed - start);
		start = elapsed;

		return fps;
	}

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		simulation::Simulation* s = reinterpret_cast<simulation::Simulation*>(glfwGetWindowUserPointer(window));
		s->cursor_position.x = (float)xpos;
		s->cursor_position.y = 768.f - (float)ypos;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			simulation::Simulation* s = reinterpret_cast<simulation::Simulation*>(glfwGetWindowUserPointer(window));

			switch (key) {
			case GLFW_KEY_S:
				s->draw_sensors = !s->draw_sensors;
				break;
			}
		}
	}

}

int main() {
	// GLFW
	if (!glfwInit()) {
		std::cout << "GLFW failed to initialise" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1366, 768, "Vehicles", NULL, NULL);

	if (!window) {
		glfwTerminate();
		std::cout << "GFLW failed to create window" << std::endl;
		return 1;
	} 

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
	glfwSetKeyCallback(window, &key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		
	// Glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		std::cout << "Glew failed to initialise" << std::endl;
		return 1;
	}

	{ // Check everything initialised correctly
		std::stringstream ss;
		ss << "Device Vendor: " << glGetString(GL_VENDOR) << std::endl;
		ss << "Device Renderer: " << glGetString(GL_RENDERER) << std::endl;
		ss << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		ss << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << ss.str() << std::endl;
	}

	// GL Config
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	simulation::Simulation simulation;

	simulation.init();
	glfwSetWindowUserPointer(window, &simulation);

	while (check_running(window, 6000)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		simulation.update();
		simulation.draw();
			
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	simulation.destroy();
	glfwTerminate();

	return 0;
}