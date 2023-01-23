//
// DodekaPitches.h
//
#pragma once

#include <vector>

namespace Dodekafon {

	using std::vector;


	class Pitches {

		vector<size_t>		m_pitches;		// Integer 1..12 representing the pitch,
											// 0 denotes the emptyness
											// Note: this vector is indexed 0-based indices
	public:
	//constructors
		Pitches ();
		explicit Pitches (size_t numPitches);
		Pitches (const Pitches& pitchesPar);

	//accessors
		size_t Size () const;
		size_t GetPitch (size_t i) const;

	//modifiers
		bool SetPitch (size_t i,
					   size_t pitchPar);

	//transformations
		Pitches			GenerateInversion()				const;
		Pitches			GenerateRetrograde()			const;
		Pitches			GenerateRetrogradeInversion()	const;
		void			Dump()							const;

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
