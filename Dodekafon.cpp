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
const size_t				Interval{ /* SpanWidth - */ 1 };


template<typename...Rest>
void DebugLine (Rest const&...	rest)
{
	static int n = 0;
	DebugPrint("Debug line ", setw(3), ++n, rest...);
}


namespace Dodekafon {

	const size_t Empty = std::numeric_limits<size_t>::max();
	const size_t Invalid = std::numeric_limits<size_t>::max() - 1;
	const size_t Endpoint = std::numeric_limits<size_t>::max() - 2;

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.

	class Spans {
		vector<size_t>    m_spans;
	public:
	//constructors
		Spans (size_t n);
		Spans (const Spans& parent);

		size_t Size () const;

		const size_t& operator [] (size_t i) const;
		size_t& operator [] (size_t i);
	};

	Spans::Spans (size_t n)
		: m_spans (n, Empty)
	{
	}

	Spans::Spans (const Spans& parent)
		: m_spans (parent.m_spans)
	{
	}

	size_t Spans::Size () const
	{
		return m_spans.size();
	}

	const size_t& Spans::operator [] (size_t i) const
	{
		const size_t* result{ &Invalid };
		if (i < Size()) {
			result = &m_spans[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	size_t& Spans::operator [] (size_t  i)
	{
		static auto myInvalid = Empty;
		if (myInvalid != Invalid) {
			myInvalid = Invalid;
		//	cout << setw(80) << "This should happen only once!\n";
		}
		size_t* result{ &myInvalid };
		if (i < Size()) {
			result = &m_spans[i];
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
		//	DebugLine (", Interval = ", n);

			for (size_t i = 0; i < sRef.Size(); ++i) {
		//		DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);

				if (sRef[i] == Empty || sRef[i] == Endpoint) {
					if (i >= n) {
						if (sRef[i - n] == Empty) {
							Spans ls(sRef);
							ls[i] = i - n;
							ls[i - n] = Endpoint;
		//					DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);
							SolveDodekafon (n + 1,
											ls,
											result);
						}
					}
					if (i + n < sRef.Size()) {
						if (sRef[i + n] == Empty) {
							Spans ls(sRef);
							ls[i] = i + n;
							ls[i + n] = Endpoint;
		//					DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);
							SolveDodekafon (n + 1,
											ls,
											result);
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
				if (spanPar[i] == first) {
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
	//
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
			nextIndex = spanPar[nextIndex];
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
			 i < result.Size() &&
			 spanPar[i] != Endpoint &&
			 spanPar[i] != Empty &&
			 spanPar[i] != Invalid;
			 ++i) {
			result[i] = Complement - spanPar[i];
		}
		return result;
	}

	const Spans GenerateRetrograde(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		const size_t Complement = spanPar.Size() - 1;
		for (size_t i = 0;
			 i < result.Size() &&
			 spanPar[i] != Endpoint &&
			 spanPar[i] != Empty &&
			 spanPar[i] != Invalid;
			 ++i) {
			result[i] = spanPar[Complement - i];
		}
		return result;
	}

	const Spans GenerateRetrogradeInversion(const Spans& spanPar)
	{
		Spans result(spanPar.Size ());
		const size_t Complement = spanPar.Size() - 1;
		for (size_t i = 0;
			 i < result.Size() &&
			 spanPar[i] != Endpoint &&
			 spanPar[i] != Empty &&
			 spanPar[i] != Invalid;
			 ++i) {
			result[i] = Complement - spanPar[Complement - i];
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
					auto ival = rsi[i];
					switch (ival) {
						case Empty:		cout << " Empty, ";		break;
						case Endpoint:	cout << " Endpoint, ";	break;
						case Invalid:	cout << " Invalid, ";	break;
						default:
							cout << setw(3) << ival + 1 << ", ";
							break;
					}
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

	SolveDodekafon (Interval,
					spans,
					result);

	cout << "Dodekafon 1 --> 12" << endl;

	PrintDodekafon (result);

}
