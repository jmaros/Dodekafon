//
// DebugSolveDodekafon.h
//
#pragma once

#include "Testdata.h"
#include "DodekaNode.h"

#if defined (DEBUG_SOLVE_DODEKAFON)

#include <string>
#include <vector>

namespace Dodekafon {

	using std::string;
	using std::vector;

	enum class Direction : int {
		Up		= 1,
		Invalid	= 0,
		Down	= -1
	};
	struct DebugSolveDodekafon {
		size_t		m_vertex{};
		size_t		m_edge{};
		Direction	m_edgeDirection{};

		DebugSolveDodekafon (size_t		vertex,
							 size_t		edge,
							 Direction	dir)
			: m_vertex(vertex)
			, m_edge(edge)
			, m_edgeDirection(dir)
		{
		};
		DebugSolveDodekafon (size_t		vertex,
							 size_t		edge,
							 int		dir)
			: m_vertex(vertex)
			, m_edge(edge)
			, m_edgeDirection(Direction(dir))
		{
		};
	};

	bool operator < (DebugSolveDodekafon lv,
					 DebugSolveDodekafon rv);

	bool operator == (DebugSolveDodekafon lv,
					  DebugSolveDodekafon rv);

	//bool operator != (DebugSolveDodekafon lv,
	//				  DebugSolveDodekafon rv);

	vector<DebugSolveDodekafon> DodekafonToDebugSolveDodekafon (const Array12& dodekafon);
	vector<vector<DebugSolveDodekafon>> GenerateDebugSolveDodekafonRefs();

	// returns true if there is a matching pattern in debugSolveDodekafonRefs
	bool debugSolveDodekafonCheck (vector<DebugSolveDodekafon>	debugSolveDodekafons,
								   const Node &					nodi,
								   const Node &					nodj,
								   string &						additionalInfo);
}

#endif // DEBUG_SOLVE_DODEKAFON
