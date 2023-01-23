//
// DodekaSpan.h
//
#pragma once

#include "DodekaNode.h"
#include "DodekaInterval.h"
#include "DodekaPitches.h"

namespace Dodekafon {

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.


	// Why cant find e. g. this:
	//   3,  10,   2,  11,   1,  12,   6,   7,   5,   8,   4,   9
	//	    7    8    9    10   11    6    1    2    3    4    5
	// Internal:
	//  2, 9, 1, 10, 0, 11, 5, 6, 4, 7, 3, 8

	class Spans {
		vector<Node>		m_nodes;
		vector<Interval>    m_intervals;
	public:
	//constructors
		Spans	(size_t n);
		Spans	(const Spans& parent);

	//accessors
		size_t			NodesSize ()			const;
		size_t			IntervalsSize ()		const;
		size_t			SpanSize ()				const;
		const Node& GetNode (size_t i)		const;
		const Interval& GetInterval (size_t i)	const;
		Node			FindFirstNode ()		const;

		Node			FindNextNode (const Node& prevNode,
									  const Interval& prevInter) const;

		bool			IsValidSpan ()			const;
		Pitches			ExtractPitches ()		const;

	//modifiers
		Node& NodeRef (size_t i);
		Interval& IntervalRef (size_t intervalWidth);

		bool	AddInterval (size_t		intervalLength,
							 size_t		lowerPich);
	//utilities
		vector<Pitches>	GenerateVariations ()		const;
		bool			CopyValidSpan (Spans& ls)	const;
	};
}
