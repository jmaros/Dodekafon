//
// DodekaBase.h
//
#pragma once

#define DEBUG_SOLVE_DODEKAFON
#define NO_ELENDIR

#include <type_traits>
#include <cstddef>

namespace Dodekafon {
	const size_t				SpanWidth{ 12 };
	const size_t				MaxIntervalLength{ SpanWidth - 1 };		// Downward

	template <typename T>
	T Distance (T v1, T v2)
	{
		return (v1 < v2) ? (v2 - v1) : (v1 - v2);
	}

	template <typename T>
	/* Here should require concept for enum class type on T */
	std::underlying_type_t<T> UnderlyingType (T value)
	{
		using UTT = std::underlying_type_t<T>;
		return static_cast<UTT>(value);
	}
}
