#pragma once
template <typename T>
class YSingleton {
	static T instance() const {
		static T _itx;
		return _itx;
	}
};