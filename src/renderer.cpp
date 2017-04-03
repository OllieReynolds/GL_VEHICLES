#include "..\include\renderer.h"

void Circle_Renderer::init() {
	shader_2D = {
		"shaders/v.uniform_MP.glsl",
		"shaders/f.uniform_colour_circle.glsl"
	};

	shader_3D = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.uniform_colour_circle.glsl"
	};

	shader_3D_shadow = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.SPOT_SHADOW.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::quad_points_textured), &mesh::quad_points_textured, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Circle_Renderer::draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour, bool filled) {
	shader_2D.use();
	glBindVertexArray(vao);

	shader_2D.set_uniform("uniform_colour", colour);
	shader_2D.set_uniform("projection", camera.matrix_projection_ortho);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));
	shader_2D.set_uniform("draw_filled", filled);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void Circle_Renderer::draw_3D(const Camera& camera, const Transform& transform, const vec4& colour, bool filled) {
	shader_3D.use();
	glBindVertexArray(vao);

	shader_3D.set_uniform("uniform_colour", colour);
	shader_3D.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D.set_uniform("view", camera.matrix_view);
	shader_3D.set_uniform("model", utils::gen_model_matrix(transform));
	shader_3D.set_uniform("draw_filled", filled);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D.release();
}

void Circle_Renderer::draw_3D_shadow(const Camera& camera, const Transform& transform) {
	shader_3D_shadow.use();
	glBindVertexArray(vao);

	shader_3D_shadow.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_shadow.set_uniform("view", camera.matrix_view);
	shader_3D_shadow.set_uniform("model", utils::gen_model_matrix(transform));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D_shadow.release();
}

void Circle_Renderer::draw_multiple_3D_shadow(const Camera& camera, const std::vector<Transform>& transform_list) {
	shader_3D_shadow.use();
	glBindVertexArray(vao);

	shader_3D_shadow.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_shadow.set_uniform("view", camera.matrix_view);
	
	for (uint32_t i = 0; i < transform_list.size(); i++) {
		shader_3D_shadow.set_uniform("model", utils::gen_model_matrix(transform_list[i]));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
	shader_3D_shadow.release();
}

void Circle_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_2D.destroy();
	shader_3D.destroy();
	shader_3D_shadow.destroy();
}

void Triangle_Renderer::init() {
	shader_3D_coloured = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.uniform_colour.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 3, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Triangle_Renderer::draw_3D_coloured(const Camera& camera, const vec3& a, const vec3& b, const vec3& c, const vec4& colour) {
	shader_3D_coloured.use();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vec3 values[3] = { a, b, c };
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 3, &values);

	shader_3D_coloured.set_uniform("uniform_colour", colour);
	shader_3D_coloured.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_coloured.set_uniform("view", camera.matrix_view);
	shader_3D_coloured.set_uniform("model", mat4());

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	shader_3D_coloured.release();
}

void Triangle_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_3D_coloured.destroy();
}

