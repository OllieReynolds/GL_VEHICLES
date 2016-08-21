#include "..\include\simulation.h"

namespace utils {
	void Simulation::init_simulation() {	
		p.init_gl();
	}

	void Simulation::draw_simulation(const float fps) {
		p.set_position({320.f, 320.f});
		p.draw_gl();
		p.set_position({960.f, 320.f});
		p.draw_gl();
	}

	void Simulation::destroy_simulation() {
		p.destroy_gl();
	}
}
