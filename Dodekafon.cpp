//
// Dodekafon.cpp
//

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp -o Dodekafon.exe ; .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time ..\Dodekafon.cpp ..\DebugPrint.cpp ..\DodekaInterval.cpp ..\DodekaNode.cpp ..\DodekaPitches.cpp ..\DodekaSpan.cpp -o Dodekafon.exe ; gdb .\Dodekafon.exe

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

#define DEBUG_SOLVE_DODECAFON
#if defined (DEBUG_SOLVE_DODECAFON)
	struct DebugSolveDodecafon {
		size_t	m_vertex{};
		size_t	m_edge{};

		DebugSolveDodecafon (size_t		vertex,
							 size_t		edge)
			:	m_vertex(vertex)
			,	m_edge(edge)
		{
		};
	};
	//bool operator < (DebugSolveDodecafon lv,
	//				 DebugSolveDodecafon rv)
	//{
	//	return ((lv.m_edge		<	rv.m_edge) ||
	//			(lv.m_edge		==	rv.m_edge &&
	//			 lv.m_vertex	<	rv.m_vertex));
	//}
	bool operator == (DebugSolveDodecafon lv,
					  DebugSolveDodecafon rv)
	{
		return (lv.m_edge	== rv.m_edge &&
				lv.m_vertex	== rv.m_vertex);
	}
	//bool operator != (DebugSolveDodecafon lv,
	//				  DebugSolveDodecafon rv)
	//{
	//	return (lv.m_edge	!= rv.m_edge ||
	//			lv.m_vertex	!= rv.m_vertex);
	//}
	vector<vector<DebugSolveDodecafon>> DebugSolveDodecafonRefs0 {
	//   3,  10,   2,  11,   1,  12,   6,   7,   5,   8,   4,   9
	//	    7    8    9    10   11    6    1    2    3    4    5
		{{ 1,11}, {11,10}, { 2, 9}, {10, 8}, { 3, 7}, {12, 6},
		 { 6, 1}, { 7, 2}, { 5, 3}, { 8, 4}, { 4, 5}},
		{{ 1,11}, { 1,10}, { 2, 9}, { 2, 8}, { 3, 7}, { 6, 6},
		 { 6, 1}, { 7, 2}, { 5, 3}, { 8, 4}, { 4, 5}}
	};

	vector<DebugSolveDodecafon> DodekafonToDebugSolveDodecafon (const Array12& dodekafon)
	{
		vector<DebugSolveDodecafon> retv;
		/* TODO */ (dodekafon);
		return retv;
	}

	vector<vector<DebugSolveDodecafon>> GenerateDebugSolveDodecafonRefs()
	{
		vector<vector<DebugSolveDodecafon>> debugSolveDodecafonRefs;
		for (const auto & dodekafon : Testdata12) {
			auto dsds = DodekafonToDebugSolveDodecafon(dodekafon);
			debugSolveDodecafonRefs.push_back(dsds);
		}
		return debugSolveDodecafonRefs;
	}

	// returns true if there is a matching pattern in debugSolveDodecafonRefs
	bool debugSolveDodecafonCheck (vector<DebugSolveDodecafon> debugSolveDodecafons)
	{
		bool result{};
		const auto DebugSolveDodecafonRefs = GenerateDebugSolveDodecafonRefs();
		for (const auto & debugSolveDodecafonRef : DebugSolveDodecafonRefs) {
			auto debugSolveDodecafonRefIter = debugSolveDodecafonRef.cbegin();
			bool isDifferent{};
			for (const auto & debugSolveDodecafon : debugSolveDodecafons) {
				if (debugSolveDodecafonRefIter != debugSolveDodecafonRef.cend()) {
					if (debugSolveDodecafon == *debugSolveDodecafonRefIter) {
						++debugSolveDodecafonRefIter;
					} else {
						// this pattern doesn't match, lets check the next one
						isDifferent = true;
						break;
					}
				} else {
					result = true;
				}
			}
			if (!isDifferent) {
				result = true;
			}
			if (result) {
				break;
			}
		}
		return result;
	}
#endif

	//
	// Recursive algorithm to solve the problem
	//
	void SolveDodekafon (size_t				intervalWidth,
						 const Spans&		sRef,
#if defined (DEBUSOL)
						 vector<DebuSol>&	debuSols,
#endif
						 vector<Spans>&		result)
	{
		if (intervalWidth > 0) {

			for (size_t i = 1; i + intervalWidth < sRef.NodesSize(); ++i) {
				// Create a copy for each possible tree branch
				Spans ls(sRef);
#if defined (DEBUSOL)
				const auto& nodi = ls.GetNode(i);
				const auto& nodj = ls.GetNode(i + intervalWidth);
#endif
			// Try to add the actual interval to the spans registry
				if (ls.AddInterval(intervalWidth, i)) {
#if defined (DEBUSOL)
					debuSols.push_back(DebuSol(nodi.GetPitch(), intervalWidth));
#endif
						// Recurse on success with one shorter interval
					SolveDodekafon (intervalWidth - 1,		// recursion, Downward
									ls,
#if defined (DEBUSOL)
									debuSols,
#endif
									result);
#if defined (DEBUSOL)
				} else {
					if (DebuSolCheck (debuSols)) {
						DebugLine(", Rejected sequence length = ",
								  setw(2), debuSols.size());
						DebugPrint(" {vert1,edge1} =",
								   setw(2), debuSols.front().m_vertex);
						DebugPrint(",",
								   setw(2), debuSols.front().m_edge);
						DebugPrint("}");
								   if (debuSols.size() > 2) {
							DebugPrint(" Other {vertex,edge} pairs: ");
							for (auto vep = 1U; vep + 1 < debuSols.size(); ++vep) {
								DebugPrint("  {",
										   setw(2), debuSols[vep].m_vertex, ",",
										   setw(2), debuSols[vep].m_edge, "}");
							}
						}

						DebugPrint(" Last {vertex, edge} pair ={",
								   setw(2), debuSols.back().m_vertex, ",",
								   setw(2), debuSols.back().m_edge, "}");
						DebugPrint(" At interval = ",
								   setw(2), intervalWidth, "\n");
					}
					if (!nodi.IsMidPoint() && !nodj.IsMidPoint()) {
						const auto stat1 = (nodi.IsMidPoint() ? " MiddlePoint" : "git add     ");
						const auto stat2 = (nodj.IsMidPoint() ? " MiddlePoint" : "git add     ");
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
#if defined (DEBUSOL)
		vector<DebuSol> debugs;
#endif
			SolveDodekafon(MaxIntervalLength,
					   spans,
#if defined (DEBUSOL)
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
