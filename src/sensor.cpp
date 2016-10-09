#include "..\include\sensor.h"

// Revise bracket style across project

namespace simulation {
	const float PI = 3.14159265358979f;

	Sensor_Attribs::Sensor_Attribs() 
	{
	}

	Sensor_Attribs::Sensor_Attribs(const vec4& colour, const vec2& position, const vec2& heading, 
		float theta, float radius) : colour(colour), position(position), radius(radius) 
	{
		float heading_angle = atan2(heading.y, heading.x) * 180.f / PI;

		float start_arm_angle_deg = heading_angle - theta * 0.5f;
		float end_arm_angle_deg = heading_angle + theta * 0.5f;

		float start_arm_angle_rad = start_arm_angle_deg * PI / 180.f;
		float end_arm_angle_rad = end_arm_angle_deg * PI / 180.f;

		start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
		end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};
	}

	void Sensor_Attribs::update_beam_headings(float rotation) {
		mat4 m = rotate(rotation);


		vec4 s_ = mult(m, vec4{start.x, start.y, 0.f, 1.f});
		vec4 e_ = mult(m, vec4{end.x, end.y, 0.f, 1.f});

		start = vec2{s_.x, s_.y};
		end = vec2{e_.x, e_.y};
	}

	void Sensor::init() 
	{
		shader = {
			"shaders/sensor.v.glsl",
			"shaders/sensor.f.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		vec4 points[4] = {
			{-0.5f, -0.5f, 0.f, 0.f},
			{-0.5f,  0.5f, 0.f, 1.f},
			{0.5f, -0.5f, 1.f, 0.f},
			{0.5f,  0.5f, 1.f, 1.f}
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec4), (GLvoid*)(sizeof(float) * 2));
	}

	void Sensor::update(const maths::vec2& cursor_pos) 
	{

	}

	void Sensor::draw() 
	{
		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		
		shader.set_uniform("start", attribs.start);
		shader.set_uniform("end", attribs.end);
		shader.set_uniform("radius", attribs.radius);
		shader.set_uniform("colour", attribs.colour);

		mat4 s = scale({attribs.radius * 2.f, attribs.radius * 2.f, 0.f});
		mat4 t = transpose(translate({attribs.position.x, attribs.position.y, 0.f}));
		mat4 r = rotate(0.f);
		mat4 m = mult(mult(s, r), t);

		shader.set_uniform("model", m);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		shader.release();
	}

	void Sensor::destroy() 
	{
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}

	
}