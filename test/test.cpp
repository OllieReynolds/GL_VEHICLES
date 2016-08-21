#include <iostream>

#include "..\include\maths.h"

namespace test {
	int main() {
		maths::vec2 a = {0.f, 1.f};
		maths::vec2 b = {1.f, 0.f};

		std::cout << a.x << std::endl;

		system("pause");
	}
}