void Quad_Renderer::init() {
	shader_2D = {
		"shaders/v.uniform_MP.glsl",
		"shaders/f.uniform_colour.glsl"
	};

	shader_3D_textured = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.texture.glsl"
	};

	shader_3D_coloured = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.uniform_colour.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh::quad_points_textured), &mesh::quad_points_textured, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Quad_Renderer::draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour) {
	shader_2D.use();
	glBindVertexArray(vao);

	shader_2D.set_uniform("uniform_colour", colour);
	shader_2D.set_uniform("projection", camera.matrix_projection_ortho);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void Quad_Renderer::draw_3D_coloured(const Camera& camera, const Transform& transform, const vec4& colour) {
	shader_3D_coloured.use();
	glBindVertexArray(vao);

	shader_3D_coloured.set_uniform("uniform_colour", colour);
	shader_3D_coloured.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_coloured.set_uniform("view", camera.matrix_view);
	shader_3D_coloured.set_uniform("model", utils::gen_model_matrix(transform));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D_coloured.release();
}

void Quad_Renderer::draw_multiple_3D_coloured(const Camera& camera, const std::vector<Transform>& transform_list, const vec4& colour) {	
	shader_3D_coloured.use();
	glBindVertexArray(vao);

	shader_3D_coloured.set_uniform("uniform_colour", colour);
	shader_3D_coloured.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_coloured.set_uniform("view", camera.matrix_view);

	for (uint32_t i = 0; i < transform_list.size(); i++) {
		shader_3D_coloured.set_uniform("model", utils::gen_model_matrix(transform_list[i]));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
	shader_3D_coloured.release();
}

void Quad_Renderer::draw_2D_textured(const Camera& camera, const vec2& position, const vec2& size, Texture& tex) {
	shader_2D.use();
	glBindVertexArray(vao);

	tex.use();

	shader_2D.set_uniform("projection", camera.matrix_projection_ortho);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void Quad_Renderer::draw_3D_textured(const Camera& camera, const Transform& transform, Texture& tex) {
	shader_3D_textured.use();
	glBindVertexArray(vao);

	tex.use();

	shader_3D_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_3D_textured.set_uniform("view", camera.matrix_view);
	shader_3D_textured.set_uniform("model", utils::gen_model_matrix(transform));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D_textured.release();
}

void Quad_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_2D.destroy();
	shader_3D_textured.destroy();
	shader_3D_coloured.destroy();
}

void Cube_Renderer::init() {
	shader = {
		"shaders/v.MVP_NORMALS.glsl",
		"shaders/f.DIFFUSE.glsl",
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(utils::mesh::cube_vertices_normals), &utils::mesh::cube_vertices_normals, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Cube_Renderer::draw(const Camera& camera, const vec3& position, const vec3& size, float rotation, const vec4& colour) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader.use();

	shader.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader.set_uniform("view", camera.matrix_view);
	shader.set_uniform("projection", camera.matrix_projection_persp);
	shader.set_uniform("uniform_colour", colour);
	shader.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader.release();
	glBindVertexArray(0);
}

void Cube_Renderer::draw_multiple(const Camera& camera, std::map<int, Transform>& transform_list, std::map<int, Vehicle_Attributes>& vehicle_attributes, std::map<int, Light>& lights) {
	shader.use();

	shader.set_uniform("view", camera.matrix_view);
	shader.set_uniform("projection", camera.matrix_projection_persp);
	shader.set_uniform("num_lights", static_cast<int>(lights.size()));

	int i = 0;
	for (std::map<int, Light>::iterator it = lights.begin(); it != lights.end(); ++it) {
		Light& light = it->second;
		std::string str = "lights[" + std::to_string(i) + "].position";
		shader.set_uniform(str.c_str(), light.position);
		str = "lights[" + std::to_string(i) + "].colour";
		shader.set_uniform(str.c_str(), light.colour);
		str = "lights[" + std::to_string(i) + "].intensity";
		shader.set_uniform(str.c_str(), light.intensity);
		i++;
	}

	for (std::map<int, Vehicle_Attributes>::iterator it = vehicle_attributes.begin(); it != vehicle_attributes.end(); ++it) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		shader.set_uniform("uniform_colour", it->second.colour);
		shader.set_uniform("model", utils::gen_model_matrix(transform_list[it->first].size, transform_list[it->first].position, transform_list[it->first].rotation));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	shader.release();
}

void Cube_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}

void Line_Renderer::init() {
	shader = {
		"shaders/v.uniform_MVP.glsl",
		"shaders/f.uniform_colour.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 3, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Line_Renderer::draw(const Camera& camera, const vec3& world_space_a, const vec3& world_space_b, const vec4& colour) {
	shader.use();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vec3 values[2] = { world_space_a, world_space_b };
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 2, &values);

	shader.set_uniform("view", camera.matrix_view);
	shader.set_uniform("projection", camera.matrix_projection_persp);
	shader.set_uniform("model", mat4());
	shader.set_uniform("uniform_colour", colour);
	
	glLineWidth(4.f);
	glDrawArrays(GL_LINES, 0, 2);
}

void Line_Renderer::draw_lineloop(const Camera& camera, const std::vector<vec3>& points, const vec4& colour) {
	shader.use();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * points.size(), &points.front());

	shader.set_uniform("view", camera.matrix_view);
	shader.set_uniform("projection", camera.matrix_projection_persp);
	shader.set_uniform("model", mat4());
	shader.set_uniform("uniform_colour", colour);

	glLineWidth(4.f);
	glDrawArrays(GL_LINE_LOOP, 0, points.size());
}


void Line_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}

