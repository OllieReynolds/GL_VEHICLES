#include "..\include\vehicle.h"

namespace simulation {
	void Vehicle::init() {
		shader = {
			"shaders/default.v.glsl",
			"shaders/default.f.glsl",
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

		right_sensor.init();
		left_sensor.init();
	}

	void Vehicle::update(const maths::vec2& cursor_pos) {
		/*transform.position += 
		transform.rotation += 0.1f;
*/
		left_sensor.parent_transform = transform;
		left_sensor.update(cursor_pos);

		right_sensor.parent_transform = transform;
		right_sensor.update(cursor_pos);

		if (left_sensor.detected_object) {

		}

		if (right_sensor.detected_object) {

		}
	}

	void Vehicle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale({transform.size, 0.f});
		mat4 t = transpose(translate({transform.position, 0.f}));
		mat4 r = rotate(transform.rotation);
		mat4 m = mult(mult(s, r), t);

		shader.set_uniform("model", m);
		
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

		attribs.push_back("     ROTATION: " + std::to_string(transform.rotation));
		attribs.push_back("TURNING FORCE: " + std::to_string(turning_force));
		attribs.push_back("     VELOCITY: " + to_string(velocity));
		attribs.push_back(" ACCELERATION: " + to_string(acceleration));

		return attribs;
	}
}