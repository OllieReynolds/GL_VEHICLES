#include "..\include\vehicle.h"

namespace simulation {
	void Vehicle::init(const mat4& projection_matrix) {
		shader = {
			"shaders/default.v.glsl",
			"shaders/default.f.glsl",
		};

		shader.set_uniform("projection", projection_matrix);

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

		right_sensor.init(projection_matrix);
		left_sensor.init(projection_matrix);
	}

	void Vehicle::update(const maths::vec2& cursor_pos) {
		detected = false;
		
		other_vehicle_locations.erase(std::remove_if(other_vehicle_locations.begin(), other_vehicle_locations.end(), [&](vec2 v) -> bool {
			return almost_equal(v, position.XZ(), 1.f);
		}), other_vehicle_locations.end());

		direction = polar_to_cartesian(to_radians(rotation));
		
		if (position.z > 100) {
			if (rotation <= 90)
				rotation -= turning_force;
			else
				rotation += turning_force;

			rotation = (rotation > 360.f) ? 0.f : rotation;
		}
		else if (position.x < -100) {
			if (rotation <= 180)
				rotation -= turning_force;
			else
				rotation += turning_force;

			rotation = (rotation > 360.f) ? 0.f : rotation;
		}
		else if (position.z < -100) {
			if (rotation <= 270)
				rotation -= turning_force;
			else
				rotation += turning_force;

			rotation = (rotation > 360.f) ? 0.f : rotation;
		}
		else if (position.x > 100) {
			if (rotation <= 360)
				rotation -= turning_force;
			else
				rotation += turning_force;

			rotation = (rotation > 360.f) ? 0.f : rotation;
		}
		
		std::for_each(other_vehicle_locations.begin(), other_vehicle_locations.end(), [&](vec2 pos) -> auto {
			float d = distance(position.XZ(), pos);
			if (d < 20.f) {
				vec2 c = pos - position.XZ();
				c = normalise(c);
				float d = dot_product(direction, c);
				float e = to_degrees(acosf(d));

				if (e < 90.f) {
					rotation += turning_force;
					detected = true;
				} 
				else if (e > 270.f && e < 360.f) {
					rotation -= turning_force;
					detected = true;
				}	
			}
		});

		velocity = direction * speed;
		position += velocity;
	

		

		/*if (position.z > 50 || position.z < -50 || position.x > 50 || position.x < -50)
			rotation += turning_force;*/
	
		
		/*const static std::pair<vec2, vec2> top = {{-50.f, 50.f}, {50.f, 50.f}};
		const static std::pair<vec2, vec2> right = {top.second, {50.f, -50.f}};
		const static std::pair<vec2, vec2> bottom = {right.second, {-50.f, -50.f}};
		const static std::pair<vec2, vec2> left = {bottom.second, top.first};


		bool l = (
		left_sensor.intersects_line(top.first, top.second)
		|| left_sensor.intersects_line(right.first, right.second)
		|| left_sensor.intersects_line(bottom.first, bottom.second)
		|| left_sensor.intersects_line(left.first, left.second));

		if (l) {
				rotation += turning_force;
				speed += 0.002f;
		}*/

		/*vec2 vehicle_heading = polar_to_cartesian(to_radians(rotation));
		velocity = normalise(vehicle_heading);
		position += velocity * speed;



		left_sensor.parent_transform = {position, size, rotation};
		right_sensor.parent_transform = {position, size, rotation};

		left_sensor.update(cursor_pos);
		right_sensor.update(cursor_pos);*/

		/*other_vehicle_locations.push_back(cursor_pos);

		other_vehicle_locations.erase(std::remove_if(other_vehicle_locations.begin(), other_vehicle_locations.end(), [&](vec2 v) -> bool {
		return almost_equal(v, position.XZ(), 1.f);
		}), other_vehicle_locations.end());*/

		//bool l = (
		//	left_sensor.intersects_line(top.first, top.second)
		//	|| left_sensor.intersects_line(right.first, right.second)
		//	|| left_sensor.intersects_line(bottom.first, bottom.second)
		//	|| left_sensor.intersects_line(left.first, left.second)
		//	|| left_sensor.intersects(other_vehicle_locations));

		//bool r = (
		//	right_sensor.intersects_line(top.first, top.second)
		//	|| right_sensor.intersects_line(right.first, right.second)
		//	|| right_sensor.intersects_line(bottom.first, bottom.second)
		//	|| right_sensor.intersects_line(left.first, left.second)
		//	|| right_sensor.intersects(other_vehicle_locations));

		//if (l && r) {
		//	rotation += turning_force * 2.f;
		//	speed = max(speed - 0.004f, .5f);
		//} else if (l) {
		//	rotation += turning_force;
		//	speed += 0.002f;
		//} else if (r) { 
		//	rotation -= turning_force;
		//	speed += 0.002f;
		//}
		//else {
		//	//speed = max(speed - 0.001f, .5f);
		//	rotation += 2;
		//	//speed += 0.002f;
		//}
		
	}

	void Vehicle::draw(const mat4& view_matrix, const mat4& projection_matrix) {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale(size);
		mat4 t = transpose(translate(position));
		mat4 r = rotate_y(rotation);
		mat4 m = mult(mult(s, r), t);
		shader.set_uniform("model", m);

		vec3 camera_pos;
		camera_pos.x = cos(utils::elapsed_time() * 0.5f) * 100.f;
		camera_pos.y = 100.f + cos(utils::elapsed_time() * 0.2f) * 95.f;
		camera_pos.z = sin(utils::elapsed_time() * 0.5f) * 100.f;
		

		shader.set_uniform("view", view_matrix);
		shader.set_uniform("projection", projection_matrix);
		
		shader.set_uniform("uniform_colour", maths::vec4(1.f, 0.f, 1.f, 1.f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		glLineWidth(2.f);
		shader.set_uniform("uniform_colour", maths::vec4(0.f, 1.f, 1.f, 1.f));
		glDrawArrays(GL_LINE_STRIP, 0, 36);

		shader.release();
		glBindVertexArray(0);
	}

	void Vehicle::draw_sensors(const mat4& view_matrix, const mat4& projection_matrix) {
		left_sensor.draw(view_matrix, projection_matrix);
		right_sensor.draw(view_matrix, projection_matrix);
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

		float degs = to_degrees(cartesian_to_polar(direction));

		attribs.push_back("     ROTATION:" + utils::friendly_float(degs));
		attribs.push_back("        SPEED:" + utils::friendly_float(speed));
		attribs.push_back("TURNING FORCE:" + utils::friendly_float(turning_force));
		attribs.push_back("     VELOCITY:" + utils::friendly_float(velocity.x) + ", " + utils::friendly_float(velocity.y));

		return attribs;
	}
}