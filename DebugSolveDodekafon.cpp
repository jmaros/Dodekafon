//
// DebugSolveDodekafon.h
//
#include <algorithm>
#include <iomanip>
#include <iostream>

#include "DebugSolveDodekafon.h"

#if defined (DEBUG_SOLVE_DODEKAFON)

#include <vector>

namespace Dodekafon {

	using std::vector;

	[[maybe_unused]]
	bool operator < (DebugSolveDodekafon lv,
					 DebugSolveDodekafon rv)
	{
		return ((lv.m_edge < rv.m_edge) ||
				(lv.m_edge == rv.m_edge &&
				 lv.m_vertex < rv.m_vertex));
	}

	bool operator == (DebugSolveDodekafon lv,
					  DebugSolveDodekafon rv)
	{
		return (lv.m_edge == rv.m_edge &&
				lv.m_vertex == rv.m_vertex);
	}

	//[[maybe_unused]]
	//bool operator != (DebugSolveDodekafon lv,
	//				  DebugSolveDodekafon rv)
	//{
	//	return (lv.m_edge	!= rv.m_edge ||
	//			lv.m_vertex	!= rv.m_vertex);
	//}

	//   3,  10,   2,  11,   1,  12,   6,   7,   5,   8,   4,   9
	//	 7    8    9    10   11    6    1    2    3    4    5

#if defined(ELENDIR)
	vector<DebugSolveDodekafon> DodekafonToDebugSolveDodekafon (const Array12& dodekafon)
	{
		vector<DebugSolveDodekafon> retv;
		for (size_t i = 1; i < dodekafon.size(); ++i) {
			size_t		first = dodekafon[i - 1];
			size_t		second = dodekafon[i];
			size_t		edge = Distance<size_t>(dodekafon[i],
												dodekafon[i - 1]);
			size_t		vertex = dodekafon[i - 1];
			Direction	dir = ((first < second) ? Direction::Up
							   : ((first > second) ? Direction::Down
								  : Direction::Invalid));
			retv.push_back(DebugSolveDodekafon(vertex, edge, dir));
		}
		std::sort(retv.begin(),
				  retv.end(),
				  [] (const DebugSolveDodekafon& left,
					  const DebugSolveDodekafon& right) -> bool
				  {
					  return left.m_edge > right.m_edge;
				  });
		auto separator = "\tArray11{";
		for (const auto& dsd : retv) {
			std::cout << separator
				<< "mt(" << dsd.m_edge
				<< "," << dsd.m_vertex
				<< "," << UnderlyingType(dsd.m_edgeDirection) + 1
				<< ")";
			separator = ",";
		}
		std::cout << "}," << std::endl;
		return retv;
	}

	vector<vector<DebugSolveDodekafon>> GenerateDebugSolveDodekafonRefs ()
	{
		vector<vector<DebugSolveDodekafon>> debugSolveDodekafonRefs;
		std::cout << "#define mt(a, b, c) std::make_tuple(size_t(a), size_t(b), int(c - 1))" << std::endl;
		std::cout << std::endl;
		std::cout << "const std::array<Array11,1328> Testdata11 {" << std::endl;

		for (const auto& dodekafon : Testdata12) {
			auto dsds = DodekafonToDebugSolveDodekafon(dodekafon);
			debugSolveDodekafonRefs.push_back(dsds);
		}

		std::cout << "};" << std::endl;
		return debugSolveDodekafonRefs;
	}
#endif

	[[nodiscard("Forcing the use of the return value! (C++20)")]]
	// returns true if there is a matching pattern in debugSolveDodekafonRefs
	bool debugSolveDodekafonCheck (vector<DebugSolveDodekafon> debugSolveDodekafons)
	{
		bool result{};
#if defined(ELENDIR)
		static const auto DebugSolveDodekafonRefs = GenerateDebugSolveDodekafonRefs();
		for (const auto& debugSolveDodekafonRef : DebugSolveDodekafonRefs) {
			auto debugSolveDodekafonRefIter = debugSolveDodekafonRef.cbegin();
			bool isDifferent{};
			for (const auto& debugSolveDodekafon : debugSolveDodekafons) {
				if (debugSolveDodekafonRefIter != debugSolveDodekafonRef.cend()) {
					if (debugSolveDodekafon == *debugSolveDodekafonRefIter) {
						++debugSolveDodekafonRefIter;
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
#else //!ELENDIR
		size_t breki{};
		size_t breko{};
		size_t testdataIndex{};
		// Testdata11 is const std::array<Array11, 1328>
		for (const auto& Data11 : Testdata11) {
			++testdataIndex;
			// initialize an iterator for the rejected sequence:
			auto debugSolveDodekafonRefIter = debugSolveDodekafons.cbegin();
			for (const auto& Data : Data11) {
				size_t	interval	= std::get<0>(Data);
				size_t	edge		= std::get<1>(Data);
				int		direction	= std::get<2>(Data);
				if (Direction(direction) == Direction::Down) {
					if (edge > interval) {
						edge -= interval;
					} else {
						++breki;
					}
				}
				DebugSolveDodekafon dsd (edge, interval, direction);
				if (debugSolveDodekafonRefIter->m_edge == dsd.m_edge &&
					debugSolveDodekafonRefIter->m_vertex == dsd.m_vertex) {
					if (UnderlyingType(debugSolveDodekafonRefIter->m_edgeDirection) != direction) {
						// Other direction
						++breko;
					}
				} else {
					// different interval or edge
					break;
				}
				++debugSolveDodekafonRefIter;
				if (debugSolveDodekafonRefIter == debugSolveDodekafons.cend()) {
					// matches all the way down
					std::cout << " Index = " << std::setw(4) << testdataIndex << " ";
					result = true;
					break;
				}
			}
			if (result == true) {
				break;
			}
		}
		if (breko) {
			static size_t brekos{};
			brekos += breko;
		}
#endif //ELENDIR
		return result;
	}
}

#endif // DEBUG_SOLVE_DODEKAFON
