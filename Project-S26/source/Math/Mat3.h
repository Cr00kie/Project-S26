#pragma once

#include <cassert>
#include <algorithm>
#include <cmath>
#include <array>
#include <numbers>

#include "Vec2.h"
#include "Vec3.h"

template <typename T>
class Mat3
{
public:
	constexpr Mat3() : _m() {}
	constexpr explicit Mat3(T val)
	{
		_m[0] = val; _m[1] =   0; _m[2] =   0;
		_m[3] =   0; _m[4] = val; _m[5] =   0;
		_m[6] =   0; _m[7] =   0; _m[8] = val;
	}
	constexpr Mat3(const std::array<T, 9>& vals)
	{
		std::copy(vals.begin(), vals.end(), _m);
	}

	static constexpr Mat3 identity() { return Mat3(T(1)); }
	static constexpr Mat3 zero() { return Mat3(T(0)); }

	static constexpr Mat3 translation(T x, T y)
	{
		Mat3 res(T(1));

		res[2] = x;
		res[5] = y;

		return res;
	}
	static constexpr Mat3 translation(const Vec2<T>& v) { return translation(v.getX(), v.getY()); }

	static Mat3 rotation(T radians)
	{
		Mat3 res(T(1));

		res[0] = std::cos(radians);
		res[1] = -std::sin(radians);
		res[3] = std::sin(radians);
		res[4] = std::cos(radians);

		return res;
	}

	static constexpr Mat3 scale(T x, T y)
	{
		Mat3 res(T(1));

		res[0] = x;
		res[4] = y;

		return res;
	}
	static constexpr Mat3 scale(const Vec2<T>& v) { return scale(v.getX(), v.getY()); }

	constexpr Vec2<T> getTranslation() const { return { _m[2], _m[5] }; }
	constexpr T getRotation() const { return std::atan2(_m[3], _m[0]); }
	constexpr T getAngle() const { return getRotation() * (180.f / std::numbers::pi_v<float>); }

	constexpr T& operator[](int idx)
	{
		assert(idx >= 0 && idx < 9);
		return _m[idx];
	}
	constexpr const T& operator[](int idx) const
	{
		assert(idx >= 0 && idx < 9);
		return _m[idx];
	}

	constexpr T& operator()(int row, int col)
	{
		assert(row >= 0 && row < 3 && col >= 0 && col < 3);
		return _m[row * 3 + col];
	}

	constexpr const T& operator()(int row, int col) const
	{
		assert(row >= 0 && row < 3 && col >= 0 && col < 3);
		return _m[row * 3 + col];
	}

	constexpr Mat3& operator*=(const Mat3& rhs) { return *this = *this * rhs; }

	constexpr friend Mat3 operator* (const Mat3& lhs, const Mat3& rhs)
	{
		Mat3 res;

		res._m[0] = lhs._m[0] * rhs._m[0] + lhs._m[1] * rhs._m[3] + lhs._m[2] * rhs._m[6];
		res._m[1] = lhs._m[0] * rhs._m[1] + lhs._m[1] * rhs._m[4] + lhs._m[2] * rhs._m[7];
		res._m[2] = lhs._m[0] * rhs._m[2] + lhs._m[1] * rhs._m[5] + lhs._m[2] * rhs._m[8];

		res._m[3] = lhs._m[3] * rhs._m[0] + lhs._m[4] * rhs._m[3] + lhs._m[5] * rhs._m[6];
		res._m[4] = lhs._m[3] * rhs._m[1] + lhs._m[4] * rhs._m[4] + lhs._m[5] * rhs._m[7];
		res._m[5] = lhs._m[3] * rhs._m[2] + lhs._m[4] * rhs._m[5] + lhs._m[5] * rhs._m[8];

		res._m[6] = lhs._m[6] * rhs._m[0] + lhs._m[7] * rhs._m[3] + lhs._m[8] * rhs._m[6];
		res._m[7] = lhs._m[6] * rhs._m[1] + lhs._m[7] * rhs._m[4] + lhs._m[8] * rhs._m[7];
		res._m[8] = lhs._m[6] * rhs._m[2] + lhs._m[7] * rhs._m[5] + lhs._m[8] * rhs._m[8];

		return res;
	}

