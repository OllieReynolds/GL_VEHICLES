#include "..\include\sensor.h"

namespace simulation {
	void Sensor::init() 
	{
		shader = {
			"shaders/sensor.v.glsl",
			"shaders/sensor.f.glsl"
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
	}

	void Sensor::update(const maths::vec2& cursor_pos) 
	{
		position = parent_transform.position;

		float degrees = -(parent_transform.rotation - rotation);
		heading = polar_to_cartesian(to_radians(degrees));
	}

	bool Sensor::intersects(const std::vector<maths::vec2>& points) {

		std::pair<vec2, vec2> arms_AB = get_sensor_arms();

		for (maths::vec2 p : points) {
			if (point_segment_intersect(p, arms_AB.first, position, arms_AB.second, size.x * 0.5f)) 
				return true;
		}

		return false;
	}

	bool Sensor::intersects_line(const vec2& a, const vec2& b) {

		std::pair<vec2, vec2> arms_AB = get_sensor_arms();

		vec2 l_a = position;
		vec2 l_b = position + (arms_AB.second * (size.y * 0.5f));

		vec2 r_a = position;
		vec2 r_b = position + (arms_AB.first * (size.x * 0.5f));


		if (shared::line_intersect(l_a, l_b, a, b))
			return true;
		else if (shared::line_intersect(r_a, r_b, a, b))
			return true;
		else
			return false;
	}

	std::pair<vec2, vec2> Sensor::get_sensor_arms() {
		float heading_angle = to_degrees(cartesian_to_polar(heading));

		float s = to_radians(heading_angle - 30.f);
		float e = to_radians(heading_angle + 30.f);

		return std::pair<vec2, vec2>(polar_to_cartesian(s), polar_to_cartesian(e));
	}

	void Sensor::draw() {
		shader.use();

		std::pair<vec2, vec2> arms_AB = get_sensor_arms();

		shader.set_uniform("start", arms_AB.first);
		shader.set_uniform("end", arms_AB.second);
		shader.set_uniform("radius", size.x * 0.5f);
		shader.set_uniform("colour", colour);
		shader.set_uniform("time", utils::elapsed_time());
		shader.set_uniform("model", gen_model_matrix(size, position));

		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		shader.release();
	}

	void Sensor::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}

	
}