#include "../include/draw_line.h"

void Draw_Line::init_line(const mat4& projection_matrix) {
	shader = {
		"shaders/cube.v.glsl",
		"shaders/cube.f.glsl"
	};

	shader.set_uniform("projection", projection_matrix);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


void Draw_Line::draw_line(const mat4& view_matrix, const vec3& a, const vec3& b) {
	shader.use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vec3 values[2] = {
		a, b
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 2, &values);

	vec3 still_pos = {0.f, 30.f, 150.f};

	shader.set_uniform("view", view_matrix);

	mat4 s = scale(1.f);
	mat4 t = transpose(translate(0.f));
	mat4 r = rotate_y(0.f);
	mat4 m = mult(mult(s, r), t);
	shader.set_uniform("model", m);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth(1.f);
	shader.set_uniform("uniform_colour", colour);
	glDrawArrays(GL_LINES, 0, 2);
}

void Draw_Line::destroy_line() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}