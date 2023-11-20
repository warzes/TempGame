#pragma once

#include "Vector.h"

namespace math
{
	template <typename T, std::size_t dims> class Size final
	{
	public:
#if defined(__SSE__) || defined(_M_X64) || _M_IX86_FP >= 1 || defined(__ARM_NEON__)
		alignas(std::is_same_v<T, float>&& dims == 4 ? dims * sizeof(T) : sizeof(T))
#endif
#if (defined(__SSE2__) || defined(_M_X64) || _M_IX86_FP >= 2) || (defined(__ARM_NEON__) && defined(__aarch64__))
			alignas(std::is_same_v<T, double>&& dims == 4 ? dims * sizeof(T) : sizeof(T))
#endif
			std::array<T, dims> v;

		[[nodiscard]] auto& operator[](const std::size_t index) noexcept { return v[index]; }
		[[nodiscard]] constexpr auto operator[](const std::size_t index) const noexcept { return v[index]; }

		[[nodiscard]] auto& width() noexcept
		{
			static_assert(dims >= 1);
			return v[0];
		}

		[[nodiscard]] constexpr auto width() const noexcept
		{
			static_assert(dims >= 1);
			return v[0];
		}

		[[nodiscard]] auto& height() noexcept
		{
			static_assert(dims >= 2);
			return v[1];
		}

		[[nodiscard]] constexpr auto height() const noexcept
		{
			static_assert(dims >= 2);
			return v[1];
		}

		[[nodiscard]] auto& depth() noexcept
		{
			static_assert(dims >= 3);
			return v[2];
		}

		[[nodiscard]] constexpr auto depth() const noexcept
		{
			static_assert(dims >= 3);
			return v[2];
		}

		void scale(const math::Vector<T, dims>& scale) noexcept
		{
			for (std::size_t i = 0; i < dims; ++i)
				v[i] *= scale.v[i];
		}

		[[nodiscard]] constexpr auto operator+() const noexcept
		{
			return *this;
		}

		[[nodiscard]] auto operator-() const noexcept
		{
			Size result = *this;
			for (T& c : result.v)
				c = -c;
			return result;
		}

		[[nodiscard]] auto operator+(const Size& size) const noexcept
		{
			Size result = *this;
			for (std::size_t i = 0; i < dims; ++i)
				result.v[i] += size.v[i];
			return result;
		}

		auto& operator+=(const Size& size) noexcept
		{
			for (std::size_t i = 0; i < dims; ++i)
				v[i] += size.v[i];
			return *this;
		}

		[[nodiscard]] auto operator-(const Size& size) const noexcept
		{
			Size result = *this;
			for (std::size_t i = 0; i < dims; ++i)
				result.v[i] -= size.v[i];
			return result;
		}

		auto& operator-=(const Size& size) noexcept
		{
			for (std::size_t i = 0; i < dims; ++i)
				v[i] -= size.v[i];
			return *this;
		}

		[[nodiscard]] auto operator*(const T scalar) const noexcept
		{
			Size result = *this;
			for (T& c : result.v)
				c *= scalar;
			return result;
		}

		auto& operator*=(const T scalar) noexcept
		{
			for (auto& c : v)
				c *= scalar;
			return *this;
		}

		[[nodiscard]] auto operator/(const T scalar) const noexcept
		{
			Size result = *this;
			for (T& c : result.v)
				c /= scalar;
			return result;
		}

		auto& operator/=(const T scalar) noexcept
		{
			for (auto& c : v)
				c /= scalar;
			return *this;
		}

		[[nodiscard]] auto operator<(const Size& size) const noexcept
		{
			for (std::size_t i = 0; i < dims; ++i)
				if (v[i] < size.v[i]) return true;
				else if (size.v[i] < v[i]) return false;

			return false;
		}

		[[nodiscard]] constexpr auto operator==(const Size& size) const noexcept
		{
			return std::equal(std::begin(v), std::end(v), std::begin(size.v));
		}

		[[nodiscard]] constexpr auto operator!=(const Size& size) const noexcept
		{
			return !std::equal(std::begin(v), std::end(v), std::begin(size.v));
		}

		[[nodiscard]] auto volume() const noexcept
		{
			T result = 0;
			for (const auto& c : v)
				result *= c;
			return result;
		}
	};

	template <typename T, std::size_t dims>
	[[nodiscard]] auto operator*(const math::Size<T, dims>& size, const math::Vector<T, dims>& v) noexcept
	{
		auto result = size;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] *= v.v[i];
		return result;
	}

	template <typename T, std::size_t dims>
	[[nodiscard]] auto operator/(const math::Size<T, dims>& size, const math::Vector<T, dims>& v) noexcept
	{
		auto result = size;
		for (std::size_t i = 0; i < dims; ++i)
			result.v[i] /= v.v[i];
		return result;
	}
}
