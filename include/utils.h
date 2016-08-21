#pragma once

#include <chrono>
#include <random>

namespace utils {
	static const maths::vec2 resolution = {1366.f, 768.f};
	static const maths::mat4 resolution_matrix = maths::orthographic_perspective(resolution.x, resolution.y, -1.f, 1.f);

	static float elapsed_time() {
		using namespace std::chrono;
		static time_point<steady_clock> start = steady_clock::now();
		return duration_cast<duration<float>>(steady_clock::now() - start).count();
	}

	static float gen_random(float min = 0.f, float max = 10.f) {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}
}