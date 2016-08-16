#include "..\include\simulation.h"

namespace utils {
	Simulation::Simulation() {
		p = Polygon();
	}

	void Simulation::init_simulation() {	
		p.init_gl();
	}

	void Simulation::update_simulation(GLFWwindow* window) {
		
	}

	void Simulation::draw_simulation(const float fps) {
		p.draw_gl();
	}

	void Simulation::destroy_simulation() {
		
	}
}
