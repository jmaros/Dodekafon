// Dodekafon.h

#pragma once
#include <vector>

namespace Dodekafon {

	using std::set;
	using std::vector;

	const size_t				SpanWidth{ 12 };
	const size_t				MaxIntervalLength{ SpanWidth - 1 };		// Downward

	enum class PitchStatus : size_t {
		Empty = std::numeric_limits<size_t>::max(),
		Invalid = std::numeric_limits<size_t>::max() - 1,
		ValidLeafpoint = std::numeric_limits<size_t>::max() - 2,
		ValidMidpoint = std::numeric_limits<size_t>::max() - 4
	};

	// The goal is to find all such series, where the elements are
	// all the integer values of the [1,12] closed interval, and the absolut values
	// of the differences are all the elements of the [1, 11] closed interval, are representing all the 11 spans.

	class Node {
		size_t			m_pitch{};						// Integer 1..12 representing the pitch, 0 denotes the emptyness
		vector<size_t>	m_intervals;					// Two Integers 1..11 representing the connected intervals
		PitchStatus		m_status{ PitchStatus::Empty };
	public:
	//constructors
		Node();
		Node(size_t note);

		Node(size_t			note,
			 PitchStatus	status);

	//accessors
		inline size_t		GetPitch ()					const;
		inline size_t		GetInterval (size_t	i)		const;
		inline size_t		GetNumIntervals ()			const;
		inline PitchStatus	GetPitchStatus ()			const;

		size_t	GetNextInterval (size_t prevInterval)	const;

		bool	IsEmpty ()								const;
		bool	IsAvailableForIntervalAddition ()		const;
		bool	IsLeafPoint ()							const;
		bool	IsMidPoint ()							const;

	//modifiers
		inline void	SetPitch		(size_t			newPitch);
		inline void	SetPitchStatus	(PitchStatus	newPitchStatus);

		bool	AddInterval		(size_t				newInterval);

		bool	InitLeafNode	(size_t				width,
								 size_t				pitch);

		bool	InitMiddleNode	(size_t				width,
								 size_t				pitch);
	};

	class Interval {
		size_t			m_width{};	// Integer 0..11 representing the pitch difference,	0 denotes the emptyness (m_width = abs(m_pitches[0] - m_pitches[1]))
		vector<size_t>	m_pitches;	// Integer 0..12 representing the connecting pitches, 0 denotes the emptyness (exactly 2 values)
	public:
	//constructors
		Interval () {}

	//accessors
		bool		IsEmpty			()					const;
		size_t		GetWidth		()					const;
		size_t		GetPitch		(size_t index)		const;
		size_t		GetNextPitch	(size_t prevPitch)	const;

	//modifiers
		Interval&	SetWidth	(size_t		newWidth);
		Interval&	AddPitch	(size_t		newPitch);

		void		Init	(size_t		width,
							 size_t		lower);

	//operators
		Interval& operator = (const Interval&) = default;
	};

	class Pitches {
		vector<size_t>		m_pitches;		// Integer 1..12 representing the pitch,
											// 0 denotes the emptyness
											// Note: this vector is indexed 0-based indices
	public:
	//constructors
		Pitches ();
		explicit Pitches (size_t numPitches);
		Pitches (const Pitches& pitchesPar);

	//accessors
		size_t Size () const;
		size_t GetPitch (size_t i) const;

	//modifiers
		bool SetPitch (size_t i,
					   size_t pitchPar);

	//transformations
		Pitches			GenerateInversion()				const;
		Pitches			GenerateRetrograde()			const;
		Pitches			GenerateRetrogradeInversion()	const;
		void			Dump()							const;

		friend bool operator < (const Pitches& pitchesLeft,
								const Pitches& pitchesRight);

		friend bool operator == (const Pitches& pitchesLeft,
								 const Pitches& pitchesRight);
	};

	//global scope operators
		bool operator < (const Pitches& pitchesLeft,
						 const Pitches& pitchesRight);

		bool operator == (const Pitches& pitchesLeft,
						  const Pitches& pitchesRight);

	// Why cant find e. g. this:
	//   3,  10,  2, 11,  1,  12,   6,  7,   5,  8,    4,  9,
	//	    7    8  9   10  11    6    1   2    3    4    5
	// Internal:
	//  2, 9, 1, 10, 0, 11, 5, 6, 4, 7, 3, 8,

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
		const Node&		GetNode (size_t i)		const;
		const Interval& GetInterval (size_t i)	const;
		Node			FindFirstNode ()		const;

		Node			FindNextNode (const Node&		prevNode,
									  const Interval&	prevInter) const;

		bool			IsValidSpan ()			const;
		Pitches			ExtractPitches()		const;

	//modifiers
		Node& NodeRef (size_t i);
		Interval& IntervalRef (size_t intervalWidth);

		bool	AddInterval (size_t		intervalLength,
							 size_t		lowerPich);
	//utilities
		vector<Pitches>	GenerateVariations ()	const;
	};


}
