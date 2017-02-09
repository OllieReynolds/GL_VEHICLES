#include "..\include\inactivity_timer.h"

Inactivity_Timer::Inactivity_Timer() : start_milliseconds(10.1f), remaining_milliseconds(10.1f) { 

}

void Inactivity_Timer::init(map<int, Transform> new_transforms) {
	old_transforms = new_transforms;
}

void Inactivity_Timer::update(map<int, Transform> new_transforms) {
	check_inactivity(new_transforms);
	remaining_milliseconds -= 0.1f;
}

void Inactivity_Timer::reset() {
	remaining_milliseconds = start_milliseconds;
}

void Inactivity_Timer::check_inactivity(map<int, Transform> new_transforms) {
	map<int, Transform>::iterator old_transform_iterator = old_transforms.begin();
	map<int, Transform>::iterator new_transform_iterator = new_transforms.begin();

	while (new_transform_iterator != new_transforms.end()) {
		// Vehicle movement so reset inactivity timer, no point checking further

		vec2 p0 = new_transform_iterator->second.position.XZ();
		vec2 p1 = old_transform_iterator->second.position.XZ();

		bool changed = !almost_equal(p0, p1, 0.2f);

		if (changed) {
			remaining_milliseconds = start_milliseconds;
			break;
		}

		++old_transform_iterator;
		++new_transform_iterator;
	}

	old_transforms = new_transforms;
}