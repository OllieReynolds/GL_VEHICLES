#include "..\include\simulation.h"

namespace simulation {

	
	 Obstacle obstacle;
	 Vehicle vehicle;

	 GLuint sensor_VAO;
	 GLuint sensor_UBO;
	 Shader sensor_shader;

	 GLuint obstacle_VAO;
	 GLuint obstacle_UBO;
	 Shader obstacle_shader;

	 vec2 cursor_position;
	 vec2 resolution;
	 vec2 near_far;
	 mat4 resolution_matrix;


	void init() {
		obstacle = {
			{1.f, 1.f, 1.f, 1.f}, // White
			{600.f, 450}, // In line with the sensor for now
			{32.f} // Smallish
		};

		resolution = {1366.f, 768.f};
		near_far = {-1.f, 1.f};
		resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
		cursor_position = {0.f, 0.f};


		{ // INIT BRAITENBERG BEHICLE

			sensor_shader = {
				"shaders/sensor.v.glsl",
				"shaders/sensor.f.glsl",
				"shaders/sensor.g.glsl"
			};

			sensor_shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &sensor_VAO);
			glBindVertexArray(sensor_VAO);
		
			// UBO to store uniform data in
			glGenBuffers(1, &sensor_UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, sensor_UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 0;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, sensor_UBO);
				glUniformBlockBinding(sensor_shader.program, glGetUniformBlockIndex(sensor_shader.program, "Sectors"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			sensor_shader.release();
		}


		{ // INIT OBSTACLE

			obstacle_shader = {
				"shaders/obstacle.v.glsl",
				"shaders/obstacle.f.glsl",
				"shaders/obstacle.g.glsl"
			};

			obstacle_shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &obstacle_VAO);
			glBindVertexArray(obstacle_VAO);

			// UBO to store uniform data in
			glGenBuffers(1, &obstacle_UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, obstacle_UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Obstacle), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 1;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, obstacle_UBO);
				glUniformBlockBinding(obstacle_shader.program, glGetUniformBlockIndex(obstacle_shader.program, "Obstacles"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			obstacle_shader.release();
		}
	}

	void update() {
		vehicle.move(obstacle);
		obstacle.move(cursor_position);
	}

	void draw() {
		{ // DRAW OBSTACLE
		    // Bind GL context
			glBindVertexArray(obstacle_VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, obstacle_UBO);
			obstacle_shader.use();


			{ // DRAW OBSTACLE
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Obstacle), &obstacle, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			obstacle_shader.release();
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}


		{ // DRAW BRAITENBEG VEHICLE
			// Bind GL context
			glBindVertexArray(sensor_VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, sensor_UBO);
			sensor_shader.use();


			{ // DRAW LEFT SENSOR
				vehicle.left_sensor.set_sector_arms(vehicle.velocity, 45.f);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), &vehicle.left_sensor, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW LEFT SENSOR
				vehicle.right_sensor.set_sector_arms(vehicle.velocity, 45.f);
				glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), &vehicle.right_sensor, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			sensor_shader.release();
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void destroy() {
		glDeleteBuffers(1, &sensor_UBO);
		glDeleteVertexArrays(1, &sensor_VAO);
		sensor_shader.destroy();

		glDeleteBuffers(1, &obstacle_UBO);
		obstacle_shader.destroy();
		glDeleteVertexArrays(1, &obstacle_VAO);
	}
}
