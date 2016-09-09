#include "..\include\simulation.h"



// Data Transformation
namespace logic {
	void move_vehicle() {
		maths::vec2 o = light_FOV_L.position;
		maths::vec2 p = box.position;
		float th = light_FOV_L.theta * 0.5f;
		float rot = light_FOV_L.rotation;

		{ // CHECK LEFT BEAM INTERSECTIONS
			float mag = maths::magnitude(maths::normalise(light_FOV_L.size));

			float semi_thetaA = (std::_Pi / 180.f) * (rot + th);
			float semi_thetaB = (std::_Pi / 180.f) * (rot - th);
			
			std::complex<float> cA = std::polar(mag, semi_thetaA);
			std::complex<float> cB = std::polar(mag, semi_thetaB);

			maths::vec2 a = maths::vec2{
			    light_FOV_L.size.x * cA.real(),
				light_FOV_L.size.y * cA.imag()
			};
					
			maths::vec2 b = maths::vec2{
				o.x + light_FOV_L.size.x * cB.real(),
				o.y + light_FOV_L.size.y * cB.imag()
			};
		
			if (intersections::point_segment_intersect(p, a, o, b))
				speed = 0.f;
		}
		

		body.position.x += speed;
		light_FOV_L.position.x += speed;
		light_FOV_R.position.x += speed;
	}
}

namespace simulation {
	void init() {
		{ // INIT BRAITENBERG BEHICLE

			graphics::shader = {
				"shaders/polygon.v.glsl",
				"shaders/polygon.f.glsl",
				"shaders/polygon.g.glsl"
			};

			graphics::shader.set_uniform("projection", resolution_matrix);

			// Create and bind GL context
			glGenVertexArrays(1, &graphics::VAO);
			glBindVertexArray(graphics::VAO);
		
			// UBO to store uniform data in
			glGenBuffers(1, &graphics::UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::UBO);

			{ // Allocate storage for UBO and link to gl binding point
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), NULL, GL_STATIC_DRAW);
				GLuint bind_index = 0;
				glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, graphics::UBO);
				glUniformBlockBinding(graphics::shader.program, glGetUniformBlockIndex(graphics::shader.program, "Sectors"), bind_index);
			}

			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void update() {
		//logic::spin_sensors();
		logic::move_vehicle();
		
	}

	void draw(const float fps) {
		{ // DRAW BRAITENBEG VEHICLE

			// Bind GL context
			graphics::shader.use();
			glBindVertexArray(graphics::VAO);
			glBindBuffer(GL_UNIFORM_BUFFER, graphics::UBO);

			{ // DRAW BODY
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), &logic::body, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW LEFT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), &logic::light_FOV_L, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW RIGHT SENSOR
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), &logic::light_FOV_R, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			{ // DRAW OBSTACLE
				glBufferData(GL_UNIFORM_BUFFER, sizeof(logic::Sector), &logic::box, GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, 1);
			}

			// Unbind GL context
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindVertexArray(0);
			graphics::shader.release();
		}
	}

	void destroy() {
		graphics::shader.destroy();
		glDeleteVertexArrays(1, &graphics::VAO);
	}
}
