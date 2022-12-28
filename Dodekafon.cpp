// Dodekafon.cpp

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp -o Dodekafon.exe ; .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp -o Dodekafon.exe ; gdb .\Dodekafon.exe

#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#include "DebugPrint.h"

using std::cout;
using std::endl;
using std::setw;
using std::vector;

const size_t				SpanWidth{ 12 };
const size_t				FirstIntervalLength{ 1 };					// Upward
//const size_t				FirstIntervalLength{ SpanWidth - 1 };		// Downward


template<typename...Rest>
void DebugLine (Rest const&...	rest)
{
	static int n = 0;
	DebugPrint("Debug line ", setw(3), ++n, rest...);
}


namespace Dodekafon {

	const size_t Empty = std::numeric_limits<size_t>::min();
	const size_t Invalid = std::numeric_limits<size_t>::max() - 1;
	const size_t Endpoint = std::numeric_limits<size_t>::max() - 2;

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.

	class Note {
		size_t			m_pitch{};	// Integer 1..12 representing the pitch, 0 denotes the emptyness
		int32_t			m_prev{};	// Integer -11..+11 representing the previous interval
		int32_t			m_next{};	// Integer -11..+11 representing the next interval
	public:
	//constructors
		Note() {}
		Note(size_t note)
			:	m_pitch(note)
		{
		}

		size_t	Pitch() const
		{
			return	m_pitch;
		}

		void	Pitch(size_t newPitch)
		{
			m_pitch = newPitch;
		}

	};

	class Interval {
		size_t			m_width{};	// Integer 0..11 representing the pitch difference, 0 denotes the emptyness
		size_t			m_prev{};	// Integer 0..12 representing the previous pitch, 0 denotes the emptyness
		size_t			m_next{};	// Integer 0..12 representing the next pitch, 0 denotes the emptyness
	public:
	//constructors
		Interval() {}
	};

	// Why cant find this:
	//  3, 10, 2, 11, 1, 12, 6, 7, 5, 8, 4, 9,
	//	  7, 8, 9, 10, 11, 6, 1, 2, 3, 4, 5,
	// Internal:
	//  2, 9, 1, 10, 0, 11, 5, 6, 4, 7, 3, 8,

	class Spans {
		vector<Note>    m_notes;
	public:
	//constructors
		Spans (size_t n);
		Spans (const Spans& parent);

		size_t Size () const;

		const Note& operator [] (size_t i) const;
		Note& operator [] (size_t i);
	};

	Spans::Spans (size_t n)
		: m_notes (n, Empty)
	{
	}

	Spans::Spans (const Spans& parent)
		: m_notes (parent.m_notes)
	{
	}

	size_t Spans::Size () const
	{
		return m_notes.size();
	}