void Model_Renderer::init() {
	shader_coloured = {
		"shaders/v.MVP_NORMALS.glsl",
		"shaders/f.DIFFUSE.glsl",
	};

	shader_textured = {
		"shaders/v.MVP_NORMALS_UVS.glsl",
		"shaders/f.DIFFUSE_TEXTURE.glsl",
	};
}

void Model_Renderer::draw_multiple_3D_textured(int n, Model& model, const Camera& camera, const std::vector<Transform>& transform_list, Texture& texture, std::map<int, Light>& lights) {
	shader_textured.use();
	shader_textured.set_uniform("view", camera.matrix_view);
	shader_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_textured.set_uniform("num_lights", static_cast<int>(lights.size()));

	int i = 0;
	for (std::map<int, Light>::iterator it = lights.begin(); it != lights.end(); ++it) {
		Light& light = it->second;
		std::string str = "lights[" + std::to_string(i) + "].position";
		shader_textured.set_uniform(str.c_str(), light.position);
		str = "lights[" + std::to_string(i) + "].colour";
		shader_textured.set_uniform(str.c_str(), light.colour);
		str = "lights[" + std::to_string(i) + "].intensity";
		shader_textured.set_uniform(str.c_str(), light.intensity);
		i++;
	}

	texture.use();

	for (int j = 0; j < n; j++) {
		shader_textured.set_uniform("model", gen_model_matrix(transform_list[j].size, transform_list[j].position, transform_list[j].rotation));

		for (uint32_t i = 0; i < model.meshes.size(); i++) {
			glBindVertexArray(model.meshes[i].vao);
			glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());
			glBindVertexArray(0);
		}
	}

	shader_textured.release();
}

void Model_Renderer::draw_multiple_3D_textured(int n, Model& model, const Camera& camera, std::map<int, std::vector<Transform>>& transform_list, Texture& texture, std::map<int, Light>& lights) {
	shader_textured.use();
	shader_textured.set_uniform("view", camera.matrix_view);
	shader_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_textured.set_uniform("num_lights", static_cast<int>(lights.size()));

	int i = 0;
	for (std::map<int, Light>::iterator it = lights.begin(); it != lights.end(); ++it) {
		Light& light = it->second;
		std::string str = "lights[" + std::to_string(i) + "].position";
		shader_textured.set_uniform(str.c_str(), light.position);
		str = "lights[" + std::to_string(i) + "].colour";
		shader_textured.set_uniform(str.c_str(), light.colour);
		str = "lights[" + std::to_string(i) + "].intensity";
		shader_textured.set_uniform(str.c_str(), light.intensity);
		i++;
	}

	texture.use();

	for (std::map<int, std::vector<Transform>>::iterator it = transform_list.begin(); it != transform_list.end(); ++it) {

		std::vector<Transform> t = it->second;

		for (uint32_t j = 0; j < t.size(); j++) {
			shader_textured.set_uniform("model", gen_model_matrix(t[j].size, t[j].position, t[j].rotation));

			for (uint32_t i = 0; i < model.meshes.size(); i++) {
				glBindVertexArray(model.meshes[i].vao);
				glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());
				glBindVertexArray(0);
			}
		}		
	}



	shader_textured.release();
}

