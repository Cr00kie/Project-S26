#pragma once

#include "Vec2.h"

template<typename T>
class Vec3
{
public:
	constexpr Vec3() : _x(), _y(), _z() {}
	constexpr Vec3(T x, T y) : _x(x), _y(y), _z(T(0)) {}
	constexpr Vec3(T x, T y, T z) : _x(x), _y(y), _z(z) {}
	constexpr Vec3(const Vec2<T>& other) : _x(other.getX()), _y(other.getY()), _z(T(1)) {}
	constexpr static Vec3 zero() { return { T(0), T(0), T(0)}; }
	constexpr static Vec3 one() { return { T(1), T(1), T(1)}; }

	constexpr Vec3 operator-() const { return { -_x, -_y, -_z }; }

	constexpr Vec3& operator+= (const Vec3& rhs)
	{
		_x += rhs._x; _y += rhs._y; _z += rhs._z;
		return *this;
	}
	constexpr Vec3& operator-= (const Vec3& rhs)
	{
		_x -= rhs._x; _y -= rhs._y; _z -= rhs._z;
		return *this;
	}
	constexpr Vec3& operator*= (T rhs)
	{
		_x *= rhs; _y *= rhs; _z *= rhs;
		return *this;
	}
	constexpr Vec3& operator/= (T rhs)
	{
		assert(rhs != T(0));
		_x /= rhs; _y /= rhs; _z *= rhs;
		return *this;
	}

	constexpr friend Vec3 operator+ (Vec3 lhs, const Vec3& rhs) { return lhs += rhs; }
	constexpr friend Vec3 operator- (Vec3 lhs, const Vec3& rhs) { return lhs -= rhs; }

	constexpr friend T operator* (const Vec3& lhs, const Vec3& rhs) { return dot(lhs, rhs); }
	constexpr friend T dot(const Vec3& lhs, const Vec3& rhs) { return lhs._x * rhs._x + lhs._y * rhs._y + lhs._z * rhs._z; }
	constexpr friend Vec3 cross(const Vec3& lhs, const Vec3& rhs) { 
		return {
			lhs._y * rhs._z - lhs._z * rhs._y,
			lhs._z * rhs._x - lhs._x * rhs._z,
			lhs._x * rhs._y - lhs._y * rhs._x
		};
	}

	constexpr friend Vec3 operator* (Vec3 lhs, T rhs) { return lhs *= rhs; }
	constexpr friend Vec3 operator* (T lhs, Vec3 rhs) { return rhs *= lhs; }
	constexpr friend Vec3 operator/ (Vec3 lhs, T rhs) { return lhs /= rhs; }

	constexpr friend bool operator== (const Vec3& lhs, const Vec3& rhs) { return (lhs._x == rhs._x) && (lhs._y == rhs._y) && (lhs._z == rhs._z); }
	constexpr friend bool operator!= (const Vec3& lhs, const Vec3& rhs) { return !(lhs == rhs); }

	friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
	{
		return os << '(' << v._x << ", " << v._y << ", " << v._z << ')';
	}

	constexpr void set(T x, T y, T z) { _x = x; _y = y;  _z = z; }
	constexpr void setX(T x) { _x = x; }
	constexpr void setY(T y) { _y = y; }
	constexpr void setZ(T z) { _z = z; }
	constexpr T getX() const { return _x; }
	constexpr T getY() const { return _y; }
	constexpr T getZ() const { return _z; }

	constexpr T lengthSqr() const { return _x * _x + _y * _y + _z * _z; }
	T length() const { return static_cast<T>(std::sqrt(static_cast<double>(lengthSqr()))); }

	friend T distanceSqr(const Vec3& a, const Vec3& b) { return (b - a).lengthSqr(); }
	friend T distance(const Vec3& a, const Vec3& b) { return (b - a).length(); }

	Vec3 normalized() const
	{
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

	friend Vec3 abs(const Vec3& v) { return { std::abs(v._x), std::abs(v._y), std::abs(v._z)}; }

	constexpr friend Vec3 lerp(const Vec3& a, const Vec3& b, T t) { return a + (b - a) * t; }

	constexpr friend Vec3 clamp(const Vec3& v, const Vec3& lo, const Vec3& hi)
	{
		return { std::clamp(v._x, lo._x, hi._x),
				 std::clamp(v._y, lo._y, hi._y),	
				 std::clamp(v._z, lo._z, hi._z)	
		};
	}

	constexpr Vec2<T> toVec2() const
	{
		return { _x,  _y };
	}

private:
	T _x;
	T _y;
	T _z;
};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;