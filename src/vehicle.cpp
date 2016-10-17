#include "..\include\vehicle.h"

namespace simulation {

	Vehicle::Vehicle(const vec2& position, const vec2& size, const vec2& sensor_offset, 
		float rotation, float speed, float turning_force) : 
		position(position), size(size), sensor_offset(sensor_offset), rotation(rotation), speed(speed),
		turning_force(turning_force), velocity(0.f), acceleration(0.f) 
	{
		left_sensor = {{
			{1.f, 0.f, 0.f, 1.f},             // Colour
			position + vec2{sensor_offset.x, sensor_offset.y},       // Position
			{1.f, 1.f},                       // Heading
			{60.f},                           // Angle
			{256.f},                          // Radius
		}};

		right_sensor = {{
			{1.f, 0.f, 0.f, 1.f},             // Colour
			position + vec2{sensor_offset.x, -sensor_offset.y},      // Position
			{1.f, -1.f},                       // Heading
			{60.f},                           // Angle
			{256.f},                          // Radius
		}};
	}

	void Vehicle::init() {
		shader = {
			"shaders/default.v.glsl",
			"shaders/default.f.glsl",
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 1000.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		vec2 points[4] = {
			{-0.5f, -0.5f},
			{-0.5f,  0.5f},
			{0.5f, -0.5f},
			{0.5f,  0.5f}
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, &points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		right_sensor.init();
		left_sensor.init();
	}

	void Vehicle::update(const maths::vec2& cursor_pos) {
		left_sensor.update(cursor_pos);
		right_sensor.update(cursor_pos);
		move();
	}

	void Vehicle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale({size.x, size.y, 0.f});
		mat4 t = transpose(translate({position.x, position.y, 0.f}));
		mat4 r = rotate(rotation);
		mat4 m = mult(mult(s, r), t);

		shader.set_uniform("model", m);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader.release();
		glBindVertexArray(0);

		///////////////////////////////////////////////////////////
		left_sensor.draw();
		right_sensor.draw();
	}

	void Vehicle::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();

		left_sensor.destroy();
		right_sensor.destroy();
	}



	bool Vehicle::test_sensor_activity(const Sensor& s) {
		return (maths::point_segment_intersect(obstacle->position, s.attribs.start, s.attribs.position, s.attribs.end, s.attribs.radius));
	}

	void Vehicle::move() {

		if (test_sensor_activity(left_sensor)) {
			
		}

		if (test_sensor_activity(right_sensor)) {
			
		}
	}
}