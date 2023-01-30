//
// NodeAndEdge.cpp
//
#include <stdexcept>

#include "NodeAndEdge.h"

namespace Dodekafon {
	//
	// Interval accessors
	//
	bool	NodeAndEdge::IsEmpty () const
	{
		return	(m_pitch == 0);
	}

	bool	NodeAndEdge::IsValidPitch (size_t	pitch)
	{
		return pitch < PitchLimit;
	}

	bool	NodeAndEdge::IsValidWidth (size_t width)
	{
		return width < EdgeLimit;
	}

	bool	NodeAndEdge::IsValidDirection (Direction	direction)
	{
		return Directions[0] == direction || Directions[1] == direction;
	}

	bool	NodeAndEdge::IsValid () const
	{
		if (!IsValidPitch(m_pitch)) {
			return false;
		}
		if (!IsValidWidth (m_width)) {
			return false;
		}
		if (m_direction == Direction::Down) {
			if (m_pitch > m_width &&
				m_pitch - m_width < MaxPitch) {
				return true;
			}
		} else if (m_direction == Direction::Up) {
			if (m_pitch + m_width < MaxPitch) {
				return true;
			}
		}
		return false;
	}

	size_t	NodeAndEdge::GetWidth () const
	{
		return	m_width;
	}

	// return the connecting pitch
	size_t	NodeAndEdge::GetPitch () const
	{
		return	m_pitch;
	}

	size_t	NodeAndEdge::GetNextPitch () const
	{
		size_t	nextPitch{};

		if (m_direction == Direction::Down) {
			if (m_pitch > m_width) {
				nextPitch = m_pitch - m_width;
			} else {
				throw (std::domain_error("NodeAndEdge::GetNextPitch#01"));
			}
		} else if (m_direction == Direction::Up) {
			nextPitch = m_pitch + m_width;
		}
		if (nextPitch > MaxPitch) {
			throw (std::domain_error("NodeAndEdge::GetNextPitch#02"));
		}
		return nextPitch;
	}

	//
	// Interval modifiers
	//
	void NodeAndEdge::SetWidth (size_t newWidth)
	{
		if (newWidth < EdgeLimit) {
			m_width = newWidth;
		} else {
			throw (std::domain_error("SetWidth"));
		}
	}

	void NodeAndEdge::SetPitch (size_t newPitch)
	{
		if (newPitch < PitchLimit) {
			m_pitch = newPitch;
		} else {
			throw (std::domain_error("SetPitch"));
		}
	}

	void NodeAndEdge::SetDirection	(Direction	newDir)
	{
		if (newDir == Direction::Down ||
			newDir == Direction::Up) {
			m_direction = newDir;
		} else {
			throw (std::domain_error("SetDirection"));
		}
	}

	void	NodeAndEdge::Init	(size_t		width,
								 size_t		firstNode,
								 Direction	dir)
	{
		SetWidth(width);
		SetPitch(firstNode);
		SetDirection(dir);
	}

}