	constexpr friend Vec3<T> operator* (const Mat3& lhs, const Vec3<T>& rhs)
	{
		return 
		{ 
			lhs._m[0]*rhs.getX() + lhs._m[1]*rhs.getY() + lhs._m[2]*rhs.getZ(),
			lhs._m[3]*rhs.getX() + lhs._m[4]*rhs.getY() + lhs._m[5]*rhs.getZ(),
			lhs._m[6]*rhs.getX() + lhs._m[7]*rhs.getY() + lhs._m[8]*rhs.getZ(),
		};
	}
	constexpr friend Vec2<T> operator* (const Mat3& lhs, const Vec2<T>& rhs)
	{
		return 
		{ 
			lhs._m[0]*rhs.getX() + lhs._m[1]*rhs.getY() + lhs._m[2],
			lhs._m[3]*rhs.getX() + lhs._m[4]*rhs.getY() + lhs._m[5],
		};
	}

	constexpr Mat3& operator*=(T rhs)
	{
		_m[0] *= rhs;
		_m[1] *= rhs;
		_m[2] *= rhs;
		_m[3] *= rhs;
		_m[4] *= rhs;
		_m[5] *= rhs;
		_m[6] *= rhs;
		_m[7] *= rhs;
		_m[8] *= rhs;

		return *this;
	}

	constexpr Mat3& operator/=(T rhs)
	{
		assert(rhs != T(0));

		_m[0] /= rhs;
		_m[1] /= rhs;
		_m[2] /= rhs;
		_m[3] /= rhs;
		_m[4] /= rhs;
		_m[5] /= rhs;
		_m[6] /= rhs;
		_m[7] /= rhs;
		_m[8] /= rhs;

		return *this;
	}

	constexpr friend Mat3 operator*(Mat3 lhs, T rhs) { return lhs *= rhs; }

	constexpr friend Mat3 operator/(Mat3 lhs, T rhs) { return lhs /= rhs; }

	constexpr Mat3& operator+=(const Mat3& rhs)
	{
		_m[0] += rhs._m[0];
		_m[1] += rhs._m[1];
		_m[2] += rhs._m[2];
		_m[3] += rhs._m[3];
		_m[4] += rhs._m[4];
		_m[5] += rhs._m[5];
		_m[6] += rhs._m[6];
		_m[7] += rhs._m[7];
		_m[8] += rhs._m[8];

		return *this;
	}

	constexpr Mat3& operator-=(const Mat3& rhs)
	{
		_m[0] -= rhs._m[0];
		_m[1] -= rhs._m[1];
		_m[2] -= rhs._m[2];
		_m[3] -= rhs._m[3];
		_m[4] -= rhs._m[4];
		_m[5] -= rhs._m[5];
		_m[6] -= rhs._m[6];
		_m[7] -= rhs._m[7];
		_m[8] -= rhs._m[8];

		return *this;
	}

	constexpr friend Mat3 operator+(Mat3 lhs, const Mat3& rhs) { return lhs += rhs; }

	constexpr friend Mat3 operator-(Mat3 lhs, const Mat3& rhs) { return lhs -= rhs; }

	constexpr T determinant() const
	{
		return 
			(_m[0]*_m[4]*_m[8] + _m[1]*_m[5]*_m[6] + _m[2]*_m[3]*_m[7])
			-
			(_m[6]*_m[4]*_m[2] + _m[7]*_m[5]*_m[0] + _m[8]*_m[3]*_m[1]);
	}

	constexpr Mat3 transposed() const
	{
		Mat3 res;
		res._m[0] = _m[0];
		res._m[1] = _m[3];
		res._m[2] = _m[6];
		res._m[3] = _m[1];
		res._m[4] = _m[4];
		res._m[5] = _m[7];
		res._m[6] = _m[2];
		res._m[7] = _m[5];
		res._m[8] = _m[8];
		return res;
	}

	constexpr void transpose() { *this = transposed(); }

	Mat3 inversed() const {
		T det = determinant();
		assert(det != T(0));
		Mat3 adj;
		adj._m[0] = (_m[4] * _m[8] - _m[5] * _m[7]);
		adj._m[1] = -(_m[1] * _m[8] - _m[2] * _m[7]);
		adj._m[2] = (_m[1] * _m[5] - _m[2] * _m[4]);
		adj._m[3] = -(_m[3] * _m[8] - _m[5] * _m[6]);
		adj._m[4] = (_m[0] * _m[8] - _m[2] * _m[6]);
		adj._m[5] = -(_m[0] * _m[5] - _m[2] * _m[3]);
		adj._m[6] = (_m[3] * _m[7] - _m[4] * _m[6]);
		adj._m[7] = -(_m[0] * _m[7] - _m[1] * _m[6]);
		adj._m[8] = (_m[0] * _m[4] - _m[1] * _m[3]);
		return adj * (T(1) / det);
	}
	void inverse() { *this = inversed(); }

private:
	T _m[9];
};

using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;
using Mat3i = Mat3<int>;
