#include "..\include\simulation.h"



// Data Transformation
namespace logic {

	float speed = 0.1f;
	float force = 0.02f;

	vec2 velocity = {0.f, 0.f};
	vec2 acceleration = {0.f, 0.f};

	void move_vehicle() {
		
		if (maths::point_segment_intersect(obstacle.position, sensor.start, sensor.position, sensor.end, sensor.radius)) {
			vec2 desired_velocity = normalise(obstacle.position - sensor.position) * speed;
			vec2 steering_force = (desired_velocity - velocity) * force;
			acceleration += steering_force;
			velocity += acceleration;
			sensor.position += velocity;
			acceleration = {0.f, 0.f};
		} 

		

		//obstacle.position.x = obstacle.position.x - (sin(utils::elapsed_time()) * 4.f);

			
	}
}

namespace simulation {
	void init() {
		{ // INIT BRAITENBERG BEHICLE

			graphics::sensor_shader = {
				"shaders/sensor.v.glsl",
				"shaders/sensor.f.glsl",
				"shaders/sensor.g.glsl"
			};

			graphics::sensor_shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &graphics::sensor_VAO);
			glBindVertexArray(graphics::sensor_VAO);
		
			// UBO to store uniform data in
			glGenBuffers(1, &graphics::sensor_UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::sensor_UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 0;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, graphics::sensor_UBO);
				glUniformBlockBinding(graphics::sensor_shader.program, glGetUniformBlockIndex(graphics::sensor_shader.program, "Sectors"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			graphics::sensor_shader.release();
		}


		{ // INIT OBSTACLE

			graphics::obstacle_shader = {
				"shaders/obstacle.v.glsl",
				"shaders/obstacle.f.glsl",
				"shaders/obstacle.g.glsl"
			};

			graphics::obstacle_shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &graphics::obstacle_VAO);
			glBindVertexArray(graphics::obstacle_VAO);

			// UBO to store uniform data in
			glGenBuffers(1, &graphics::obstacle_UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::obstacle_UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Obstacle), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 1;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, graphics::obstacle_UBO);
				glUniformBlockBinding(graphics::obstacle_shader.program, glGetUniformBlockIndex(graphics::obstacle_shader.program, "Obstacles"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			graphics::obstacle_shader.release();
		}
	}

	void update() {
		logic::move_vehicle();
	}

	void draw() {
		{ // DRAW OBSTACLE
		    // Bind GL context
			glBindVertexArray(graphics::obstacle_VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::obstacle_UBO);
			graphics::obstacle_shader.use();


			{ // DRAW OBSTACLE
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Obstacle), &logic::obstacle, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			graphics::obstacle_shader.release();
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}


		{ // DRAW BRAITENBEG VEHICLE
			// Bind GL context
			glBindVertexArray(graphics::sensor_VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::sensor_UBO);
			graphics::sensor_shader.use();


			{ // DRAW LEFT SENSOR
				logic::sensor = logic::create_sector(logic::sensor.colour, logic::sensor.position, logic::velocity, 45.f, logic::sensor.radius);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), &logic::sensor, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			graphics::sensor_shader.release();
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void destroy() {
		glDeleteBuffers(1, &graphics::sensor_UBO);
		glDeleteVertexArrays(1, &graphics::sensor_VAO);
		graphics::sensor_shader.destroy();

		glDeleteBuffers(1, &graphics::obstacle_UBO);
		graphics::obstacle_shader.destroy();
		glDeleteVertexArrays(1, &graphics::obstacle_VAO);
	}
}
