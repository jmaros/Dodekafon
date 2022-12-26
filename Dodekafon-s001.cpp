// Dodekafon.cpp

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp -o Dodekafon.exe ; .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp -o Dodekafon.exe ; gdb .\Dodekafon.exe

#include <iomanip>
#include <iostream>
#include <limits>
//#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::setw;
using std::vector;

void DebugPrint()
{
	std::cout << '\n';
}
template<typename T>

void DebugPrint(T const& t)
{
	std::cout << t;
	DebugPrint();
}

template<typename T, typename...Rest>
void DebugPrint(T const& t,
				Rest const&...	rest)
{
	std::cout << t;
	DebugPrint(rest...);
}

namespace Dodekafon {

	const size_t Empty = std::numeric_limits<size_t>::max();
	const size_t Invalid = std::numeric_limits<size_t>::max() - 1;
	const size_t Endpoint = std::numeric_limits<size_t>::max() - 2;

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.

	class Spans {
	//  vector<pair<size_t, size_t>> m_spans;	// could have resulted in a cleaner logic by storing the previous and the following indices...
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
			cout << setw(80) << "This should happen only once!\n";
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

	template<typename...Rest>
	void DebugLine (Rest const&...	rest)
	{
		static int n = 0;
		DebugPrint("Debug line ", setw(3), ++n, rest...);
	}

	void SolveDodekafon (size_t          n,
						 const Spans&    sRef,
						 vector<Spans>&  result)
	{
		if (n > 0) {
			DebugLine (", Interval = ", n);

			for (size_t i = 0; i < sRef.Size(); ++i) {
				DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);

				if (sRef[i] == Empty || sRef[i] == Endpoint) {
					if (i >= n) {
						if (sRef[i - n] == Empty) {
							Spans ls(sRef);
							ls[i] = i - n;
							ls[i - n] = Endpoint;
							DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);
							SolveDodekafon (n - 1,
											ls,
											result);
						}
					}
					if (i + n < sRef.Size()) {
						if (sRef[i + n] == Empty) {
							Spans ls(sRef);
							ls[i] = i + n;
							ls[i + n] = Endpoint;
							DebugPrint(setw(16), " ", "sRef[", setw(2), i, "] = ", setw(20), sRef[i]);
							SolveDodekafon (n - 1,
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

	void PrintDodekafon (const vector<Spans>& result)
	{
		size_t n = 0;
		for (auto rsin : result) {
			Spans rsi{ RearrangeSpans(rsin) };
			cout << ++n << ". series: ";
			for (size_t i = 0; i < rsi.Size(); ++i) {
				auto ival = rsi[i];
				ival = ((ival == Empty) ? 101 : ival);
				ival = ((ival == Endpoint) ? 102 : ival);
				ival = ((ival == Invalid) ? 103 : ival);
				cout << setw(3) << ival + 1 << ", ";
			}
			cout << endl;
		}
	}
}

/// @brief  This is the main entry point of a C or C++ program
/// @return The return value is an integer, automatically provided as 0, when the extlicit return is omitterd
int main ()
{
	const size_t				SpanWidth{ 12 };
	const size_t				Interval { SpanWidth-1 };
	Dodekafon::Spans            spans(SpanWidth);
	vector<Dodekafon::Spans>    result;

	SolveDodekafon (Interval,
					spans,
					result);

	PrintDodekafon (result);

	cout << "Dodekafon" << endl;
}
