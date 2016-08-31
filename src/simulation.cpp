#include "..\include\simulation.h"

// CONFIG
namespace { 
	static const maths::vec2 resolution = {1366.f, 768.f};
	static const maths::vec2 near_far = {-1.f, 1.f};
	static const maths::mat4 resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
}

// GFX
namespace graphics { 
	struct Sector {
		maths::vec4 colour;
		maths::vec2 position;
		maths::vec2 size;
		float rotation;
		float theta;

		bool is_body;
	};
	
	Sector light_FOV_L = {{1.f, 0.f, 0.f, 1.f}, {-10.f, 510.f},{256.f}, 270.f, 40.f, false};
	Sector light_FOV_R = {{1.f, 0.f, 0.f, 1.f}, {-10.f, 546.f},{256.f}, 270.f, 40.f, false};

	Sector body = {{1.f}, {-64.f, 528.f}, {96.f, 64.f}, 270.f, 40.f, true};

	Sector box = {{1.f, 1.f, 0.5, 1.f},{593.f, 580.f},{16.f, 16.f}, 270.f, 40.f, true};

	GLuint VAO;
	GLuint UBO;

	utils::Shader shader;
}

// Data Transformation
namespace logic {
	float speed = 2.f;

	void spin_sensors() {
		float t = utils::elapsed_time();
		float s = sin(t) * 0.5f;

		graphics::light_FOV_L.rotation -= s;
		graphics::light_FOV_R.rotation += s;
	}

	void move_vehicle() {
		

		{ // CHECK LEFT BEAM INTERSECTIONS
			float th = graphics::light_FOV_L.theta * 0.5f;
			float rot = graphics::light_FOV_L.rotation + 90.0;

			float radiansA = (rot + th); //* (std::_Pi / 180.f);
			maths::vec2 a = maths::vec2{
				graphics::light_FOV_L.size.x * cos(radiansA),
				graphics::light_FOV_L.size.y * sin(radiansA)
			};
					

			float radiansB = (rot - th); //* (std::_Pi / 180.f);
			maths::vec2 b = maths::vec2{
				graphics::light_FOV_L.size.x * cos(radiansB),
				graphics::light_FOV_L.size.y * sin(radiansB)
			};
		


			maths::vec2 o = graphics::light_FOV_L.position;
			maths::vec2 p = graphics::box.position;

			

			if (maths::intersections::point_segment(p, {a, o, b}))
				speed = 0.f;
		}

		float t = utils::elapsed_time() * 3;
		graphics::box.position.x += sin(t);
		graphics::body.position.x += speed;
		graphics::light_FOV_L.position.x += speed;
		graphics::light_FOV_R.position.x += speed;

		
	}
}

namespace simulation {
	void Simulation::init_simulation() {
		{ // INIT BRAITENBERG BEHICLE
			graphics::shader = {
				"shaders/polygon.v.glsl",
				"shaders/polygon.f.glsl",
				"shaders/polygon.g.glsl"
			};

			graphics::shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &graphics::VAO);
			glBindVertexArray(graphics::VAO);
		
			// UBO to store uniform data in
			glGenBuffers(1, &graphics::UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(graphics::Sector), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 0;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, graphics::UBO);
				glUniformBlockBinding(graphics::shader.program, glGetUniformBlockIndex(graphics::shader.program, "Sectors"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void Simulation::update_simulation() {
		logic::move_vehicle();
		logic::spin_sensors();
	}

	void Simulation::draw_simulation(const float fps) {
		
		{ // DRAW BRAITENBEG VEHICLE

			// Bind GL context
			graphics::shader.use();
			glBindVertexArray(graphics::VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::UBO);

			

			{ // DRAW BODY
				glBufferData(GL_UNIFORM_BUFFER, sizeof(graphics::Sector), &graphics::body, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW LEFT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(graphics::Sector), &graphics::light_FOV_L, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW RIGHT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(graphics::Sector), &graphics::light_FOV_R, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW OBSTACLE
				glBufferData(GL_UNIFORM_BUFFER, sizeof(graphics::Sector), &graphics::box, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			graphics::shader.release();
		}
	}

	void Simulation::destroy_simulation() {
		graphics::shader.destroy();
		glDeleteVertexArrays(1, &graphics::VAO);
	}
}
