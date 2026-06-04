#pragma once

#include <cmath>
#include <ostream>
#include <cassert>
#include <limits>
#include <algorithm>

template<typename T>
class Vec2
{
public:
	constexpr Vec2() : _x(), _y() {}
	constexpr Vec2(T x, T y) : _x(x), _y(y) {}
	constexpr static Vec2 zero() { return { T(0), T(0) }; }
	constexpr static Vec2 one() { return { T(1), T(1) }; }

	constexpr Vec2 operator-() const { return { -_x, -_y }; }

	constexpr Vec2& operator+= (const Vec2& rhs)
	{
		_x += rhs._x; _y += rhs._y;
		return *this;
	}
	constexpr Vec2& operator-= (const Vec2& rhs)
	{
		_x -= rhs._x; _y -= rhs._y;
		return *this;
	}
	constexpr Vec2& operator*= (T rhs)
	{
		_x *= rhs; _y *= rhs;
		return *this;
	}
	constexpr Vec2& operator/= (T rhs)
	{
		assert(rhs != T(0));
		_x /= rhs; _y /= rhs;
		return *this;
	}
	constexpr friend Vec2 operator+ (Vec2 lhs, const Vec2& rhs) { return lhs += rhs; }
	constexpr friend Vec2 operator- (Vec2 lhs, const Vec2& rhs) { return lhs -= rhs; }
	
	constexpr friend T operator* (const Vec2& lhs, const Vec2& rhs) { return dot(lhs, rhs); }
	constexpr friend T dot(const Vec2& lhs, const Vec2& rhs) { return lhs._x * rhs._x + lhs._y * rhs._y; }
	constexpr friend T cross(const Vec2& lhs, const Vec2& rhs) { return lhs._x * rhs._y - lhs._y * rhs._x; }

	constexpr friend Vec2 operator* (Vec2 lhs, T rhs) { return lhs *= rhs; }
	constexpr friend Vec2 operator* (T lhs, Vec2 rhs) { return rhs *= lhs; }
	constexpr friend Vec2 operator/ (Vec2 lhs, T rhs) { return lhs /= rhs; }

	constexpr friend bool operator== (const Vec2& lhs, const Vec2& rhs) { return (lhs._x == rhs._x) && (lhs._y == rhs._y); }
	constexpr friend bool operator!= (const Vec2 & lhs, const Vec2 & rhs) { return !(lhs == rhs); }

	friend std::ostream& operator<<(std::ostream& os, const Vec2& v)
	{
		return os << '(' << v._x << ", " << v._y << ')';
	}

	constexpr void set(T x, T y) { _x = x; _y = y; }
	constexpr void setX(T x) { _x = x; }
	constexpr void setY(T y) { _y = y; }
	constexpr T getX() const { return _x; }
	constexpr T getY() const { return _y; }

	constexpr T lengthSqr() const { return _x * _x + _y * _y; }
	T length() const { return static_cast<T>(std::sqrt(static_cast<double>(lengthSqr()))); }

	friend T distanceSqr(const Vec2& a, const Vec2& b) { return (b - a).lengthSqr(); }
	friend T distance(const Vec2& a, const Vec2& b) { return (b - a).length(); }

	Vec2 normalized() const { 
		T l = length();
		assert(l > std::numeric_limits<T>::epsilon());
		return *this / l; 
	}
	void normalize()
	{
		T l = length();
		assert(l > std::numeric_limits<T>::epsilon());
		*this /= l;
	}

	friend Vec2 abs(const Vec2& v) { return { std::abs(v._x), std::abs(v._y) }; }

	constexpr friend Vec2 lerp(const Vec2& a, const Vec2& b, T t) { return a + (b - a) * t; }

	constexpr friend Vec2 clamp(const Vec2& v, const Vec2& lo, const Vec2& hi)
	{
		return { std::clamp(v._x, lo._x, hi._x),
				 std::clamp(v._y, lo._y, hi._y) };
	}

	void rotate(float rotation)
	{
		_x = _x * std::cos(rotation) - _y * std::sin(rotation);
		_y = _y * std::sin(rotation) + _y * std::cos(rotation);
	}

private:
	T _x;
	T _y;
};

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;