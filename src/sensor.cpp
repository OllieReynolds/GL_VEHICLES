#include "..\include\sensor.h"

// Revise bracket style across project

namespace simulation {
	const float PI = 3.14159265358979f;

	GLuint Sensor::bind_index = 0;

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

	void Sensor::init() 
	{
		shader = {
			"shaders/sensor.v.glsl",
			"shaders/sensor.f.glsl",
			"shaders/sensor.g.glsl"
		};

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer_object);

		glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor_Attribs), &attribs, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, gl_buffer_object);
		glUniformBlockBinding(shader.program, glGetUniformBlockIndex(shader.program, "Sectors"), bind_index);
		bind_index++;
	}

	void Sensor::update(const maths::vec2& cursor_pos) 
	{

	}

	void Sensor::draw() 
	{
		bind_index = 0;

		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer_object);
		
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Sensor_Attribs), &attribs, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, gl_buffer_object);
		glUniformBlockBinding(shader.program, glGetUniformBlockIndex(shader.program, "Sectors"), bind_index);
		bind_index++;

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		glDrawArrays(GL_POINTS, 0, 1);
	}

	void Sensor::destroy() 
	{
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}
}