#include "..\include\simulation.h"

// CONFIG
namespace { 
	static const maths::vec2 resolution = {1366.f, 768.f};
	static const maths::vec2 near_far = {-1.f, 1.f};
	static const maths::mat4 resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
}

// VEHICLE DATA
namespace { 
	struct Sector {
		maths::vec4 colour;
		maths::vec2 position;
		maths::vec2 size;
		float rotation;
		float theta;
	};

	Sector sector1 = {{1.f},{500.f, 500.f},{128.f}, 270.f, 40.f};
	Sector sector2 = {{1.f},{500.f, 556.f},{128.f}, 270.f, 40.f};

	GLuint VAO;
	GLuint UBO;

	utils::Shader shader;

	void spin_sensors() {
		float t = utils::elapsed_time();
		float s = sin(t) * 0.5f;

		sector1.rotation -= s;
		sector2.rotation += s;
	}

	void move_vehicle() {
		float t = utils::elapsed_time() * 48.f;

		sector1.position.x = t;
		sector2.position.x = t;
	}
}

namespace simulation {
	void Simulation::init_simulation() {
		{ // INIT BRAITENBERG BEHICLE
			shader = {
				"shaders/polygon.v.glsl",
				"shaders/polygon.f.glsl",
				"shaders/polygon.g.glsl"
			};

			shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
		
			// UBO to store uniform data in
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sector), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 0;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, UBO);
				glUniformBlockBinding(shader.program, glGetUniformBlockIndex(shader.program, "Sectors"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void Simulation::update_simulation() {
		move_vehicle();
		spin_sensors();
	}

	void Simulation::draw_simulation(const float fps) {
		
		{ // DRAW BRAITENBEG VEHICLE

			// Bind GL context
			shader.use();
			glBindVertexArray(VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);

			{ // DRAW LEFT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sector), &sector1, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW RIGHT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sector), &sector2, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			shader.release();
		}
	}

	void Simulation::destroy_simulation() {
		shader.destroy();
		glDeleteVertexArrays(1, &VAO);
	}
}
