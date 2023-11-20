#pragma once

namespace math
{
	template <typename T, std::size_t dims> class Vector final
	{
	public:
#if defined(__SSE__) || defined(_M_X64) || _M_IX86_FP >= 1 || defined(__ARM_NEON__)
		alignas(std::is_same_v<T, float>&& dims == 4 ? dims * sizeof(T) : sizeof(T))
#endif
#if (defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2) || (defined(__ARM_NEON__) && defined(__aarch64__))
			alignas(std::is_same_v<T, double>&& dims == 4 ? dims * sizeof(T) : sizeof(T))
#endif
			T v[dims];

		constexpr Vector() noexcept = default;

		template <typename ...A>
		explicit constexpr Vector(const A... args) noexcept :
			v{ args... }
		{
		}

		template <std::size_t d, auto c = dims, std::enable_if_t<(c != d)>* = nullptr>
		explicit Vector(const math::Vector<T, d>& vec) noexcept
		{
			for (std::size_t i = 0; i < dims; ++i)
				v[i] = (i < d) ? vec.v[i] : T(0);
		}

		[[nodiscard]] auto& operator[](const std::size_t index) noexcept { return v[index]; }
		[[nodiscard]] constexpr auto operator[](const std::size_t index) const noexcept { return v[index]; }

		[[nodiscard]] auto& x() noexcept
		{
			static_assert(dims >= 1);
			return v[0];
		}

		[[nodiscard]] constexpr auto x() const noexcept
		{
			static_assert(dims >= 1);
			return v[0];
		}

		[[nodiscard]] auto& y() noexcept
		{
			static_assert(dims >= 2);
			return v[1];
		}

		[[nodiscard]] constexpr auto y() const noexcept
		{
			static_assert(dims >= 2);
			return v[1];
		}

		[[nodiscard]] auto& z() noexcept
		{
			static_assert(dims >= 3);
			return v[2];
		}

		[[nodiscard]] constexpr auto z() const noexcept
		{
			static_assert(dims >= 3);
			return v[2];
		}

		[[nodiscard]] auto& w() noexcept
		{
			static_assert(dims >= 4);
			return v[3];
		}

		[[nodiscard]] constexpr auto w() const noexcept
		{
			static_assert(dims >= 4);
			return v[3];
		}
	};

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator==(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			if (vector1.v[i] != vector2.v[i]) return false;
		return true;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator!=(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			if (vector1.v[i] != vector2.v[i]) return true;
		return false;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator<(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			if (vector1.v[i] < vector2.v[i]) return true;
		return false;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator>(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			if (vector1.v[i] > vector2.v[i]) return true;
		return false;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator+(Vector<T, dims>& vector) noexcept
	{
		return vector;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator-(const Vector<T, dims>& vector) noexcept
	{
		Vector<T, dims> result;
		for (std::size_t i = 0; i < dims; ++i) result.v[i] = -vector.v[i];
		return result;
	}

	template <typename T, std::size_t dims>
	constexpr void negate(Vector<T, dims>& vector) noexcept
	{
		for (auto& c : vector.v) c = -c;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator+(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		Vector<T, dims> result;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] = vector1.v[i] + vector2.v[i];
		return result;
	}

	template <typename T, std::size_t dims>
	auto& operator+=(Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			vector1.v[i] += vector2.v[i];
		return vector1;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator-(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		Vector<T, dims> result;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] = vector1.v[i] - vector2.v[i];
		return result;
	}

	template <typename T, std::size_t dims>
	auto& operator-=(Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			vector1.v[i] -= vector2.v[i];
		return vector1;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator*(const Vector<T, dims>& vector,
		const T scalar) noexcept
	{
		Vector<T, dims> result;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] = vector.v[i] * scalar;
		return result;
	}

	template <typename T, std::size_t dims>
	auto& operator*=(Vector<T, dims>& vector, const T scalar) noexcept
	{
		for (auto& c : vector.v) c *= scalar;
		return vector;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto operator/(const Vector<T, dims>& vector,
		const T scalar) noexcept
	{
		Vector<T, dims> result;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] = vector.v[i] / scalar;
		return result;
	}

	template <typename T, std::size_t dims>
	auto& operator/=(Vector<T, dims>& vector, const T scalar) noexcept
	{
		for (auto& c : vector.v) c /= scalar;
		return vector;
	}

	template <typename T>
	[[nodiscard]] constexpr auto cross(const Vector<T, 3>& vector1,
		const Vector<T, 3>& vector2) noexcept
	{
		return Vector<T, 3>{
			(vector1.v[1] * vector2.v[2]) - (vector1.v[2] * vector2.v[1]),
				(vector1.v[2] * vector2.v[0]) - (vector1.v[0] * vector2.v[2]),
				(vector1.v[0] * vector2.v[1]) - (vector1.v[1] * vector2.v[0])
		};
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto length(const Vector<T, dims>& vector) noexcept
	{
		T lengthSquared{};
		for (const auto& c : vector.v) lengthSquared += c * c;
		return std::sqrt(lengthSquared);
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto lengthSquared(const Vector<T, dims>& vector) noexcept
	{
		T lengthSquared{};
		for (const auto& c : vector.v) lengthSquared += c * c;
		return lengthSquared;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto dot(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		T result{};
		for (std::size_t i = 0; i < dims; ++i)
			result += vector1.v[i] * vector2[i];
		return result;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto distance(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		T distanceSquared{};
		for (std::size_t i = 0; i < dims; ++i)
			distanceSquared += (vector1.v[i] - vector2.v[i]) * (vector1.v[i] - vector2.v[i]);
		return std::sqrt(distanceSquared);
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] constexpr auto distanceSquared(const Vector<T, dims>& vector1,
		const Vector<T, dims>& vector2) noexcept
	{
		T distanceSquared{};
		for (std::size_t i = 0; i < dims; ++i)
			distanceSquared += (vector1.v[i] - vector2.v[i]) * (vector1.v[i] - vector2.v[i]);
		return distanceSquared;
	}

	template <typename T, std::size_t dims>
	void normalize(Vector<T, dims>& vector) noexcept
	{
		if (const auto l = length(vector); l > T(0))
			for (auto& c : vector.v) c /= l;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto normalized(const Vector<T, dims>& vector) noexcept
	{
		Vector<T, dims> result;
		if (const auto l = length(vector); l > T(0))
			for (std::size_t i = 0; i < dims; ++i)
				result.v[i] = vector.v[i] / l;
		else
			result = {};
		return result;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto operator*(const T scalar,
		const Vector<T, dims>& vec) noexcept
	{
		return vec * scalar;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto isNormalized(const Vector<T, dims>& vec,
		const T tolerance = std::numeric_limits<T>::epsilon()) noexcept
	{
		return std::abs(T(1) - lengthSquared(vec)) <= tolerance;
	}

	template <typename T>
	[[nodiscard]] auto getAngle(const Vector<T, 2>& vec) noexcept
	{
		return std::atan2(vec.v[1], vec.v[0]);
	}

	template <typename T>
	[[nodiscard]] auto getAngle(const Vector<T, 3>& vec,
		const Vector<T, 3>& axis) noexcept
	{
		constexpr T dx = axis.v[0] - vec.v[0] - vec.v[1] * axis.v[2] + vec.v[2] * axis.v[1];
		constexpr T dy = axis.v[1] - vec.v[1] - vec.v[2] * axis.v[0] + vec.v[0] * axis.v[2];
		constexpr T dz = axis.v[2] - vec.v[2] - vec.v[0] * axis.v[1] + vec.v[1] * axis.v[0];

		return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz), dot(axis));
	}

	template <typename T>
	[[nodiscard]] auto getAngle(const Vector<T, 4>& vec,
		const Vector<T, 4>& axis) noexcept
	{
		constexpr T dx = vec.v[3] * axis.v[0] - vec.v[0] * axis.v[3] - vec.v[1] * axis.v[2] + vec.v[2] * axis.v[1];
		constexpr T dy = vec.v[3] * axis.v[1] - vec.v[1] * axis.v[3] - vec.v[2] * axis.v[0] + vec.v[0] * axis.v[2];
		constexpr T dz = vec.v[3] * axis.v[2] - vec.v[2] * axis.v[3] - vec.v[0] * axis.v[1] + vec.v[1] * axis.v[0];

		return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz), dot(axis));
	}

	template <typename T>
	void rotate(Vector<T, 2>& vec, const T angle) noexcept
	{
		const auto sine = std::sin(angle);
		const auto cosine = std::cos(angle);

		const auto tempX = vec.v[0] * cosine - vec.v[1] * sine;
		vec.v[1] = vec.v[1] * cosine + vec.v[0] * sine;
		vec.v[0] = tempX;
	}

	template <typename T>
	void rotate(Vector<T, 2>& vec,
		const math::Vector<T, 2>& point,
		const T angle) noexcept
	{
		const auto sine = std::sin(angle);
		const auto cosine = std::cos(angle);

		if (point.v[0] == T(0) || point.v[1] == T(0))
		{
			const auto tempX = vec.v[0] * cosine - vec.v[1] * sine;
			vec.v[1] = vec.v[1] * cosine + vec.v[0] * sine;
			vec.v[0] = tempX;
		}
		else
		{
			const auto tempX = vec.v[0] - point.v[0];
			const auto tempY = vec.v[1] - point.v[1];

			vec.v[0] = tempX * cosine - tempY * sine + point.v[0];
			vec.v[1] = tempY * cosine + tempX * sine + point.v[1];
		}
	}

	template <typename T, std::size_t dims>
	void clamp(Vector<T, dims>& vec,
		const Vector<T, dims>& min,
		const Vector<T, dims>& max) noexcept
	{
		for (std::size_t i = 0; i < dims; ++i)
			if (vec.v[i] < min.v[i]) vec.v[i] = min.v[i];
			else if (vec.v[i] > max.v[i]) vec.v[i] = max.v[i];
	}

	template <typename T, std::size_t dims>
	void smooth(Vector<T, dims>& vec,
		const Vector<T, dims>& target,
		const T elapsedTime,
		const T responseTime) noexcept
	{
		if (elapsedTime > T(0))
			vec += (target - vec) * (elapsedTime / (elapsedTime + responseTime));
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
	inline auto operator-(const Vector<float, 4>& vector) noexcept
	{
		Vector<float, 4> result;
		const auto z = _mm_setzero_ps();
		_mm_store_ps(result.v, _mm_sub_ps(z, _mm_load_ps(vector.v)));
		return result;
	}

	template <>
	inline void negate(Vector<float, 4>& vector) noexcept
	{
		const auto z = _mm_setzero_ps();
		_mm_store_ps(vector.v, _mm_sub_ps(z, _mm_load_ps(vector.v)));
	}

	template <>
	[[nodiscard]] inline auto operator+(const Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		Vector<float, 4> result;
		_mm_store_ps(result.v, _mm_add_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v)));
		return result;
	}

	template <>
	inline auto& operator+=(Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		_mm_store_ps(vector1.v, _mm_add_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v)));
		return vector1;
	}

	template <>
	[[nodiscard]] inline auto operator-(const Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		Vector<float, 4> result;
		_mm_store_ps(result.v, _mm_sub_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v)));
		return result;
	}

	template <>
	inline auto& operator-=(Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		_mm_store_ps(vector1.v, _mm_sub_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v)));
		return vector1;
	}

	template <>
	[[nodiscard]] inline auto operator*(const Vector<float, 4>& vector,
		const float scalar) noexcept
	{
		Vector<float, 4> result;
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(result.v, _mm_mul_ps(_mm_load_ps(vector.v), s));
		return result;
	}

	template <>
	inline auto& operator*=(Vector<float, 4>& vector,
		const float scalar) noexcept
	{
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(vector.v, _mm_mul_ps(_mm_load_ps(vector.v), s));
		return vector;
	}

	template <>
	[[nodiscard]] inline auto operator/(const Vector<float, 4>& vector,
		const float scalar) noexcept
	{
		Vector<float, 4> result;
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(result.v, _mm_div_ps(_mm_load_ps(vector.v), s));
		return result;
	}

	template <>
	inline auto& operator/=(Vector<float, 4>& vector,
		const float scalar) noexcept
	{
		const auto s = _mm_set1_ps(scalar);
		_mm_store_ps(vector.v, _mm_div_ps(_mm_load_ps(vector.v), s));
		return vector;
	}

#  ifndef __SSE3__
	template <>
	[[nodiscard]] inline auto length(const Vector<float, 4>& vector) noexcept
	{
		const auto v = _mm_load_ps(vector.v);
		const auto mul = _mm_mul_ps(v, v);
		const auto shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 0, 3));
		const auto sum = _mm_add_ps(mul, shuf);
		const auto mov = _mm_movehl_ps(shuf, sum);
		const auto sqrt = _mm_sqrt_ps(_mm_add_ss(sum, mov));
		return _mm_cvtss_f32(sqrt);
	}

	template <>
	[[nodiscard]] inline auto lengthSquared(const Vector<float, 4>& vector) noexcept
	{
		const auto v = _mm_load_ps(vector.v);
		const auto mul = _mm_mul_ps(v, v);
		const auto shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 0, 3));
		const auto sum = _mm_add_ps(mul, shuf);
		const auto mov = _mm_movehl_ps(shuf, sum);
		return _mm_cvtss_f32(_mm_add_ss(sum, mov));
	}

	template <>
	[[nodiscard]] inline auto dot(const Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		const auto mul = _mm_mul_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v));
		const auto shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 0, 3));
		const auto sum = _mm_add_ps(mul, shuf);
		const auto mov = _mm_movehl_ps(shuf, sum);
		return _mm_cvtss_f32(_mm_add_ss(sum, mov));
	}
