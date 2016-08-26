#include "..\include\simulation.h"

namespace utils {
	void Simulation::init_simulation() {	
		polygon.init_gl();
	}

	void Simulation::draw_simulation(const float fps) {
		polygon.shader.use();

		{
			maths::vec2 vehicle_position = utils::resolution * 0.5f;

			{ // Left Sensor
				float xOffset = -50.f;
				float theta = 60.f;
				float rotation = 30.f;
				float size = 256.f;

				polygon.shader.set_uniform("colour", maths::vec4{1.f, 1.f, 1.f, 1.f});
				polygon.shader.set_uniform("vp", {vehicle_position.x + xOffset, vehicle_position.y});

				polygon.shader.set_uniform("size", size);

				polygon.shader.set_uniform("rotation", rotation);
				polygon.shader.set_uniform("theta", theta);

				polygon.draw_gl();
			}
			
			{ // Right Sensor
				float xOffset = 50.f;
				float theta = 60.f;
				float rotation = 330.f;
				float size = 256.f;

				polygon.shader.set_uniform("colour", maths::vec4{1.f, 1.f, 1.f, 1.f});
				polygon.shader.set_uniform("vp", {vehicle_position.x + xOffset, vehicle_position.y});

				polygon.shader.set_uniform("size", size);

				polygon.shader.set_uniform("rotation", rotation);
				polygon.shader.set_uniform("theta", theta);
				
				polygon.draw_gl();
			}
		}

		polygon.shader.release();
	}

	void Simulation::destroy_simulation() {
		polygon.destroy_gl();
	}
}
