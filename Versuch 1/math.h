#ifndef __MATH_H__
#define __MATH_H__

template<T>
struct vec2_base {
	union { T x, u; };
	union { T y, v; };

public:
	vec2_base() : x((T)0.0), y((T)0.0) {

	}

	vec2_base(T x, T y) {
		this->x = x;
		this->y = y;
	}
};

typedef vec2_base<float> vec2;

#endif