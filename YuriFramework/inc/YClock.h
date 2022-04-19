#pragma once
#include <chrono>

class YClock {
public:
	YClock() {
		reset();
	}

	void reset() {
		mLastTime = std::chrono::high_resolution_clock::now();
	};

	float delta() {
		std::chrono::steady_clock::time_point t = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> d = t - mLastTime;
		return d.count();
	};
private:
	std::chrono::steady_clock::time_point mLastTime;
};