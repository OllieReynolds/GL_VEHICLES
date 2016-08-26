#include "..\include\simulation.h"

namespace utils {
	void Simulation::init_simulation() {	
		polygon.init_gl();
	}

	void Simulation::draw_simulation(const float fps) {
		polygon.shader.use();


		// Uniforms are getting out of hand - move to UBO Solution soon
		{
			static const float mid = utils::resolution.x / 2;
			static const float fudge = 50.f;

			polygon.shader.set_uniform("vp", {mid - fudge, 320.f});
			polygon.shader.set_uniform("colour", maths::vec4{1.f, 0.f, 0.f, 1.f});
			polygon.draw_gl();

			polygon.shader.set_uniform("vp", {mid, 320.f});
			polygon.shader.set_uniform("colour", maths::vec4{0.f, 1.f, 0.f, 1.f});
			polygon.draw_gl();

			polygon.shader.set_uniform("vp", {mid + fudge, 320.f});
			polygon.shader.set_uniform("colour", maths::vec4{0.f, 0.f, 1.f, 1.f});
			polygon.draw_gl();
		}
		polygon.shader.release();
	}

	void Simulation::destroy_simulation() {
		polygon.destroy_gl();
	}
}
