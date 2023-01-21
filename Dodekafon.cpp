// Dodekafon.cpp

// g++ -std=c++20 -O2 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp DebugPrint.cpp -o Dodekafon.exe ; .\Dodekafon.exe
// g++ -std=c++20 -g3 -Wc++20-compat -Wall -Wpedantic -time Dodekafon.cpp DebugPrint.cpp -o Dodekafon.exe ; gdb .\Dodekafon.exe

#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <string>

#include "Dodekafon.h"
#include "DebugPrint.h"

using std::cout;
using std::endl;
using std::setw;
using std::string;

template<typename...Rest>
void DebugLine (Rest const&...	rest)
{
	static int n = 0;
	DebugPrint("Debug line ", setw(3), ++n, rest...);
}


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

	//
	// Interval accessors
	//
	bool	Interval::IsEmpty () const
	{
		return	m_pitches.empty();
	}

	size_t	Interval::GetWidth () const
	{
		return	m_width;
	}

	// GetPitch (size_t	index) --> size_t
	// return the indexth connegting pitch
	// Valid "index" can be 0 or 1, since each
	// interval is supposed to be used only once
	size_t	Interval::GetPitch (size_t	index) const
	{
		size_t result{};

		if (index < 2 && index < m_pitches.size()) {
			result = m_pitches[index];
		}
		return	result;
	}

	size_t	Interval::GetNextPitch (size_t	prevPitch) const
	{
		size_t	nextPitch{GetPitch(0)};
		if (nextPitch == prevPitch) {
			nextPitch = GetPitch(1);
		}
		return nextPitch;
	}

	//
	// Interval modifiers
	//
	Interval& Interval::SetWidth (size_t newWidth)
	{
		m_width = newWidth;
		return *this;
	}

	Interval& Interval::AddPitch (size_t newPitch)
	{
		if (m_pitches.size() < 2) {
			m_pitches.push_back(newPitch);
		}
		return *this;
	}

	void	Interval::Init	(size_t		width,
							 size_t		lower)
	{
		SetWidth(width);
		AddPitch(lower);
		AddPitch(width + lower);
	}

	//
	// Spans constructors
	//
	Spans::Spans (size_t n)
		: m_nodes(n + 1, Node{0, PitchStatus::Empty})
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

	Node	Spans::FindNextNode (const Node&		prevNode,
								 const Interval&	prevInter) const
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

			size_t		nextPithchi{};

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

				Node& nlr{ NodeRef(lowerPich) };
				Node& nhr{ NodeRef(higherPitch) };
				Interval& ir{ IntervalRef(intervalLength) };

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
					if (nlr.GetPitch() != lowerPich || nhr.GetPitch() != higherPitch) {
						cout << "\nInternal Error at Spans::AddInterval!\n";
						// this should rather throw
					}
				}
			}

		}

		if (success) {
			//DebugLine(", Added Interval = ",
			//		  setw(2), intervalLength, ". Pos(",
			//		  setw(2), intervalLength, ") = ",
			//		  setw(2), GetNode(intervalLength).Pitch() + 1);
		} else {
			//DebugLine(", Rejected Interval = ",
			//		  setw(2), intervalLength, ". Pos(",
			//		  setw(2), intervalLength, ") = ",
			//		  setw(2), GetNode(intervalLength).Pitch() + 1);
		}

		return success;
	}

	bool CopyValidSpan(const Spans&		sRef,
					   Spans&			ls)
	{
		bool success{ sRef.IsValidSpan() };
		if (success) {
			ls = sRef;
		}
		return success;
	}

	void SolveDodekafon (size_t          intervalWidth,
						 const Spans&    sRef,
						 vector<Spans>&  result)
	{
		if (intervalWidth > 0) {

			for (size_t i = 1; i + intervalWidth < sRef.NodesSize(); ++i) {
				// Create a copy for each possible tree branch
				Spans ls(sRef);
				// Try to add the actual interval to the spans registry
				if (ls.AddInterval(intervalWidth, i)) {
					// Recurse on success with one shorter interval
					SolveDodekafon (intervalWidth - 1,		// recursion, Downward
									ls,
									result);
				}

			}
		} else {
			// If we managed to get here than we have a possible new resulting span
			Spans ls(sRef.SpanSize());
			if (CopyValidSpan(sRef, ls)) {
				result.push_back(ls);
			}
		}
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

	//
	// Pitches constructors
	//
	Pitches::Pitches ()
		: m_pitches	(SpanWidth, 0)
	{
	}

	Pitches::Pitches (size_t numPitches)
		: m_pitches	(numPitches, 0)
	{
	}

	Pitches::Pitches (const Pitches& pitchesPar)
		: m_pitches	(pitchesPar.m_pitches)
	{
	}

	// Pitches accessors
	size_t Pitches::Size () const
	{
		return m_pitches.size();
	}

	size_t Pitches::GetPitch (size_t i) const
	{
		return (i < Size() ? m_pitches[i] : 0);
	}

	// Pitches modifiers
	bool Pitches::SetPitch (size_t i,
							size_t pitchPar)
	{
		bool success{ i < Size() };
		if (success) {
			m_pitches[i] = pitchPar;
		}
		return success;
	}

	// Pitches operators
	bool Pitches::operator < (const Pitches& pitchesRight) const
	{
		return m_pitches < pitchesRight.m_pitches;
	}

	bool Pitches::operator == (const Pitches& pitchesRight) const
	{
		return m_pitches == pitchesRight.m_pitches;
	}

	// Pitches transformations

	//
	// Tukor (Mirror in Hungarian)
	//
	Pitches Pitches::GenerateInversion () const
	{
		Pitches result(Size());
		const size_t Complement = Size() + 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result.SetPitch(i, Complement - GetPitch(i));
		}
		return result;
	}

	//
	// Crab
	//
	Pitches Pitches::GenerateRetrograde () const
	{
		Pitches result(Size ());
		const size_t Maxi = Size() - 1;
		for (size_t i = 0;
			 i < result.Size();
			 ++i) {
			result.SetPitch(i, GetPitch(Maxi - i));
		}
		return result;
	}

	//
	// Mirror of Crab
	//
	Pitches Pitches::GenerateRetrogradeInversion () const
	{
		Pitches result(Size ());
		const size_t Complement = Size() + 1;
		const size_t Maxi		= Size() - 1;
		for (size_t i = 1;
			 i < result.Size();
			 ++i) {
			size_t retroPitch	= GetPitch(Maxi - i);
			size_t newPitch		= Complement - retroPitch;
			result.SetPitch(i, newPitch);
		}
		return result;
	}

	void Pitches::Dump () const
	{
		string mySeparator;
		for (size_t i = 1; i < Size(); ++i) {
			cout << mySeparator << setw(3) << GetPitch(i);
			mySeparator = ", ";
		}
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

	//
	// The goal is to print the 1..12 integer numbers that are representing the pitches of an octave
	// (here we are changing the internal 0..11 repres4ntation into the 1..12 representation
	//
	void PrintDodekafon (const vector<Spans>& result)
	{
		size_t d = 0;
		size_t n = 0;
		set<Pitches> sortedList;
		for (auto rsin : result) {
			const vector<Pitches> rsi4{ rsin.GenerateVariations() };
			++n;
			size_t v = 0;
			for (auto rsi : rsi4) {
				if (sortedList.insert(rsi).second) {
					cout << " ";
				} else {
					++d;
					cout << "-";
				}
				cout << setw(3) << n << "." << ++v << " series: ";
				rsi.Dump();
				cout << endl;
			}
		}
		cout << endl;
		cout << d << " duplicate eliminated out of " << 4 * n << ", remained: " << 4 * n - d << endl;
		cout << endl;
		size_t m = 0;
		for (auto slm : sortedList) {
			cout << setw(3) << ++m << ". sorted: ";
			slm.Dump();
			cout << endl;
		}
	}
}

using namespace Dodekafon;
/// @brief  This is the main entry point of a C or C++ program
/// @return The return value is an integer, automatically provided as 0, when the extlicit return is omitterd
int main ()
{
	Spans			     spans(SpanWidth);
	std::vector<Spans>    result;

	SolveDodekafon(MaxIntervalLength,
				   spans,
				   result);

	cout << "Dodekafon 1 --> " << SpanWidth << endl;

	PrintDodekafon (result);
}
