#include "..\include\simulation.h"

namespace utils {
	void Simulation::init_simulation() {	
		polygon.init_gl();
	}

	void Simulation::draw_simulation(const float fps) {
		polygon.shader.use();
		{
			polygon.shader.set_uniform("vp", {320.f, 320.f});
			polygon.draw_gl();

			polygon.shader.set_uniform("vp", {960.f, 320.f});
			polygon.draw_gl();
		}
		polygon.shader.release();
	}

	void Simulation::destroy_simulation() {
		polygon.destroy_gl();
	}
}
