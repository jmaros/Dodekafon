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
	Spans::Spans (size_t n)
		: m_nodes(n + 1, Node{ 0, PitchStatus::Empty })
		, m_intervals{ n, Interval{} }
	{
	}

	Spans::Spans (const Spans& parent)
		: m_nodes{ parent.m_nodes }
		, m_intervals{ parent.m_intervals }
	{
	}

	//
	// Spans accessors
	//
	size_t	Spans::NodesSize () const
	{
		return m_nodes.size();
	}

	size_t	Spans::IntervalsSize () const
	{
		return m_intervals.size();
	}

	size_t	Spans::SpanSize () const
	{
		return IntervalsSize();
	}

	const Node& Spans::GetNode (size_t i) const
	{
		static const Node invalid{ 0, PitchStatus::Invalid };
		const Node* result{ &invalid };
		if (i > 0 && i < m_nodes.size()) {
			result = &m_nodes[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	const Interval& Spans::GetInterval (size_t i) const
	{
		static const Interval empty;
		const Interval* result{ &empty };
		if (i > 0 && i < m_intervals.size()) {
			result = &m_intervals[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	Node	Spans::FindFirstNode () const
	{
		Node	result{ 0, PitchStatus::Invalid };
		for (size_t i = 1; i < NodesSize(); ++i) {
			if (GetNode(i).GetPitchStatus() == PitchStatus::Empty) {
				break;
			}
			if (GetNode(i).GetPitchStatus() == PitchStatus::ValidLeafpoint) {
				result = GetNode(i);
				break;
			}
		}
		return result;
	}

	Node	Spans::FindNextNode (const Node& prevNode,
								 const Interval& prevInter) const
	{
		Node	resultingNode{ 0, PitchStatus::Invalid };
		size_t	prevPitchIndex{ prevNode.GetPitch() };
		size_t	pitch0000Index{ prevInter.GetPitch(0) };
		size_t	pitch0001Index{ prevInter.GetPitch(1) };
		size_t	nextPitchIndex{};

		if (prevPitchIndex == pitch0000Index) {
			nextPitchIndex = pitch0001Index;
		} else if (prevPitchIndex == pitch0001Index) {
			nextPitchIndex = pitch0000Index;
		}

		if (nextPitchIndex) {
			resultingNode = GetNode(nextPitchIndex);
		}
		return resultingNode;
	}

	bool Spans::IsValidSpan () const
	{
		size_t	prevInterval{};
		Node	node = FindFirstNode();
		bool	success = node.IsLeafPoint();
		if (success) {
			set<size_t> nodeIndices;
			set<size_t> intervalIndices;

		//	size_t		nextPithchi{};

			for (size_t i = 1; i < SpanSize(); ++i) {
				auto pitchi = node.GetPitch();
				if (nodeIndices.insert(pitchi).second == false) {
					return false;
				}
				auto nexti = node.GetNextInterval(prevInterval);
				if (nexti) {
					if (intervalIndices.insert(nexti).second == false) {
						return false;
					}
					Interval inti{ GetInterval(nexti) };
					auto nextPitchi = inti.GetNextPitch(pitchi);
					if (nextPitchi) {
						node = GetNode(nextPitchi);
					} else {
						return false;
					}
					prevInterval = nexti;
				}
			}
			if (success) {
				auto pitchi = node.GetPitch();
				if (nodeIndices.insert(pitchi).second == false) {
					return false;
				}
				success = nodeIndices.size() == SpanWidth;
				if (success) {
					success = intervalIndices.size() == MaxIntervalLength;
				}
			}
		}
		return success;
	}

	//
	// Spans modifiers
	//
	Node& Spans::NodeRef (size_t  i)
	{
		static const Node invalid{ 0, PitchStatus::Empty };
		static Node myInvalid{ 0, PitchStatus::Empty };
		if (myInvalid.GetPitchStatus() != invalid.GetPitchStatus()) {
			myInvalid = invalid;
			cout << setw(80) << "This should happen only once!\n";
		}
		Node* result{ &myInvalid };
		if (i < NodesSize()) {
			result = &m_nodes[i];
		} else {
			cout << "\nInternal Error!\n";
			// this should rather throw
		}
		return *result;
	}

	Interval& Spans::IntervalRef (size_t  intervalWidth)
	{
		static const Interval empty;
		static Interval myEmpty{};
		if (!myEmpty.IsEmpty()) {
			myEmpty = empty;
			cout << setw(80) << "This should happen only once!\n";
		}
		Interval* result{ &myEmpty };
		if (intervalWidth < IntervalsSize()) {
			result = &m_intervals[intervalWidth];
		} else {
			cout << "\nInternal Error at Spans::GetIntervalRef!\n";
			// this should rather throw
		}
		return *result;
	}

	bool	Spans::AddInterval (size_t		intervalLength,
								size_t		lowerPich)
	{
		bool success{ false };
		if (intervalLength > 0 && intervalLength < IntervalsSize()) {
			// valid interval length

			size_t higherPitch{ lowerPich + intervalLength };
			if (higherPitch < NodesSize()) {
				// valid higherPitch

				Node		nlr{ GetNode(lowerPich) };
				Node		nhr{ GetNode(higherPitch) };
				Interval	ir{ GetInterval(intervalLength) };

				if (ir.IsEmpty() &&
					nlr.IsAvailableForIntervalAddition() &&
					nhr.IsAvailableForIntervalAddition()) {
					// This interval can be added
					ir.Init(intervalLength, lowerPich);
					bool	successA{};
					if (nlr.IsEmpty()) {
						successA = nlr.InitLeafNode(intervalLength, lowerPich);
					} else if (nlr.IsLeafPoint()) {
						successA = nlr.InitMiddleNode(intervalLength, lowerPich);
					}
					bool	successB{};
					if (nhr.IsEmpty()) {
						successB = nhr.InitLeafNode(intervalLength, higherPitch);
					} else if (nlr.IsLeafPoint()) {
						successB = nhr.InitMiddleNode(intervalLength, higherPitch);
					}
					success = successA && successB;
				}
				if (success) {
					if (nlr.GetPitch() == lowerPich && nhr.GetPitch() == higherPitch) {
						NodeRef(lowerPich) = nlr;
						NodeRef(higherPitch) = nhr;
						IntervalRef(intervalLength) = ir;
					} else {
						throw std::logic_error("Internal Error at Spans::AddInterval!");
					}
				}
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
		Pitches		result(SpanSize ());
		Node		node = FindFirstNode();
		if (node.IsLeafPoint()) {
			size_t intervalIndex = node.GetInterval(0);
			Interval	prevInterval = GetInterval(intervalIndex);
			for (size_t i = 0;
				 i < result.Size() &&
				 node.GetPitchStatus() != PitchStatus::Invalid;
				 ++i) {
				result.SetPitch(i, node.GetPitch());
				if (i > 0 && node.IsLeafPoint()) {
					break;
				}
				Node		prevNode{ node };
				node = FindNextNode(prevNode,
									prevInterval);
				intervalIndex = node.GetNextInterval(prevInterval.GetWidth());
				if (intervalIndex) {
					prevInterval = GetInterval(intervalIndex);
				} else {
					prevInterval = Interval();
				}
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