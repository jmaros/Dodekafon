//
// DodekaSpan.h
//
#include <exception>
#include <iomanip>
#include <iostream>
#include <set>

#include "DodekaSpan.h"
#include "DodekaBase.h"

using std::cout;
using std::set;
using std::setw;

namespace Dodekafon {

	//
	// Spans constructors
	//
	Spans::Spans(size_t	firstPitch)
		:	m_firstPitch	(firstPitch)
	{
		if (firstPitch < 1 || firstPitch > MaxPitch) {
			throw std::out_of_range("Spans::Spans");
		}
	}

	//
	// Spans accessors
	//
	size_t	Spans::Size () const
	{
		return m_nodeAndEdges.size();
	}

	const NodeAndEdge& Spans::GetNodeAndEdge (size_t	index) const
	{
		if (index >= m_nodeAndEdges.size()) {
			throw std::out_of_range("GetNodeAndEdge");
		}
		return m_nodeAndEdges[index];
	}

	bool	Spans::IsPitchAvailable	(size_t		pitch) const
	{
		if (pitch > MaxPitch) {
			throw std::out_of_range("IsPitchAvailable");
		}
		return m_pitches.GetPitch(pitch - 1) == 0;
	}

	bool	Spans::IsEdgeAvailable (size_t		edge) const
	{
		if (edge > MaxEdge) {
			throw std::out_of_range("IsEdgeAvailable");
		}
		return m_edges[edge - 1] == 0;
	}

	bool Spans::IsValidSpan () const
	{
		bool success = Size() > 0;
		if (success) {
			set<size_t> nodeIndices;
			set<size_t> intervalIndices;

			for (size_t index = 1; index < Size(); ++index) {
				const auto & nodedge = GetNodeAndEdge(index);
				auto pitchi = nodedge.GetPitch();
				if (nodeIndices.insert(pitchi).second == false) {
					return false;
				}
				auto nexti = nodedge.GetWidth();
				if (nexti) {
					if (intervalIndices.insert(nexti).second == false) {
						return false;
					}
					auto nextPitchi = nodedge.GetNextPitch();
					if (nextPitchi == 0) {
						return false;
					}
				}
			}
		}
		return success;
	}

	//
	// Spans modifiers
	//
	bool	Spans::AddInterval (size_t		intervalLength,
								Direction	direction)
	{
		bool success{ false };
		if (intervalLength > 0 &&
			intervalLength < EdgeLimit &&
			IsEdgeAvailable(intervalLength)) {
			// valid interval length
			size_t firstPitch{ m_firstPitch };
			if (Size() > 0) {
				NodeAndEdge last = m_nodeAndEdges.back();
				firstPitch = last.GetNextPitch();
			}

			NodeAndEdge newNodeAndAngle;
			newNodeAndAngle.Init(intervalLength,
								 firstPitch,
								 direction);

			// check whether the first pich is still available
			if (newNodeAndAngle.IsValid()		&&
				IsPitchAvailable(firstPitch)	&&
				IsPitchAvailable(newNodeAndAngle.GetNextPitch())) {
				m_firstPitch = newNodeAndAngle.GetNextPitch();
				m_pitches.SetPitch(firstPitch - 1, firstPitch);
				m_edges[intervalLength - 1] = intervalLength;
				m_nodeAndEdges.push_back(newNodeAndAngle);
				success = true;
			}

		}
		return success;
	}

	bool Spans::CopyValidSpan(Spans& lsParam) const
	{
		bool success{};
		if (IsValidSpan()) {
			lsParam = *this;
			success = true;
		}
		return success;
	}

	//
	// Converting to time based representation of the notes
	//
	Pitches Spans::ExtractPitches() const
	{
		Pitches		result;
		if (Size() > 0 &&
			Size() < EdgeLimit) {
			 size_t nextPitch = GetNodeAndEdge(0).GetPitch();
		//	 if (nextPitch != m_firstPitch) {
		//		 throw  std::logic_error("Spans::ExtractPitches#1");
		//	 }
			 size_t pitchIndex{};
			for (const auto& nodeAndEdge : m_nodeAndEdges) {
				if (nextPitch != nodeAndEdge.GetPitch()) {
					throw  std::logic_error("Spans::ExtractPitches");
				}
				result.SetPitch(pitchIndex++, nextPitch);
				nextPitch = nodeAndEdge.GetNextPitch();
			}
		}
		return result;
	}


	vector<Pitches> Spans::GenerateVariations () const
	{
		vector<Pitches> result;
		const Pitches rearranged{ ExtractPitches() };
		result.push_back(rearranged);
		result.push_back(rearranged.GenerateInversion());			// Mirror
		result.push_back(rearranged.GenerateRetrograde());			// Crab
		result.push_back(rearranged.GenerateRetrogradeInversion());	// Mirror's Crab

		return result;
	}
}