	const Note& Spans::operator [] (size_t i) const
	{
		static const Note invalid{ Invalid };
		const Note* result{ &invalid };
		if (i < Size()) {
			result = &m_notes[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	Note& Spans::operator [] (size_t  i)
	{
		static const Note invalid{ Empty };
		static Note myInvalid{ Empty };
		if (myInvalid.Pitch() != invalid.Pitch()) {
			myInvalid = Invalid;
		//	cout << setw(80) << "This should happen only once!\n";
		}
		Note* result{ &myInvalid };
		if (i < Size()) {
			result = &m_notes[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	void SolveDodekafon (size_t          n,
						 const Spans&    sRef,
						 vector<Spans>&  result)
	{
		if (n > 0 && n < SpanWidth) {

			for (size_t i = 0; i < sRef.Size(); ++i) {

				if (sRef[i].Pitch() == Empty || sRef[i].Pitch() == Endpoint) {
					if (i >= n) {
						if (sRef[i - n].Pitch() == Empty) {
							Spans ls(sRef);
							ls[i] = i - n;
							ls[i - n] = Endpoint;
							//DebugLine(", Interval = ", setw(2), n, ". Pos(", setw(2), i, ") = ", setw(2), ls[i].Pitch() + 1);

							SolveDodekafon (n + 1,			// Upward
											ls,
											result);

						//	SolveDodekafon (n - 1,			// Downward
						//					ls,
						//					result);
						}
					}
					if (i + n < sRef.Size()) {
						if (sRef[i + n].Pitch() == Empty) {
							Spans ls(sRef);
							ls[i] = i + n;
							ls[i + n] = Endpoint;
						//	DebugLine(", Interval = ", setw(2), n,". Pos(", setw(2), i, ") = ", setw(2), ls[i].Pitch() + 1);

							SolveDodekafon (n + 1,			// Upward
											ls,
											result);
						//	SolveDodekafon (n - 1,			// Downward
						//					ls,
						//					result);
						}
					}
				}
			}
		} else {
			result.push_back(sRef);
		}
	}

	size_t	FindFirstIndex (const Spans& spanPar)
	{
		size_t	result = Invalid;
		for (size_t first = 0; first < spanPar.Size(); ++first) {
			result = first;
			for (size_t i = 0; i < spanPar.Size(); ++i) {
				if (spanPar[i].Pitch() == first) {
					result = Invalid;
					break;
				}
			}
			if (result != Invalid) {
				break;
			}
		}
		return result;
	}

	//
	// Converting to time based representation of the notes
	//
	const Spans RearrangeSpans(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		// Find first index
		size_t	nextIndex = FindFirstIndex (spanPar);
		for (size_t i = 0;
			 i < result.Size()		&&
			 nextIndex != Endpoint	&&
			 nextIndex != Invalid;
			 ++i) {
			result[i] = nextIndex;
			nextIndex = spanPar[nextIndex].Pitch();
		}
		return result;
	}

	//
	// Tukor (Mirror in Hungarian)
	//
	const Spans GenerateInversion(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		const size_t Complement = spanPar.Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result[i].Pitch (Complement - spanPar[i].Pitch());
		}
		return result;
	}

	//
	// Rak (Crab in Hungarian)
	//
	const Spans GenerateRetrograde(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		const size_t Complement = spanPar.Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result[i] = spanPar[Complement - i];
		}
		return result;
	}

	//
	// Rak Tukre (Mirror of Crab in Hungarian)
	//
	const Spans GenerateRetrogradeInversion(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		const size_t Complement = spanPar.Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result[i] = Complement - spanPar[Complement - i].Pitch();
		}
		return result;
	}

	vector<Spans> GenerateVariations (const Spans&	rsin)
	{
		vector<Spans> result;
		result.push_back(RearrangeSpans(rsin));
		result.push_back(GenerateInversion(result[0]));				// Tuekoer
		result.push_back(GenerateRetrograde(result[0]));			// Raak
		result.push_back(GenerateRetrogradeInversion(result[0]));	// Tuekoer Raakja

		return result;
	}

	//
	// The goal is to print the 1..12 integer numbers that are representing the pitches of an octave
	// (here we are changing the internal 0..11 repres4ntation into the 1..12 representation
	//
	void PrintDodekafon (const vector<Spans>& result)
	{
		size_t n = 0;
		for (auto rsin : result) {
			const vector<Spans> rsi4{ GenerateVariations(rsin) };
			++n;
			size_t v = 0;
			for (auto rsi : rsi4) {
				cout << n << "." << ++v << " series: ";
				for (size_t i = 0; i < rsi.Size(); ++i) {
					cout << setw(3) << rsi[i].Pitch() + 1 << ", ";
				}
				cout << endl;
			}
		}
	}
}

/// @brief  This is the main entry point of a C or C++ program
/// @return The return value is an integer, automatically provided as 0, when the extlicit return is omitterd
int main ()
{
	Dodekafon::Spans            spans(SpanWidth);
	vector<Dodekafon::Spans>    result;

	SolveDodekafon (FirstIntervalLength,
					spans,
					result);

	cout << "Dodekafon 1 --> 12" << endl;

	PrintDodekafon (result);

}
