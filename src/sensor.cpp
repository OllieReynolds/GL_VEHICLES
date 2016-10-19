#include "..\include\sensor.h"

namespace simulation {
	const float PI = 3.14159265358979f;

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
		transform.position = parent_transform.position;

		
		float d = parent_transform.rotation + transform.rotation;
		float r = d * PI / 180.f;
		heading = vec2{cos(r), sin(r)};

		//heading.x = cos(utils::elapsed_time());

		//transform.rotation = atan2(heading.y, heading.x) * 180.f / PI;

		scan(cursor_pos);
	}

	void Sensor::scan(const maths::vec2& position) {
		
		std::pair<vec2, vec2> arms_AB = get_sensor_arms_AB();

		bool test_intersect_of_point = point_segment_intersect(
			position,
			arms_AB.first,
			transform.position,
			arms_AB.second,
			transform.size.x * 0.5f
		);

		detected_object = test_intersect_of_point;
	}

	std::pair<vec2, vec2> Sensor::get_sensor_arms_AB() {
		float heading_angle = atan2(heading.y, heading.x) * 180.f / PI;

		float start_arm_angle_deg = heading_angle - 50.f * 0.5f;
		float end_arm_angle_deg = heading_angle + 50.f * 0.5f;

		float start_arm_angle_rad = start_arm_angle_deg * PI / 180.f;
		float end_arm_angle_rad = end_arm_angle_deg * PI / 180.f;

		vec2 start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
		vec2 end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};

		return std::pair<vec2, vec2>(start, end);
	}

	void Sensor::draw() {
		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		mat4 s = scale(vec3{transform.size, 0.f});
		mat4 t = transpose(translate(vec3{transform.position, 0.f}));
		//mat4 r = rotate(transform.rotation);
		//mat4 m = mult(mult(s, r), t);
		mat4 m = mult(s, t);

		shader.set_uniform("model", m);

		std::pair<vec2, vec2> arms_AB = get_sensor_arms_AB();



		shader.set_uniform("start", arms_AB.first);
		shader.set_uniform("end", arms_AB.second);
		shader.set_uniform("radius", transform.size.x * 0.5f);
		shader.set_uniform("colour", colour);
		shader.set_uniform("time", utils::elapsed_time());

		

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