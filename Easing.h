﻿#pragma once
#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>


struct  Quad
{
	Vector2 center;
	Vector2 rad;
	Vector2 LT;
	Vector2 LB;
	Vector2 RT;
	Vector2 RB;
};

template<class T> T EasingFunc(T start, T movement, std::function<T(T)> func, T t) { return start + func(t) * movement; }

// Sine
template<class T> T EaseInSine(T x) { return 1 - cosf((x * M_PI) / 2); }

template<class T> T EaseOutSine(T x) { return sinf((x * M_PI) / 2); }

template<class T> T EaseInOutSine(T x) { return -(cosf(M_PI * x) - 1) / 2; }


// Cubic
template<class T> T EaseInCubic(T x) { return x * x * x; }

template<class T> T EaseOutCubic(T x) { return 1 - powf(1 - x, 3); }

template<class T> T EaseInOutCubic(T x) { return x < 0.5 ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2; }

// Quint
template<class T> T EaseInQuint(T x) { return x * x * x * x * x; }

template<class T> T EaseOutQuint(T x) { return 1 - powf(1 - x, 5); }

template<class T> T EaseInOutQuint(T x) { return x < 0.5 ? 16 * x * x * x * x * x : 1 - powf(-2 * x + 2, 5) / 2; }



// Circ
template<class T> T EaseInCirc(T x) { return 1 - sqrtf(1 - powf(x, 2)); }

template<class T> T EaseOutCirc(T x) { return sqrtf(1 - powf(x - 1, 2)); }

template<class T> T EaseInOutCirc(T x) { return x < 0.5 ? (1 - sqrtf(1 - powf(2 * x, 2))) / 2 : (sqrtf(1 - powf(-2 * x + 2, 2)) + 1) / 2; }



// Elastic
template<class T> T EaseInElastic(T x) {
	const T c4 = float((2 * M_PI) / 3);

	return x == 0 ? 0 : x == 1 ? 1 : -powf(2, 10 * x - 10) * sinf((x * 10 - 10.75f) * c4);
}

template<class T> T EaseOutElastic(T x) {
	const T c4 = float((2 * M_PI) / 3);

	return x == 0 ? 0 : x == 1 ? 1 : powf(2, -10 * x) * sinf((x * 10 - 0.75f) * c4) + 1;
}

template<class T> T EaseInOutElastic(T x) {
	const T c5 = float((2 * M_PI) / 4.5);

	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5 ? -(powf(2, 20 * x - 10) * sinf((20 * x - 11.125) * c5)) / 2 : (powf(2, -20 * x + 10) * sinf((20 * x - 11.125f) * c5)) / 2 + 1;
}



// Quad
template<class T> T EaseInQuad(T x) { return x * x; }

template<class T> T EaseOutQuad(T x) { return 1 - (1 - x) * (1 - x); }

template<class T> T EaseInOutQuad(T x) { return x < 0.5 ? 2 * x * x : 1 - powf(-2 * x + 2, 2) / 2; }



// Quart
template<class T> T EaseInQuart(T x) { return x * x * x * x; }

template<class T> T EaseOutQuart(T x) { return 1 - powf(1 - x, 4); }

template<class T> T EaseInOutQuart(T x) { return x < 0.5 ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2; }



// Expo
template<class T> T EaseInExpo(T x) { return x == 0 ? 0 : powf(2, 10 * x - 10); }

//template<class T> T EaseOutExpo(T x) { return x == = 1 ? 1 : 1 - powf(2, -10 * x); }

//template<class T> T EaseInOutExpo(T x) { return x == = 0 ? 0 : x == = 1 ? 1 : x < 0.5 ? powf(2, 20 * x - 10) / 2 : (2 - powf(2, -20 * x + 10)) / 2; }



// Back
template<class T> T EaseInBack(T x) {
	const T c1 = 1.70158;
	const T c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

template<class T> T EaseOutBack(T x) {
	const T c1 = 1.70158;
	const T c3 = c1 + 1;

	return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
}

template<class T> T EaseInOutBack(T x) {
	const T c1 = 1.70158;
	const T c2 = c1 * 1.525;

	return x < 0.5 ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}



// Bounce
template<class T> T EaseInBounce(T x) { return 1 - EaseOutBounce(1 - x); }

template<class T> T EaseOutBounce(T x) {
	const T n1 = 7.5625;
	const T d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

template<class T> T EaseInOutBounce(T x) { return x < 0.5 ? (1 - EaseOutBounce(1 - 2 * x)) / 2 : (1 + EaseOutBounce(2 * x - 1)) / 2; }