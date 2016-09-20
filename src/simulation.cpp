#include "..\include\simulation.h"

namespace simulation {

	
	 Obstacle obstacle;
	 Vehicle vehicle;

	 GLuint sensor_VAO;
	 GLuint sensor_UBO;
	 Shader sensor_shader;

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


		 Sensor sl = {{
			 {0.f, 0.f, 1.f, 1.f}, // Colour
			 {40.f, 600.f},        // Position
			 {1.f, 1.f},           // Heading
			 {60.f},               // Angle
			 {256.f},              // Radius
		 }};

		Sensor sr = {{
			{1.f, 0.f, 0.f, 1.f}, // Colour
			{40.f, 500.f},        // Position
			{1.f, -1.f},           // Heading
			{60.f},               // Angle
			{256.f},              // Radius
		}};

		vehicle = {
			sl,
			sr,
			{40.f, 550.f},            // Position
			0.1f,                     // Speed
			0.002f                    // Turning force
		};


		resolution = {1366.f, 768.f};
		near_far = {-1.f, 1.f};
		resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
		cursor_position = {0.f, 0.f};

		{ // INIT VEHICLE BODY
			vehicle.init();
		}




		{ // INIT OBSTACLE
			obstacle.init();
		}
	}

	void update() {
		vehicle.set_obstacle_ptr(obstacle);
		vehicle.update(cursor_position);

		obstacle.update(cursor_position);
	}

	void draw() {
		{ // DRAW VEHICLE BODY
			vehicle.draw();
		}


		{ // DRAW OBSTACLE
			obstacle.draw();
		}

	}

	void destroy() {
		vehicle.destroy();

		

		obstacle.destroy();
	}
}
