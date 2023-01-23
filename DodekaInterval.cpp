//
// DodekaInterval.cpp
//

#include "DodekaInterval.h"

namespace Dodekafon {
	//
	// Interval accessors
	//
	bool	Interval::IsEmpty () const
	{
		return	m_pitches.empty();
	}

	size_t	Interval::GetWidth () const
	{
		return	m_width;
	}

	// GetPitch (size_t	index) --> size_t
	// return the indexth connegting pitch
	// Valid "index" can be 0 or 1, since each
	// interval is supposed to be used only once
	size_t	Interval::GetPitch (size_t	index) const
	{
		size_t result{};

		if (index < 2 && index < m_pitches.size()) {
			result = m_pitches[index];
		}
		return	result;
	}

	size_t	Interval::GetNextPitch (size_t	prevPitch) const
	{
		size_t	nextPitch{ GetPitch(0) };
		if (nextPitch == prevPitch) {
			nextPitch = GetPitch(1);
		}
		return nextPitch;
	}

	//
	// Interval modifiers
	//
	Interval& Interval::SetWidth (size_t newWidth)
	{
		m_width = newWidth;
		return *this;
	}

	Interval& Interval::AddPitch (size_t newPitch)
	{
		if (m_pitches.size() < 2) {
			m_pitches.push_back(newPitch);
		}
		return *this;
	}

	void	Interval::Init	(size_t		width,
							 size_t		lower)
	{
		SetWidth(width);
		AddPitch(lower);
		AddPitch(width + lower);
	}

}
