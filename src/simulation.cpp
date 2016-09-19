#include "..\include\simulation.h"

namespace simulation {

	
	 Obstacle obstacle;
	 Vehicle vehicle;

	 vec2 points[4] = {
		 {-0.5f, -0.5f},
		 {-0.5f,  0.5f},
		 { 0.5f, -0.5f},
		 { 0.5f,  0.5f}
	 };

	 GLuint sensor_VAO;
	 GLuint sensor_UBO;
	 Shader sensor_shader;

	 GLuint vehicle_VAO;
	 GLuint vehicle_VBO;
	 Shader vehicle_shader;
	 mat4 vehicle_matrix;

	 vec2 cursor_position;
	 vec2 resolution;
	 vec2 near_far;
	 mat4 resolution_matrix;


	void init() {
		obstacle = {{
			{1.f, 1.f, 1.f, 1.f},     // Colour
			{600.f, 450},             // Position
			{32.f}                    // Size
		}};

		vehicle = {
			Sensor(
				{1.f, 1.f, 0.f, 1.f}, // Colour
				{40.f, 600.f},        // Position
				{1.f, 1.f},           // Heading
				{60.f},               // Angle
				{256.f}               // Radius
			),
			Sensor(
				{1.f, 1.f, 0.f, 1.f}, // Colour
				{40.f, 500.f},        // Position
				{1.f, -1.f},          // Heading
				{60.f},               // Angle
				{256.f}               // Radius
			),
			{40.f, 550.f},            // Position
			0.1f,                     // Speed
			0.002f                    // Turning force
		};


		resolution = {1366.f, 768.f};
		near_far = {-1.f, 1.f};
		resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
		cursor_position = {0.f, 0.f};

		{ // INIT VEHICLE BODY
			vehicle_shader = {
				"shaders/default.v.glsl",
				"shaders/default.f.glsl",
			};

			vehicle_shader.set_uniform("model", vehicle.model);
			vehicle_shader.set_uniform("projection", resolution_matrix);

			glGenVertexArrays(1, &vehicle_VAO);
			glBindVertexArray(vehicle_VAO);

			glGenBuffers(1, &vehicle_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, vehicle_VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, &points, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			vehicle_shader.release();
		}

		{ // INIT SENSOR
			sensor_shader = {
				"shaders/sensor.v.glsl",
				"shaders/sensor.f.glsl",
				"shaders/sensor.g.glsl"
			};

			sensor_shader.set_uniform("projection", resolution_matrix);

			glGenVertexArrays(1, &sensor_VAO);
			glBindVertexArray(sensor_VAO);
		
			glGenBuffers(1, &sensor_UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, sensor_UBO);

			glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), NULL, GL_STATIC_DRAW);
			GLuint bind_index = 0;
			glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, sensor_UBO);
			glUniformBlockBinding(sensor_shader.program, glGetUniformBlockIndex(sensor_shader.program, "Sectors"), bind_index);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			sensor_shader.release();
		}


		{ // INIT OBSTACLE
			obstacle.init();
		}
	}

	void update() {
		vehicle.move(obstacle);
		obstacle.update(cursor_position);
	}

	void draw() {
		{ // DRAW VEHICLE BODY
			glBindVertexArray(vehicle_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, vehicle_VBO);
			vehicle_shader.use();

			vehicle_matrix.scale({100.f, 100.f, 100.f});
			vehicle_matrix.translate({1366.f/2.f, 768.f/2.f, 0.f});
			vehicle_shader.set_uniform("model", vehicle_matrix);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			vehicle_shader.release();
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}


		{ // DRAW OBSTACLE
			obstacle.draw();
		}


		{ // DRAW SENSORS
			glBindVertexArray(sensor_VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, sensor_UBO);
			sensor_shader.use();

			// DRAW LEFT SENSOR
			//vehicle.left_sensor.set_sector_arms(vehicle.velocity, 45.f);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), &vehicle.left_sensor, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
			
			// DRAW LEFT SENSOR
			//vehicle.right_sensor.set_sector_arms(vehicle.velocity, 45.f);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor), &vehicle.right_sensor, GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
			
			sensor_shader.release();
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void destroy() {
		glDeleteBuffers(1, &vehicle_VBO);
		glDeleteVertexArrays(1, &vehicle_VAO);
		vehicle_shader.destroy();

		glDeleteBuffers(1, &sensor_UBO);
		glDeleteVertexArrays(1, &sensor_VAO);
		sensor_shader.destroy();

		obstacle.destroy();
	}
}
