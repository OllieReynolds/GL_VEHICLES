#include "..\include\vehicle.h"

namespace simulation {

	void Vehicle::init(const mat4& projection_matrix) {
		shader = {
			"shaders/MVP_NORMALS.v.glsl",
			"shaders/DIFFUSE.f.glsl",
		};

		set_gl_buffer_data(sizeof(utils::data::cube_vertices_normals), &utils::data::cube_vertices_normals);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}


	void Vehicle::update(const maths::vec2& cursor_pos) {
		detected = false;
		
		other_vehicle_locations.erase(
			std::remove_if(other_vehicle_locations.begin(), other_vehicle_locations.end(), [&](vec2 v) -> bool {
			    return almost_equal(v, position.XZ(), 5.f);
		    }), other_vehicle_locations.end()
		);

		direction = polar_to_cartesian(to_radians(rotation));
		
		if (!point_quad_intersect(position.XZ(), -100, 100, 100, -100)) rotation += turning_force;

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

		direction = polar_to_cartesian(to_radians(rotation));
	}

	void Vehicle::draw(const mat4& view_matrix, const mat4& projection_matrix) {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		shader.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
		shader.set_uniform("view", view_matrix);
		shader.set_uniform("projection", projection_matrix);
		shader.set_uniform("uniform_colour", colour);
		shader.set_uniform("light_position", vec3{0.f, 30.f, 0.f});
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shader.release();
		glBindVertexArray(0);
	}

	void Vehicle::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
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