void Model_Renderer::draw_3D_textured(Model& model, const Camera& camera, const Transform& transform, Texture& texture) {
	shader_textured.use();
	shader_textured.set_uniform("view", camera.matrix_view);
	shader_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	shader_textured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));

	texture.use();

	for (uint32_t i = 0; i < model.meshes.size(); i++) {
		glBindVertexArray(model.meshes[i].vao);
		glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());
		glBindVertexArray(0);
	}

	shader_textured.release();
}

void Model_Renderer::draw_3D_coloured(Model& model, const Camera& camera, const Transform& transform, const vec4& colour) {
	for (uint32_t i = 0; i < model.meshes.size(); i++) {
		glBindVertexArray(model.meshes[i].vao);
		shader_coloured.use();

		shader_coloured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));
		shader_coloured.set_uniform("view", camera.matrix_view);
		shader_coloured.set_uniform("projection", camera.matrix_projection_persp);
		shader_coloured.set_uniform("uniform_colour", colour);
		shader_coloured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
		glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());

		shader_coloured.release();
		glBindVertexArray(0);
	}
}

void Model_Renderer::destroy() {
	shader_coloured.destroy();
	shader_textured.destroy();
}

void Text_Renderer::init(const vec2& screen_resolution) {
	{
		FT_Library ft_lib;
		FT_Face ff;

		FT_Init_FreeType(&ft_lib);
		FT_New_Face(ft_lib, font.c_str(), 0, &ff);
		FT_Set_Pixel_Sizes(ff, 0, pixel_size);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (GLubyte c = 0; c < 128; ++c) {
			FT_Load_Char(ff, c, FT_LOAD_RENDER);

			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ff->glyph->bitmap.width, ff->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, ff->glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glyph_map.insert(std::pair<GLchar, Glyph>(
				c,
				{
					tex,
					ff->glyph->advance.x,
					vec2(static_cast<float>(ff->glyph->bitmap.width), static_cast<float>(ff->glyph->bitmap.rows)),
					vec2(static_cast<float>(ff->glyph->bitmap_left), static_cast<float>(ff->glyph->bitmap_top))
				}
			));
		}

		FT_Done_Face(ff);
		FT_Done_FreeType(ft_lib);
	}

	{ // GL Data
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	}

	{ // Shaders
		shader = {
			"shaders/v.text.glsl",
			"shaders/f.text.glsl"
		};

		shader.set_uniform("colour", vec4(1.f, 1.f, 1.f, 1.f));
		shader.set_uniform("projection", orthographic_matrix(screen_resolution, -1.f, 1.f, mat4()));
	}
}

void Text_Renderer::draw(const std::string& msg, const vec2& position, bool centered, const vec4& colour) {
	shader.use();
	shader.set_uniform("colour", colour);

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);

	float x = position.x;

	if (centered) {
		float x_ = x;
		for (GLchar c : msg)
			x_ += (glyph_map[c].next_glyph_offset >> 6);

		x -= ((x_ - x) * .5f);
	}

	for (GLchar c : msg) {
		Glyph g = glyph_map[c];

		float xPos = x + g.bearing_offset.x;
		float yPos = position.y - (g.glyph_size.y - g.bearing_offset.y);

		x += (g.next_glyph_offset >> 6);

		float vertices[6][4] = {
			{ xPos,                  yPos + g.glyph_size.y, 0.f, 0.f },
			{ xPos,                  yPos,                  0.f, 1.f },
			{ xPos + g.glyph_size.x, yPos,                  1.f, 1.f },
			{ xPos,                  yPos + g.glyph_size.y, 0.f, 0.f },
			{ xPos + g.glyph_size.x, yPos,                  1.f, 1.f },
			{ xPos + g.glyph_size.x, yPos + g.glyph_size.y, 1.f, 0.f }
		};

		glBindTexture(GL_TEXTURE_2D, g.data);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text_Renderer::destroy() {
	shader.destroy();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}