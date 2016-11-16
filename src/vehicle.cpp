#include "..\include\vehicle.h"

namespace simulation {
	void Vehicle::init() {
		shader = {
			"shaders/default.v.glsl",
			"shaders/default.f.glsl",
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		vec4 points[4] = {
			{-0.5f, -0.5f, 0.f, 0.f},
			{-0.5f,  0.5f, 0.f, 1.f},
			{0.5f, -0.5f, 1.f, 0.f},
			{0.5f,  0.5f, 1.f, 1.f}
		};

		set_gl_buffer_data(sizeof(points), &points);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)(sizeof(float) * 2));

		right_sensor.init();
		left_sensor.init();
	}

	void Vehicle::update(const maths::vec2& cursor_pos) {

		static float rotation_interval = 5.5f;
		static float movement_interval = 5.5f;

		/////////////////////////////////////////////////////////////////

		vec2 vehicle_heading = polar_to_cartesian(to_radians(rotation));
		velocity = normalise(vehicle_heading);
		velocity.y = -velocity.y;

		position += velocity * movement_interval;

		left_sensor.parent_transform = {position, size, rotation};
		left_sensor.update(cursor_pos);

		right_sensor.parent_transform = {position, size, rotation};
		right_sensor.update(cursor_pos);
	
		/////////////////////////////////////////////////////////////////

		std::vector<vec2> points = {cursor_pos};
		
		if (left_sensor.intersects(points))
			rotation += rotation_interval;
		else if (right_sensor.intersects(points))
			rotation -= rotation_interval;


		const static std::pair<vec2, vec2> top = {{34.f, 748.f},{1332.f, 748.f}};
		const static std::pair<vec2, vec2> right = {top.second, {1332.f, 172.f}};
		const static std::pair<vec2, vec2> bottom = {right.second, {34.f, 172.f}};
		const static std::pair<vec2, vec2> left = {bottom.second, top.first};

		if (
			   left_sensor.intersects_line(top.first, top.second) 
			|| left_sensor.intersects_line(right.first, right.second) 
			|| left_sensor.intersects_line(bottom.first, bottom.second) 
			|| left_sensor.intersects_line(left.first, left.second)) 
			    rotation += rotation_interval;
		else if (
			   right_sensor.intersects_line(top.first, top.second)
			|| right_sensor.intersects_line(right.first, right.second)
			|| right_sensor.intersects_line(bottom.first, bottom.second)
			|| right_sensor.intersects_line(left.first, left.second))
			    rotation -= rotation_interval;
		
	}

	void Vehicle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale({size, 0.f});
		mat4 t = transpose(translate({position, 0.f}));
		mat4 r = rotate_z(rotation);
		mat4 m = mult(mult(s, r), t);

		shader.set_uniform("model", m);
		shader.set_uniform("time", utils::elapsed_time());
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader.release();
		glBindVertexArray(0);

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

	std::vector<std::string> Vehicle::string_attribs() {
		std::vector<std::string> attribs;

		attribs.push_back("     ROTATION: " + std::to_string(rotation));
		attribs.push_back("TURNING FORCE: " + std::to_string(turning_force));
		attribs.push_back("     VELOCITY: " + to_string(velocity));
		attribs.push_back(" ACCELERATION: " + to_string(acceleration));

		return attribs;
	}
}