#pragma once

#include "Vector.h"

namespace math
{
	template <typename T> class Quaternion final
	{
	public:
#if defined(__SSE__) || defined(_M_X64) || _M_IX86_FP >= 1 || defined(__ARM_NEON__)
		alignas(std::is_same_v<T, float> ? 4 * sizeof(T) : sizeof(T))
#endif
#if (defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2) || (defined(__ARM_NEON__) && defined(__aarch64__))
			alignas(std::is_same_v<T, double> ? 4 * sizeof(T) : sizeof(T))
#endif
			T v[4];

		[[nodiscard]] auto& operator[](const std::size_t index) noexcept { return v[index]; }
		[[nodiscard]] constexpr auto operator[](const std::size_t index) const noexcept { return v[index]; }

		[[nodiscard]] auto& x() noexcept { return v[0]; }
		[[nodiscard]] constexpr auto x() const noexcept { return v[0]; }

		[[nodiscard]] auto& y() noexcept { return v[1]; }
		[[nodiscard]] constexpr auto y() const noexcept { return v[1]; }

		[[nodiscard]] auto& z() noexcept { return v[2]; }
		[[nodiscard]] constexpr auto z() const noexcept { return v[2]; }

		[[nodiscard]] auto& w() noexcept { return v[3]; }
		[[nodiscard]] constexpr auto w() const noexcept { return v[3]; }

		[[nodiscard]] auto getNorm() const noexcept
		{
			constexpr T n = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
			if (n == T(1)) // already normalized
				return T(1);

			return std::sqrt(n);
		}

		void getRotation(T& angle, math::Vector<T, 3>& axis) const noexcept
		{
			angle = T(2) * std::acos(v[3]);
			const auto s = std::sqrt(T(1) - v[3] * v[3]);
			if (s <= std::numeric_limits<T>::epsilon()) // too close to zero
			{
				axis.v[0] = v[0];
				axis.v[1] = v[1];
				axis.v[2] = v[2];
			}
			else
			{
				axis.v[0] = v[0] / s;
				axis.v[1] = v[1] / s;
				axis.v[2] = v[2] / s;
			}
		}

		void lerp(const Quaternion& q1, const Quaternion& q2, T t) noexcept
		{
			*this = q1 * (T(1) - t) + (q2 * t);
		}
	};

	template <class T>
	constexpr auto identityQuaternion = Quaternion<T>{ T(0), T(0), T(0), T(1) };

