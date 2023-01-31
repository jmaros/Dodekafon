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
	// Pitches constructors are the defaults
	//

	// Pitches accessors
	size_t Pitches::Size () const
	{
		return m_pitches.size();
	}

	size_t Pitches::GetPitch (size_t index) const
	{
		if (index >= Size() ||
			Size() == 0) {
			throw std::out_of_range("Pitches::GetPitch");
		}
		return m_pitches[index];
	}

	// Pitches transformations

	//
	// Tukor (Mirror in Hungarian)
	//
	Pitches Pitches::GenerateInversion () const
	{
		Pitches result;
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
		Pitches result;
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
		Pitches result;
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
	void Pitches::SetPitch (size_t index,
							size_t pitchPar)
	{
		if (index >= Size()) {
			throw std::out_of_range("Pitches::SetPitch");
		}
		m_pitches[index] = pitchPar;
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