#  endif // __SSE3__
#endif // SSE

#if defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2
#  ifndef __AVX__
	template <>
	inline auto operator-(const Vector<double, 4>& vector) noexcept
	{
		Vector<double, 4> result;
		const auto z = _mm_setzero_pd();
		_mm_store_pd(&result.v[0], _mm_sub_pd(z, _mm_load_pd(&vector.v[0])));
		_mm_store_pd(&result.v[2], _mm_sub_pd(z, _mm_load_pd(&vector.v[2])));
		return result;
	}

	template <>
	inline void negate(Vector<double, 4>& vector) noexcept
	{
		const auto z = _mm_setzero_pd();
		_mm_store_pd(&vector.v[0], _mm_sub_pd(z, _mm_load_pd(&vector.v[0])));
		_mm_store_pd(&vector.v[2], _mm_sub_pd(z, _mm_load_pd(&vector.v[2])));
	}

	template <>
	[[nodiscard]] inline auto operator+(const Vector<double, 4>& vector1,
		const Vector<double, 4>& vector2) noexcept
	{
		Vector<double, 4> result;
		_mm_store_pd(&result.v[0], _mm_add_pd(_mm_load_pd(&vector1.v[0]), _mm_load_pd(&vector2.v[0])));
		_mm_store_pd(&result.v[2], _mm_add_pd(_mm_load_pd(&vector1.v[2]), _mm_load_pd(&vector2.v[2])));
		return result;
	}

	template <>
	inline auto& operator+=(Vector<double, 4>& vector1,
		const Vector<double, 4>& vector2) noexcept
	{
		_mm_store_pd(&vector1.v[0], _mm_add_pd(_mm_load_pd(&vector1.v[0]), _mm_load_pd(&vector2.v[0])));
		_mm_store_pd(&vector1.v[2], _mm_add_pd(_mm_load_pd(&vector1.v[2]), _mm_load_pd(&vector2.v[2])));
		return vector1;
	}

	template <>
	[[nodiscard]] inline auto operator-(const Vector<double, 4>& vector1,
		const Vector<double, 4>& vector2) noexcept
	{
		Vector<double, 4> result;
		_mm_store_pd(&result.v[0], _mm_sub_pd(_mm_load_pd(&vector1.v[0]), _mm_load_pd(&vector2.v[0])));
		_mm_store_pd(&result.v[2], _mm_sub_pd(_mm_load_pd(&vector1.v[2]), _mm_load_pd(&vector2.v[2])));
		return result;
	}

	template <>
	inline auto& operator-=(Vector<double, 4>& vector1,
		const Vector<double, 4>& vector2) noexcept
	{
		_mm_store_pd(&vector1.v[0], _mm_sub_pd(_mm_load_pd(&vector1.v[0]), _mm_load_pd(&vector2.v[0])));
		_mm_store_pd(&vector1.v[2], _mm_sub_pd(_mm_load_pd(&vector1.v[2]), _mm_load_pd(&vector2.v[2])));
		return vector1;
	}

	template <>
	[[nodiscard]] inline auto operator*(const Vector<double, 4>& vector,
		const double scalar) noexcept
	{
		Vector<double, 4> result;
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&result.v[0], _mm_mul_pd(_mm_load_pd(&vector.v[0]), s));
		_mm_store_pd(&result.v[2], _mm_mul_pd(_mm_load_pd(&vector.v[2]), s));
		return result;
	}

	template <>
	inline auto& operator*=(Vector<double, 4>& vector,
		const double scalar) noexcept
	{
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&vector.v[0], _mm_mul_pd(_mm_load_pd(&vector.v[0]), s));
		_mm_store_pd(&vector.v[2], _mm_mul_pd(_mm_load_pd(&vector.v[2]), s));
		return vector;
	}

	template <>
	[[nodiscard]] inline auto operator/(const Vector<double, 4>& vector,
		const double scalar) noexcept
	{
		Vector<double, 4> result;
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&result.v[0], _mm_div_pd(_mm_load_pd(&vector.v[0]), s));
		_mm_store_pd(&result.v[2], _mm_div_pd(_mm_load_pd(&vector.v[2]), s));
		return result;
	}

	template <>
	inline auto& operator/=(Vector<double, 4>& vector,
		const double scalar) noexcept
	{
		const auto s = _mm_set1_pd(scalar);
		_mm_store_pd(&vector.v[0], _mm_div_pd(_mm_load_pd(&vector.v[0]), s));
		_mm_store_pd(&vector.v[2], _mm_div_pd(_mm_load_pd(&vector.v[2]), s));
		return vector;
	}