	template <typename T>
	constexpr void setIdentity(Quaternion<T>& quat) noexcept
	{
		quat.v = { T(0), T(0), T(0), T(1) };
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator==(const Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		return quat1.v[0] == quat2.v[0] &&
			quat1.v[1] == quat2.v[1] &&
			quat1.v[2] == quat2.v[2] &&
			quat1.v[3] == quat2.v[3];
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator!=(const Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		return quat1.v[0] != quat2.v[0] ||
			quat1.v[1] != quat2.v[1] ||
			quat1.v[2] != quat2.v[2] ||
			quat1.v[3] != quat2.v[3];
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator+(const Quaternion<T>& quat) noexcept
	{
		return quat;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator-(const Quaternion<T>& quat) noexcept
	{
		return Quaternion<T>{-quat.v[0], -quat.v[1], -quat.v[2], -quat.v[3]};
	}

	template <typename T>
	constexpr void negate(Quaternion<T>& quat) noexcept
	{
		quat.v[0] = -quat.v[0];
		quat.v[1] = -quat.v[1];
		quat.v[2] = -quat.v[2];
		quat.v[3] = -quat.v[3];
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator+(const Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		return Quaternion<T>{
			quat1.v[0] + quat2.v[0],
				quat1.v[1] + quat2.v[1],
				quat1.v[2] + quat2.v[2],
				quat1.v[3] + quat2.v[3]
		};
	}

	template <typename T>
	constexpr auto& operator+=(Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		quat1.v[0] += quat2.v[0];
		quat1.v[1] += quat2.v[1];
		quat1.v[2] += quat2.v[2];
		quat1.v[3] += quat2.v[3];

		return quat1;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator-(const Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		return Quaternion<T>{
			quat1.v[0] - quat2.v[0],
				quat1.v[1] - quat2.v[1],
				quat1.v[2] - quat2.v[2],
				quat1.v[3] - quat2.v[3]
		};
	}

	template <typename T>
	constexpr auto& operator-=(Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		quat1.v[0] -= quat2.v[0];
		quat1.v[1] -= quat2.v[1];
		quat1.v[2] -= quat2.v[2];
		quat1.v[3] -= quat2.v[3];

		return quat1;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator*(const Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		return Quaternion<T>{
			quat1.v[0] * quat2.v[3] + quat1.v[1] * quat2.v[2] - quat1.v[2] * quat2.v[1] + quat1.v[3] * quat2.v[0],
				-quat1.v[0] * quat2.v[2] + quat1.v[1] * quat2.v[3] + quat1.v[2] * quat2.v[0] + quat1.v[3] * quat2.v[1],
				quat1.v[0] * quat2.v[1] - quat1.v[1] * quat2.v[0] + quat1.v[2] * quat2.v[3] + quat1.v[3] * quat2.v[2],
				-quat1.v[0] * quat2.v[0] - quat1.v[1] * quat2.v[1] - quat1.v[2] * quat2.v[2] + quat1.v[3] * quat2.v[3]
		};
	}

	template <typename T>
	constexpr auto& operator*=(Quaternion<T>& quat1,
		const Quaternion<T>& quat2) noexcept
	{
		quat1.v = {
		quat1.v[0] * quat2.v[3] + quat1.v[1] * quat2.v[2] - quat1.v[2] * quat2.v[1] + quat1.v[3] * quat2.v[0],
		-quat1.v[0] * quat2.v[2] + quat1.v[1] * quat2.v[3] + quat1.v[2] * quat2.v[0] + quat1.v[3] * quat2.v[1],
		quat1.v[0] * quat2.v[1] - quat1.v[1] * quat2.v[0] + quat1.v[2] * quat2.v[3] + quat1.v[3] * quat2.v[2],
		-quat1.v[0] * quat2.v[0] - quat1.v[1] * quat2.v[1] - quat1.v[2] * quat2.v[2] + quat1.v[3] * quat2.v[3]
		};

		return quat1;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator*(const Quaternion<T>& quat,
		const T scalar) noexcept
	{
		return Quaternion<T>{
			quat.v[0] * scalar,
				quat.v[1] * scalar,
				quat.v[2] * scalar,
				quat.v[3] * scalar
		};
	}

	template <typename T>
	constexpr auto& operator*=(Quaternion<T>& quat,
		const T scalar) noexcept
	{
		quat.v[0] *= scalar;
		quat.v[1] *= scalar;
		quat.v[2] *= scalar;
		quat.v[3] *= scalar;

		return quat;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator/(const Quaternion<T>& quat,
		const T scalar) noexcept
	{
		return Quaternion<T>{
			quat.v[0] / scalar,
				quat.v[1] / scalar,
				quat.v[2] / scalar,
				quat.v[3] / scalar
		};
	}

	template <typename T>
	constexpr auto& operator/=(Quaternion<T>& quat,
		const T scalar) noexcept
	{
		quat.v[0] /= scalar;
		quat.v[1] /= scalar;
		quat.v[2] /= scalar;
		quat.v[3] /= scalar;

		return quat;
	}

	template <typename T>
	[[nodiscard]] auto norm(const Quaternion<T>& quat) noexcept
	{
		return std::sqrt(quat.v[0] * quat.v[0] + quat.v[1] * quat.v[1] + quat.v[2] * quat.v[2] + quat.v[3] * quat.v[3]);
	}

	template <typename T>
	constexpr void invert(Quaternion<T>& quat) noexcept
	{
		const auto normSquared = quat.v[0] * quat.v[0] + quat.v[1] * quat.v[1] + quat.v[2] * quat.v[2] + quat.v[3] * quat.v[3];
		quat.v[0] = -quat.v[0] / normSquared;
		quat.v[1] = -quat.v[1] / normSquared;
		quat.v[2] = -quat.v[2] / normSquared;
		quat.v[3] = quat.v[3] / normSquared;
	}

	template <typename T>
	[[nodiscard]] auto inverse(const Quaternion<T>& quat) noexcept
	{
		const auto normSquared = quat.v[0] * quat.v[0] + quat.v[1] * quat.v[1] + quat.v[2] * quat.v[2] + quat.v[3] * quat.v[3];
		return Quaternion<T>{
			-quat.v[0] / normSquared,
				-quat.v[1] / normSquared,
				-quat.v[2] / normSquared,
				quat.v[3] / normSquared
		};
	}

	template <typename T>
	constexpr void conjugate(Quaternion<T>& quat) noexcept
	{
		quat.v[0] = -quat.v[0];
		quat.v[1] = -quat.v[1];
		quat.v[2] = -quat.v[2];
	}

	template <typename T>
	constexpr auto conjugated(const Quaternion<T>& quat) noexcept
	{
		return Quaternion<T>{-quat.v[0], -quat.v[1], -quat.v[2], quat.v[3]};
	}

	template <typename T>
	void setRotation(Quaternion<T>& quat,
		const T angle,
		const math::Vector<T, 3>& axis) noexcept
	{
		const auto normalizedAxis = normalized(axis);

		const auto cosine = std::cos(angle / T(2));
		const auto sine = std::sin(angle / T(2));

		quat.v[0] = normalizedAxis.v[0] * sine;
		quat.v[1] = normalizedAxis.v[1] * sine;
		quat.v[2] = normalizedAxis.v[2] * sine;
		quat.v[3] = cosine;
	}

	template <typename T>
	void setRotationX(Quaternion<T>& quat,
		const T angle) noexcept
	{
		quat.v[0] = std::sin(angle / T(2));
		quat.v[1] = T(0);
		quat.v[2] = T(0);
		quat.v[3] = std::cos(angle / T(2));
	}

	template <typename T>
	void setRotationY(Quaternion<T>& quat,
		const T angle) noexcept
	{
		quat.v[0] = T(0);
		quat.v[1] = std::sin(angle / T(2));
		quat.v[2] = T(0);
		quat.v[3] = std::cos(angle / T(2));
	}

	template <typename T>
	void setRotationZ(Quaternion<T>& quat,
		const T angle) noexcept
	{
		quat.v[0] = T(0);
		quat.v[1] = T(0);
		quat.v[2] = std::sin(angle / T(2));
		quat.v[3] = std::cos(angle / T(2));
	}

	template <typename T>
	void rotateVector(math::Vector<T, 3>& vec,
		const Quaternion<T>& quat) noexcept
	{
		constexpr math::Vector<T, 3> q{ quat.v[0], quat.v[1], quat.v[2] };
		const auto t = T(2) * cross(q, vec);
		vec += (quat.v[3] * t) + cross(q, t);
	}

	template <typename T>
	[[nodiscard]] auto rotatedVector(const math::Vector<T, 3>& vec,
		const Quaternion<T>& quat) noexcept
	{
		constexpr math::Vector<T, 3> q{ quat.v[0], quat.v[1], quat.v[2] };
		const auto t = T(2) * cross(q, vec);
		return vec + (quat.v[3] * t) + cross(q, t);
	}

	template <typename T>
	[[nodiscard]] auto operator*(const Quaternion<T>& quat,
		const math::Vector<T, 3>& vec) noexcept
	{
		return rotateVector(vec, quat);
	}

	template <typename T>
	[[nodiscard]] auto getRightVector(const Quaternion<T>& quat) noexcept
	{
		return rotateVector(math::Vector<T, 3>{T(1), T(0), T(0)}, quat);
	}

	template <typename T>
	[[nodiscard]] auto getUpVector(const Quaternion<T>& quat) noexcept
	{
		return rotateVector(math::Vector<T, 3>{T(0), T(1), T(0)}, quat);
	}

	template <typename T>
	[[nodiscard]] auto getForwardVector(const Quaternion<T>& quat) noexcept
	{
		return rotateVector(math::Vector<T, 3>{T(0), T(0), T(1)}, quat);
	}

	template <typename T>
	[[nodiscard]] auto getEulerAngles(const Quaternion<T>& quat) noexcept
	{
		return math::Vector<T, 3>{
			std::atan2(T(2)* (quat.v[1] * quat.v[2] + quat.v[3] * quat.v[0]),
				quat.v[3] * quat.v[3] - quat.v[0] * quat.v[0] - quat.v[1] * quat.v[1] + quat.v[2] * quat.v[2]),
				std::asin(-T(2) * (quat.v[0] * quat.v[2] - quat.v[3] * quat.v[1])),
				std::atan2(T(2)* (quat.v[0] * quat.v[1] + quat.v[3] * quat.v[2]),
					quat.v[3] * quat.v[3] + quat.v[0] * quat.v[0] - quat.v[1] * quat.v[1] - quat.v[2] * quat.v[2])
		};
	}

	template <typename T>
	[[nodiscard]] auto getEulerAngleX(const Quaternion<T>& quat) noexcept
	{
		return std::atan2(T(2) * (quat.v[1] * quat.v[2] + quat.v[3] * quat.v[0]),
			quat.v[3] * quat.v[3] - quat.v[0] * quat.v[0] - quat.v[1] * quat.v[1] + quat.v[2] * quat.v[2]);
	}

	template <typename T>
	[[nodiscard]] auto getEulerAngleY(const Quaternion<T>& quat) noexcept
	{
		return std::asin(T(-2) * (quat.v[0] * quat.v[2] - quat.v[3] * quat.v[1]));
	}

	template <typename T>
	[[nodiscard]] auto getEulerAngleZ(const Quaternion<T>& quat) noexcept
	{
		return std::atan2(T(2) * (quat.v[0] * quat.v[1] + quat.v[3] * quat.v[2]),
			quat.v[3] * quat.v[3] + quat.v[0] * quat.v[0] - quat.v[1] * quat.v[1] - quat.v[2] * quat.v[2]);
	}

	template <typename T>
	void setEulerAngles(Quaternion<T>& quat,
		const math::Vector<T, 3>& angles) noexcept
	{
		const auto roll = angles.v[0] / T(2);
		const auto sr = std::sin(roll);
		const auto cr = std::cos(roll);

		const auto pitch = angles.v[1] / T(2);
		const auto sp = std::sin(pitch);
		const auto cp = std::cos(pitch);

		const auto yaw = angles.v[2] / T(2);
		const auto sy = std::sin(yaw);
		const auto cy = std::cos(yaw);

		const auto cpcy = cp * cy;
		const auto spcy = sp * cy;
		const auto cpsy = cp * sy;
		const auto spsy = sp * sy;

		quat.v[0] = sr * cpcy - cr * spsy;
		quat.v[1] = cr * spcy + sr * cpsy;
		quat.v[2] = cr * cpsy - sr * spcy;
		quat.v[3] = cr * cpcy + sr * spsy;
	}
}

#if defined(__SSE__) || defined(_M_X64) || _M_IX86_FP >= 1
#  include <xmmintrin.h>
#endif

#if defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2
#  include <emmintrin.h>
#endif

#ifdef __SSE3__
#  include <pmmintrin.h>
#endif // __SSE3__

namespace math
{
#if defined(__SSE__) || defined(_M_X64) || _M_IX86_FP >= 1
	template <>
	inline auto operator-(const Quaternion<float>& quat) noexcept
	{
		Quaternion<float> result;
		const auto z = _mm_setzero_ps();
		_mm_store_ps(result.v, _mm_sub_ps(z, _mm_load_ps(quat.v)));
		return result;
	}

	template <>
	inline void negate(Quaternion<float>& quat) noexcept
	{
		const auto z = _mm_setzero_ps();
		_mm_store_ps(quat.v, _mm_sub_ps(z, _mm_load_ps(quat.v)));
	}

	template <>
	[[nodiscard]] inline auto operator+(const Quaternion<float>& quat1,
		const Quaternion<float>& quat2) noexcept
	{
		Quaternion<float> result;
		_mm_store_ps(result.v, _mm_add_ps(_mm_load_ps(quat1.v), _mm_load_ps(quat2.v)));
		return result;
	}

	template <>
	inline auto& operator+=(Quaternion<float>& quat1,
		const Quaternion<float>& quat2) noexcept
	{
		_mm_store_ps(quat1.v, _mm_add_ps(_mm_load_ps(quat1.v), _mm_load_ps(quat2.v)));
		return quat1;
	}

	template <>
	[[nodiscard]] inline auto operator-(const Quaternion<float>& quat1,
		const Quaternion<float>& quat2) noexcept
	{
		Quaternion<float> result;
		_mm_store_ps(result.v, _mm_sub_ps(_mm_load_ps(quat1.v), _mm_load_ps(quat2.v)));
		return result;
	}

	template <>
	inline auto& operator-=(Quaternion<float>& quat1,
		const Quaternion<float>& quat2) noexcept
	{
		_mm_store_ps(quat1.v, _mm_sub_ps(_mm_load_ps(quat1.v), _mm_load_ps(quat2.v)));
		return quat1;
	}

	template <>
	[[nodiscard]] inline auto operator*(const Quaternion<float>& quat,
		const float scalar) noexcept
	{
		Quaternion<float> result;
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(result.v, _mm_mul_ps(_mm_load_ps(quat.v), s));
		return result;
	}

	template <>
	inline auto& operator*=(Quaternion<float>& quat,
		const float scalar) noexcept
	{
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(quat.v, _mm_mul_ps(_mm_load_ps(quat.v), s));
		return quat;
	}

	template <>
	[[nodiscard]] inline auto operator/(const Quaternion<float>& quat,
		const float scalar) noexcept
	{
		Quaternion<float> result;
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(result.v, _mm_div_ps(_mm_load_ps(quat.v), s));
		return result;
	}

	template <>
	inline auto& operator/=(Quaternion<float>& quat,
		const float scalar) noexcept
	{
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(quat.v, _mm_div_ps(_mm_load_ps(quat.v), s));
		return quat;
	}
#endif // SSE

#if defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2
#  ifndef __AVX__
	template <>
	inline auto operator-(const Quaternion<double>& quat) noexcept
	{
		Quaternion<double> result;
		const auto z = _mm_setzero_pd();
		_mm_store_pd(&result.v[0], _mm_sub_pd(z, _mm_load_pd(&quat.v[0])));
		_mm_store_pd(&result.v[2], _mm_sub_pd(z, _mm_load_pd(&quat.v[2])));
		return result;
	}

	template <>
	inline void negate(Quaternion<double>& quat) noexcept
	{
		const auto z = _mm_setzero_pd();
		_mm_store_pd(&quat.v[0], _mm_sub_pd(z, _mm_load_pd(&quat.v[0])));
		_mm_store_pd(&quat.v[2], _mm_sub_pd(z, _mm_load_pd(&quat.v[2])));
	}

	template <>
	[[nodiscard]] inline auto operator+(const Quaternion<double>& quat1,
		const Quaternion<double>& quat2) noexcept
	{
		Quaternion<double> result;
		_mm_store_pd(&result.v[0], _mm_add_pd(_mm_load_pd(&quat1.v[0]), _mm_load_pd(&quat2.v[0])));
		_mm_store_pd(&result.v[2], _mm_add_pd(_mm_load_pd(&quat1.v[2]), _mm_load_pd(&quat2.v[2])));
		return result;
	}

	template <>
	inline auto& operator+=(Quaternion<double>& quat1,
		const Quaternion<double>& quat2) noexcept
	{
		_mm_store_pd(&quat1.v[0], _mm_add_pd(_mm_load_pd(&quat1.v[0]), _mm_load_pd(&quat2.v[0])));
		_mm_store_pd(&quat1.v[2], _mm_add_pd(_mm_load_pd(&quat1.v[2]), _mm_load_pd(&quat2.v[2])));
		return quat1;
	}

	template <>
	[[nodiscard]] inline auto operator-(const Quaternion<double>& quat1,
		const Quaternion<double>& quat2) noexcept
	{
		Quaternion<double> result;
		_mm_store_pd(&result.v[0], _mm_sub_pd(_mm_load_pd(&quat1.v[0]), _mm_load_pd(&quat2.v[0])));
		_mm_store_pd(&result.v[2], _mm_sub_pd(_mm_load_pd(&quat1.v[2]), _mm_load_pd(&quat2.v[2])));
		return result;
	}

	template <>
	inline auto& operator-=(Quaternion<double>& quat1,
		const Quaternion<double>& quat2) noexcept
	{
		_mm_store_pd(&quat1.v[0], _mm_sub_pd(_mm_load_pd(&quat1.v[0]), _mm_load_pd(&quat2.v[0])));
		_mm_store_pd(&quat1.v[2], _mm_sub_pd(_mm_load_pd(&quat1.v[2]), _mm_load_pd(&quat2.v[2])));
		return quat1;
	}

	template <>
	[[nodiscard]] inline auto operator*(const Quaternion<double>& quat,
		const double scalar) noexcept
	{
		Quaternion<double> result;
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&result.v[0], _mm_mul_pd(_mm_load_pd(&quat.v[0]), s));
		_mm_store_pd(&result.v[2], _mm_mul_pd(_mm_load_pd(&quat.v[2]), s));
		return result;
	}

	template <>
	inline auto& operator*=(Quaternion<double>& quat,
		const double scalar) noexcept
	{
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&quat.v[0], _mm_mul_pd(_mm_load_pd(&quat.v[0]), s));
		_mm_store_pd(&quat.v[2], _mm_mul_pd(_mm_load_pd(&quat.v[2]), s));
		return quat;
	}

	template <>
	[[nodiscard]] inline auto operator/(const Quaternion<double>& quat,
		const double scalar) noexcept
	{
		Quaternion<double> result;
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&result.v[0], _mm_div_pd(_mm_load_pd(&quat.v[0]), s));
		_mm_store_pd(&result.v[2], _mm_div_pd(_mm_load_pd(&quat.v[2]), s));
		return result;
	}

	template <>
	inline auto& operator/=(Quaternion<double>& quat,
		const double scalar) noexcept
	{
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&quat.v[0], _mm_div_pd(_mm_load_pd(&quat.v[0]), s));
		_mm_store_pd(&quat.v[2], _mm_div_pd(_mm_load_pd(&quat.v[2]), s));
		return quat;
	}
#  endif // __AVX__
#endif // SSE2

#ifdef __SSE3__
	template <>
	[[nodiscard]] inline auto norm(const Quaternion<float>& quat) noexcept
	{
		const auto v = _mm_load_ps(quat.v);
		const auto t1 = _mm_mul_ps(v, v);
		const auto t2 = _mm_hadd_ps(t1, t1);
		const auto t3 = _mm_hadd_ps(t2, t2);
		const auto s = _mm_sqrt_ps(t3);
		return _mm_cvtss_f32(s);
	}
#endif // __SSE3__
}