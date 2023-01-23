//
// DodekaPitches.cpp
//
#include <iomanip>
#include <iostream>

#include "DodekaPitches.h"
#include "DodekaBase.h"

using std::cout;
using std::setw;

namespace Dodekafon {
	//
	// Pitches constructors
	//
	Pitches::Pitches ()
		: m_pitches	(SpanWidth, 0)
	{
	}

	Pitches::Pitches (size_t numPitches)
		: m_pitches	(numPitches, 0)
	{
	}

	Pitches::Pitches (const Pitches& pitchesPar)
		: m_pitches	(pitchesPar.m_pitches)
	{
	}

	// Pitches accessors
	size_t Pitches::Size () const
	{
		return m_pitches.size();
	}

	size_t Pitches::GetPitch (size_t i) const
	{
		return (i < Size() ? m_pitches[i] : 0);
	}

	// Pitches transformations

	//
	// Tukor (Mirror in Hungarian)
	//
	Pitches Pitches::GenerateInversion () const
	{
		Pitches result(Size());
		const size_t Complement = Size() + 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result.SetPitch(i, Complement - GetPitch(i));
		}
		return result;
	}

	//
	// Crab
	//
	Pitches Pitches::GenerateRetrograde () const
	{
		Pitches result(Size ());
		const size_t Maxi = Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result.SetPitch(i, GetPitch(Maxi - i));
		}
		return result;
	}

	//
	// Mirror of Crab
	//
	Pitches Pitches::GenerateRetrogradeInversion () const
	{
		Pitches result(Size ());
		const size_t Complement = Size() + 1;
		const size_t Maxi = Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			size_t retroPitch = GetPitch(Maxi - i);
			size_t newPitch = Complement - retroPitch;
			result.SetPitch(i, newPitch);
		}
		return result;
	}

	void Pitches::Dump () const
	{
		if (Size() > 0) {
			cout << setw(3) << GetPitch(0);
			for (size_t i = 1; i < Size(); ++i) {
				cout << ", " << setw(3) << GetPitch(i);
			}
		}
	}

	// Pitches modifiers
	bool Pitches::SetPitch (size_t i,
							size_t pitchPar)
	{
		bool success{ i < Size() };
		if (success) {
			m_pitches[i] = pitchPar;
		}
		return success;
	}

	// Pitches global operators
	bool operator < (const Pitches& pitchesLeft,
					 const Pitches& pitchesRight)
	{
		return pitchesLeft.m_pitches < pitchesRight.m_pitches;
	}

	bool operator == (const Pitches& pitchesLeft,
					  const Pitches& pitchesRight)
	{
		return pitchesLeft.m_pitches == pitchesRight.m_pitches;
	}
}