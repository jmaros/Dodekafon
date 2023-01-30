//
// NodeAndEdge.h
//
#pragma once

#include <array>

#include "DodekaBase.h"

namespace Dodekafon {

	using std::array;

	// Edge direction
	enum class Direction : int {
		Up = 1,	// The length of the edge is to be added to the number representing the previous pitch
		Invalid = 0,
		Down = -1	// The length of the edge is to be substracted from the number denoting previous pitch
	};

	const array<Direction, 2> Directions{ Direction::Down, Direction::Up };

	class NodeAndEdge {
		size_t			m_pitch{};							// Integer 0..12 representing the starting pitch
		size_t			m_width{};							// Integer 0..11 representing the pitch difference,
															// 0 denotes the emptyness (this Intreval is unused)
		Direction		m_direction{ Direction::Invalid };	// Possible Values: Up or Down
	public:
	//constructors
		NodeAndEdge () {}

	//static utilities
		static bool	IsValidPitch		(size_t		pitch);
		static bool	IsValidWidth		(size_t		width);
		static bool	IsValidDirection	(Direction	direction);

	//accessors
		bool		IsEmpty				()	const;
		bool		IsValid				()	const;
		size_t		GetWidth			()	const;
		size_t		GetPitch			()	const;
		size_t		GetNextPitch		()	const;

	//modifiers
		void		SetWidth			(size_t		newWidth);
		void		SetPitch			(size_t		newPitch);
		void		SetDirection		(Direction	newDir);

		void		Init	(size_t		width,
							 size_t		lower,
							 Direction	newDir);

	//operators
		NodeAndEdge& operator = (const NodeAndEdge&) = default;
	};
}
