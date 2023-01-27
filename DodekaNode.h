//
// DodekaNode.h
//
#pragma once

#include <cstddef>
#include <limits>
#include <string>
#include <vector>

namespace Dodekafon {

	using std::string;
	using std::vector;

	enum class PitchStatus : size_t {
		Empty = std::numeric_limits<size_t>::max(),
		Invalid = std::numeric_limits<size_t>::max() - 1,
		ValidLeafpoint = std::numeric_limits<size_t>::max() - 2,
		ValidMidpoint = std::numeric_limits<size_t>::max() - 4
	};

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

		bool	IsEmpty ()								const;
		bool	IsAvailableForIntervalAddition ()		const;
		bool	IsLeafPoint ()							const;
		bool	IsMidPoint ()							const;

		string	GetStatusWord ()						const;
		size_t	GetNextInterval (size_t prevInterval)	const;

	//modifiers
		inline void	SetPitch		(size_t			newPitch);
		inline void	SetPitchStatus	(PitchStatus	newPitchStatus);

		bool	AddInterval		(size_t				newInterval);

		bool	InitLeafNode	(size_t				width,
								 size_t				pitch);

		bool	InitMiddleNode	(size_t				width,
								 size_t				pitch);
	};

	//
	// Node inline accessors
	//
	inline size_t	Node::GetPitch ()	const
	{
		return	m_pitch;
	}

	inline size_t	Node::GetInterval (size_t	i)	const
	{
		return	i < m_intervals.size() ? m_intervals[i] : 0;
	}

	inline size_t	Node::GetNumIntervals ()	const
	{
		return	m_intervals.size();
	}

	inline PitchStatus	Node::GetPitchStatus ()	const
	{
		return	m_status;
	}
}