#  endif // __AVX__
#endif // SSE2

#ifdef __SSE3__
	template <>
	[[nodiscard]] inline auto length(const Vector<float, 4>& vector) noexcept
	{
		const auto v = _mm_load_ps(vector.v);
		const auto t1 = _mm_mul_ps(v, v);
		const auto t2 = _mm_hadd_ps(t1, t1);
		const auto t3 = _mm_hadd_ps(t2, t2);
		const auto s = _mm_sqrt_ps(t3);
		return _mm_cvtss_f32(s);
	}

	template <>
	[[nodiscard]] inline auto lengthSquared(const Vector<float, 4>& vector) noexcept
	{
		const auto v = _mm_load_ps(vector.v);
		const auto t1 = _mm_mul_ps(v, v);
		const auto t2 = _mm_hadd_ps(t1, t1);
		const auto t3 = _mm_hadd_ps(t2, t2);
		return _mm_cvtss_f32(t3);
	}

	template <>
	[[nodiscard]] inline auto dot(const Vector<float, 4>& vector1,
		const Vector<float, 4>& vector2) noexcept
	{
		const auto t1 = _mm_mul_ps(_mm_load_ps(vector1.v), _mm_load_ps(vector2.v));
		const auto t2 = _mm_hadd_ps(t1, t1);
		const auto t3 = _mm_hadd_ps(t2, t2);
		return _mm_cvtss_f32(t3);
	}
#endif // __SSE3__
}
