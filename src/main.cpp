#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype265MT.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "Box2D.lib")

#include "..\include\simulation.h"
#include "..\include\utils.h"

namespace {
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		Simulation* s = reinterpret_cast<Simulation*>(glfwGetWindowUserPointer(window));
		s->cursor_position.x = (float)xpos;
		s->cursor_position.y = config::resolution.y - (float)ypos;
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
			Simulation* s = reinterpret_cast<Simulation*>(glfwGetWindowUserPointer(window));

			int active_button = s->ui.index_active_button;
			switch (active_button) {
				case 0: s->add_vehicle(gen_random(0.f, 1.f) > 0.5f);			break;
				case 1: s->remove_vehicle();									break;
				case 2: s->camera.follow_vehicle = !s->camera.follow_vehicle;	break;
				case 3: s->is_updating = true;									break;
				case 4: s->is_updating = false;									break;
				case 5: s->reset();												break;
				case 6: glfwSetWindowShouldClose(window, GLFW_TRUE);			break;
				case 7: s->draw_sensors = !s->draw_sensors;						break;
				case 8: s->draw_sensor_outlines = !s->draw_sensor_outlines;		break;
#ifdef _DEBUG
				default: std::cout << s->cursor_position << std::endl;			break;
#endif
			}
		}
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			Simulation* s = reinterpret_cast<Simulation*>(glfwGetWindowUserPointer(window));

			switch (key) {
				case GLFW_KEY_F: s->camera.follow_vehicle = !s->camera.follow_vehicle;	break;
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
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWwindow* window = glfwCreateWindow(config::resolution.x, config::resolution.y, "Vehicles", (config::fullscreen) ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (!window) {
		glfwTerminate();
		std::cout << "GFLW failed to create window" << std::endl;
		return 1;
	} 

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
	glfwSetMouseButtonCallback(window, &mouse_button_callback);
	glfwSetKeyCallback(window, &key_callback);
		
	// Glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		std::cout << "Glew failed to initialise" << std::endl;
		return 1;
	}

#ifdef _DEBUG
	{ // Check everything initialised correctly
		std::stringstream ss;
		ss << "Device Vendor: " << glGetString(GL_VENDOR) << std::endl;
		ss << "Device Renderer: " << glGetString(GL_RENDERER) << std::endl;
		ss << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		ss << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << ss.str() << std::endl;
	}
#endif

	// GL Config
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	
	Simulation simulation = Simulation();
	simulation.init();

	glfwSetWindowUserPointer(window, &simulation);

	while (!glfwWindowShouldClose(window)) {
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