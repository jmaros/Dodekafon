//
// DodekaNode.cpp
//

#include "DodekaNode.h"

namespace Dodekafon {
	//
	// Node constructors
	//
	Node::Node() {}
	Node::Node(size_t note)
		: m_pitch(note)
		, m_status(PitchStatus::Invalid)
	{
	}

	Node::Node (size_t			note,
				PitchStatus		status)
		: m_pitch	(note)
		, m_status	(status)
	{
	}

	//
	// Node accessors
	//
	bool		Node::IsEmpty ()				const
	{
		return	m_pitch == 0 &&
			m_intervals.size() == 0 &&
			GetPitchStatus() == PitchStatus::Empty;
	}

	bool		Node::IsAvailableForIntervalAddition ()	const
	{
		return	(m_intervals.size() < 2) &&
			GetPitchStatus() != PitchStatus::ValidMidpoint;
	}

	bool		Node::IsLeafPoint ()	const
	{
		return	(m_intervals.size() == 1) &&
			GetPitchStatus() == PitchStatus::ValidLeafpoint;
	}

	bool		Node::IsMidPoint ()	const
	{
		return	(m_intervals.size() == 2) &&
			GetPitchStatus() == PitchStatus::ValidMidpoint;
	}

	size_t		Node::GetNextInterval(size_t prevInterval) const
	{
		size_t retv{};
		if (IsLeafPoint()) {
			if (prevInterval == 0) {
				retv = GetInterval(0);
			}
		} else {
			if (IsMidPoint()) {
				if (prevInterval != 0) {
					retv = GetInterval(0);
					if (retv == prevInterval) {
						retv = GetInterval(1);
					} else {
						if (GetInterval(1) != prevInterval) {
							// one of the intervals must be the same as the prevInterval
							retv = 0;
						}
					}
				}
			}
		}
		return retv;
	}

	//
	// Node modifiers
	//
	inline void	Node::SetPitch (size_t		newPitch)
	{
		m_pitch = newPitch;
	}

	inline void	Node::SetPitchStatus (PitchStatus newPitchStatus)
	{
		m_status = newPitchStatus;
	}

	bool	Node::AddInterval (size_t		newInterval)
	{
		bool success = false;
		if (IsAvailableForIntervalAddition()) {
			m_intervals.push_back(newInterval);
			if (m_intervals.size() == 1) {
				SetPitchStatus(PitchStatus::ValidLeafpoint);
				success = true;
			} else {
				if (m_intervals.size() == 2) {
					SetPitchStatus(PitchStatus::ValidMidpoint);
					success = true;
				} else {
					SetPitchStatus(PitchStatus::Invalid);
				}
			}
		}
		return success;
	}

	bool	Node::InitLeafNode (size_t	width,
								size_t	pitch)
	{
		bool success{ IsEmpty() };

		if (success) {
			SetPitch(pitch);
			success = AddInterval (width);
		}
		return success;
	}

	bool	Node::InitMiddleNode	(size_t width,
									 size_t pitch)
	{
		bool success{
			IsLeafPoint() &&
			m_intervals[0] != width &&
			GetPitch() == pitch
		};

		if (success) {
			if (AddInterval(width)) {
				SetPitchStatus (PitchStatus::ValidMidpoint);
			} else {
				SetPitchStatus (PitchStatus::Invalid);
			}
		}
		return success;
	}
}
