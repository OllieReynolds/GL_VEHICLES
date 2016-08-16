#include "..\include\simulation.h"

namespace utils {
	void Simulation::init_simulation() {	
		p.init_gl();
	}

	void Simulation::draw_simulation(const float fps) {
		p.set_position({32.f, 32.f});
		p.draw_gl();
		p.set_position({96.f, 96.f});
		p.draw_gl();
	}

	void Simulation::destroy_simulation() {
		p.destroy_gl();
	}
}
