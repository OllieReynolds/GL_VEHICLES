#include "..\include\vehicle.h"

namespace simulation {
	void Vehicle::init() {
		shader = {
			"shaders/default.v.glsl",
			"shaders/default.f.glsl",
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		float points[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		set_gl_buffer_data(sizeof(points), &points);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		right_sensor.init();
		left_sensor.init();
	}

	void Vehicle::update(const maths::vec2& cursor_pos) {
		/////////////////////////////////////////////////////////////////

		vec2 vehicle_heading = polar_to_cartesian(to_radians(rotation));
		velocity = normalise(vehicle_heading);
		velocity.y = -velocity.y;

		position += velocity * speed;

		left_sensor.parent_transform = {position, size, rotation};
		left_sensor.update(cursor_pos);

		right_sensor.parent_transform = {position, size, rotation};
		right_sensor.update(cursor_pos);
	
		other_vehicle_locations.push_back(cursor_pos);

		other_vehicle_locations.erase(std::remove_if(other_vehicle_locations.begin(), other_vehicle_locations.end(), [&](vec2 v) -> bool {
			return almost_equal(v, position.XY(), 1.f);
		}), other_vehicle_locations.end());
		
		const static std::pair<vec2, vec2> top = {{34.f, 748.f},{1332.f, 748.f}};
		const static std::pair<vec2, vec2> right = {top.second, {1332.f, 172.f}};
		const static std::pair<vec2, vec2> bottom = {right.second, {34.f, 172.f}};
		const static std::pair<vec2, vec2> left = {bottom.second, top.first};

		bool l = (
			left_sensor.intersects_line(top.first, top.second)
			|| left_sensor.intersects_line(right.first, right.second)
			|| left_sensor.intersects_line(bottom.first, bottom.second)
			|| left_sensor.intersects_line(left.first, left.second)
			|| left_sensor.intersects(other_vehicle_locations));

		bool r = (
			right_sensor.intersects_line(top.first, top.second)
			|| right_sensor.intersects_line(right.first, right.second)
			|| right_sensor.intersects_line(bottom.first, bottom.second)
			|| right_sensor.intersects_line(left.first, left.second)
			|| right_sensor.intersects(other_vehicle_locations));

		if (l && r) {
			rotation += turning_force * 2.f;
			speed = max(speed - 0.004f, .5f);
		} else if (l) {
			rotation += turning_force;
			speed += 0.002f;
		} else if (r) { 
			rotation -= turning_force;
			speed += 0.002f;
		}
		else {
			speed = max(speed - 0.001f, .5f);
		}
		
	}

	void Vehicle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale(size);
		mat4 t = transpose(translate(position));
		mat4 r = rotate_z(rotation);
		mat4 m = mult(mult(s, r), t);
		shader.set_uniform("model", m);
		
		shader.set_uniform("uniform_colour", maths::vec4(1.f, 0.f, 1.f, 1.f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shader.release();
		glBindVertexArray(0);
	}

	void Vehicle::draw_sensors() {
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