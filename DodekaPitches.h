//
// DodekaPitches.h
//
#pragma once

#include <array>

#include "DodekaBase.h"

namespace Dodekafon {

	using std::array;


	class Pitches {

		array<size_t, MaxPitch>	m_pitches{};		// Integer 1..12 representing the pitch,
													// 0 denotes the emptyness
													// Note: this array is indexed 0-based indices
	public:
	//constructors
		//the defaults are used
	//accessors
		size_t	Size		()					const;
		size_t	GetPitch	(size_t index)		const;

	//transformations
		Pitches	GenerateInversion			()	const;
		Pitches	GenerateRetrograde			()	const;
		Pitches	GenerateRetrogradeInversion	()	const;
		void	Dump						()	const;

	//modifiers
		void SetPitch (size_t index,
					   size_t pitchPar);

		friend bool operator < (const Pitches& pitchesLeft,
								const Pitches& pitchesRight);

		friend bool operator == (const Pitches& pitchesLeft,
								 const Pitches& pitchesRight);
	};

	//global scope operators
	bool operator < (const Pitches& pitchesLeft,
					 const Pitches& pitchesRight);

	bool operator == (const Pitches& pitchesLeft,
					  const Pitches& pitchesRight);
}
