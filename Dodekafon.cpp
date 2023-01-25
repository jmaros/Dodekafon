//
// Dodekafon.cpp
//

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DebugSolveDodekafon.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp ..\Testdata.cpp -o Dodekafon.exe && .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DebugSolveDodekafon.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp ..\Testdata.cpp -o Dodekafon.exe && gdb .\Dodekafon.exe

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
	void SolveDodekafon (size_t							intervalWidth,
						 const Spans&					sRef,
#if defined (DEBUG_SOLVE_DODEKAFON)
						 vector<DebugSolveDodekafon>&	debuSols,
#endif
						 vector<Spans>&					result)
	{
		if (intervalWidth > 0) {

			for (size_t i = 1; i + intervalWidth < sRef.NodesSize(); ++i) {
				// Create a copy for each possible tree branch
				Spans ls(sRef);
#if defined (DEBUG_SOLVE_DODEKAFON)
				const auto& nodi = ls.GetNode(i);
				const auto& nodj = ls.GetNode(i + intervalWidth);
#endif
			// Try to add the actual interval to the spans registry
				if (ls.AddInterval(intervalWidth, i)) {
#if defined (DEBUG_SOLVE_DODEKAFON)
					debuSols.push_back(DebugSolveDodekafon(nodi.GetPitch(), intervalWidth, Direction::Up));
#endif
						// Recurse on success with one shorter interval
					SolveDodekafon (intervalWidth - 1,		// recursion, Downward
									ls,
#if defined (DEBUG_SOLVE_DODEKAFON)
									debuSols,
#endif
									result);
#if defined (DEBUG_SOLVE_DODEKAFON)
				} else {
					if (debugSolveDodekafonCheck (debuSols)) {
						DebugLine(", Rejected length = ",
								  setw(2), debuSols.size(),
								  " Interval = ",
								  setw(2), intervalWidth);
						DebugPrint(" {vertex,edge} pairs: ");
						for (auto vep = 0u; vep < debuSols.size(); ++vep) {
							DebugPrint("{",
										setw(2), debuSols[vep].m_vertex, ",",
										setw(2), debuSols[vep].m_edge, "}");
						}

						DebugPrint("\n");
					}
					if (!nodi.IsMidPoint() && !nodj.IsMidPoint()) {
						const auto stat1 = (nodi.IsMidPoint() ? " MiddlePoint" : "            ");
						const auto stat2 = (nodj.IsMidPoint() ? " MiddlePoint" : "            ");
						DebugNewLine(", Rejected interval = ",
									 setw(2), intervalWidth,
									 ". Pos(", setw(2), i, ") = ",
									 setw(2), nodi.GetPitch(), stat1,
									 "; Pos(", setw(2), i + intervalWidth, ") = ",
									 setw(2), nodj.GetPitch(), stat2);
					}
#endif
				}
			}
		} else {
			// If we managed to get here than we have a possible new resulting span
			Spans ls(sRef.SpanSize());
			if (sRef.CopyValidSpan(ls)) {
				result.push_back(ls);
			}
		}
	}
}

using namespace Dodekafon;
/// @brief  This is the main entry point of a C or C++ program
/// @return The return value is an integer, automatically provided as 0, when the extlicit return is omitterd
int main ()
{
	Spans			     spans(SpanWidth);
	std::vector<Spans>    result;

	try {
#if defined (DEBUG_SOLVE_DODEKAFON)
		vector<DebugSolveDodekafon> debugs;
#endif
			SolveDodekafon(MaxIntervalLength,
					   spans,
#if defined (DEBUG_SOLVE_DODEKAFON)
					   debugs,
#endif
					   result);

		cout << "Dodekafon 1 --> " << SpanWidth << endl;

		PrintDodekafon (result);
	}
	catch (...) {
		cout << "Unexpected error occured!" << endl;
	}
}
