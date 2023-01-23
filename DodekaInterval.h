//
// DodekaInterval.h
//
#pragma once

#include <vector>

namespace Dodekafon {

	using std::vector;

	class Interval {
		size_t			m_width{};	// Integer 0..11 representing the pitch difference,	0 denotes the emptyness (m_width = abs(m_pitches[0] - m_pitches[1]))
		vector<size_t>	m_pitches;	// Integer 0..12 representing the connecting pitches, 0 denotes the emptyness (exactly 2 values)
	public:
	//constructors
		Interval () {}

	//accessors
		bool		IsEmpty			()					const;
		size_t		GetWidth		()					const;
		size_t		GetPitch		(size_t index)		const;
		size_t		GetNextPitch	(size_t prevPitch)	const;

	//modifiers
		Interval& SetWidth	(size_t		newWidth);
		Interval& AddPitch	(size_t		newPitch);

		void		Init	(size_t		width,
							 size_t		lower);

	//operators
		Interval& operator = (const Interval&) = default;
	};
}
