//
// DodekaBase.h
//
#pragma once

#include <type_traits>
#include <limits>
#include <cstddef>

namespace Dodekafon {
	const size_t	MaxPitch{ 12 };					// Number of elements in the sound queue
	const size_t	PitchLimit{ MaxPitch + 1 };		// Pitch limit, larger
													// than the max pitch value
	const size_t	EdgeLimit{ MaxPitch };			// Edge upper limit, any smaller positive nonzero integer
													// represent a valid edge length value
													// Same as the limit for the max number of edges
	const size_t	MaxEdge{ MaxPitch - 1 };		// Max edge length

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
