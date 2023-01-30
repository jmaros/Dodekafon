//
// DodekaSpan.h
//
#pragma once
#include <vector>

#include "NodeAndEdge.h"
#include "DodekaPitches.h"

namespace Dodekafon {

	using std::vector;

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.


	// Why cant find e. g. this:
	//   3,  10,   2,  11,   1,  12,   6,   7,   5,   8,   4,   9
	//	    7    8    9    10   11    6    1    2    3    4    5
	// Internal:
	//  2, 9, 1, 10, 0, 11, 5, 6, 4, 7, 3, 8

	class Spans {
		array<size_t, MaxEdge>	m_edges{};
		size_t					m_firstPitch{};
		Pitches					m_pitches;		// The value of "m_pitches[pitch - 1]" must be either 0,
												// or pitch. This is used as an inventory of pitches.
		vector<NodeAndEdge>		m_nodeAndEdges;	// A sequence of nodes(pitchel) and edges(intervals)
												// the last pitch is not stored, it has to be derived.
	public:
	//constructors
		explicit Spans(size_t	firstPitch);
		// use defaults
		Spans() = default;

	//accessors
		size_t					Size				()					const;
		const	NodeAndEdge &	GetNodeAndEdge		(size_t		index)	const;
		bool					IsPitchAvailable	(size_t		pitch)	const;
		bool					IsEdgeAvailable		(size_t		edge)	const;

		bool			IsValidSpan ()			const;
		Pitches			ExtractPitches ()		const;

	//modifiers
		bool	AddInterval (size_t		intervalLength,
							 Direction	direction);
	//utilities
		vector<Pitches>	GenerateVariations ()		const;
		bool			CopyValidSpan (Spans& ls)	const;
	};
}
