//
// Dodekafon.cpp
//

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DebugSolveDodekafon.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp ..\Testdata.cpp -o Dodekafon.exe && .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DebugSolveDodekafon.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp ..\Testdata.cpp -o Dodekafon.exe && gdb .\Dodekafon.exe

#include <exception>
#include <iomanip>
#include <iostream>
#include <set>

#include "Dodekafon.h"

using std::cout;
using std::endl;
using std::setw;
using std::set;

namespace Dodekafon {

	//
	// The goal is to print the 1..12 integer numbers that are representing the pitches of an octave
	// (here we are changing the internal 0..11 repres4ntation into the 1..12 representation
	//
	void PrintDodekafon (const vector<Spans>& result)
	{
		size_t d = 0;
		size_t n = 0;
		set<Pitches> sortedList;
		for (auto & rsin : result) {
			const vector<Pitches> rsi4{ rsin.GenerateVariations() };
			++n;
			size_t v = 0;
			for (auto & rsi : rsi4) {
				if (sortedList.insert(rsi).second) {
					cout << " ";
				} else {
					++d;
					cout << "-";
				}
				cout << setw(3) << n << "." << ++v << " series: ";
				rsi.Dump();
				cout << endl;
			}
		}
		cout << endl;
		cout << d << " duplicate eliminated out of " << 4 * n << ", remained: " << 4 * n - d << endl;
		cout << endl;
		size_t m = 0;
		for (auto & slm : sortedList) {
			cout << setw(3) << ++m << ". sorted: ";
			slm.Dump();
			cout << endl;
		}
	}

	//
	// Recursive algorithm to solve the problem
	//
	void SolveDodekafon (const Spans&					sRef,
						 vector<Spans>&					result)
	{
		if (sRef.Size() < MaxEdge) {

			for (size_t intervalLength = 1;
				 intervalLength < EdgeLimit;
				 ++intervalLength) {
				if (sRef.IsEdgeAvailable(intervalLength)) {
					// check for both direction
					for (const auto direction : Directions) {
						// Create a copy for each possible tree branch
						Spans ls(sRef);
						// Try to add the actual interval to the spans registry
						if (ls.AddInterval(intervalLength,
										   direction)) {
								// Recurse on success with one shorter interval
							SolveDodekafon (ls,
											result);
						}
					}

				}
			}
		} else {
			// If we managed to get here than we have a possible new resulting span
			Spans ls;
			if (sRef.CopyValidSpan(ls)) {
				result.push_back(ls);
			}
		}
	}

	void SolveDodekafon (vector<Spans>& result)
	{
		// exclude the mirrored sequences
		for (size_t pitch = 1; pitch < (MaxPitch + (MaxPitch & 1) / 2) + 1; ++pitch) {
			Spans	spans { pitch };
			SolveDodekafon (spans,
							result);
		}

	}
}

using namespace Dodekafon;
/// @brief  This is the main entry point of a C or C++ program
/// @return The return value is an integer, automatically provided as 0, when the extlicit return is omitterd
int main ()
{
	std::vector<Spans>    result;

	try {
			SolveDodekafon(result);

		cout << "Dodekafon 1 --> " << MaxPitch << endl;

		PrintDodekafon (result);
	}
	catch (std::exception& e) {
		std::cerr << "exception caught: " << e.what() << endl;
	}
	catch (...) {
		cout << "Unexpected error occured!" << endl;
	}
}
