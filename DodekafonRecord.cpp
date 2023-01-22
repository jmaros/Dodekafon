/*
DodekafonRecord
*/
#include <array>
#include <vector>
#include <string>


#include <windows.h>
#include <Debugapi.h>

#include "DodekafonRecord.h"


DodekafonRecord::DodekafonRecord(std::size_t n)
{
	*this = DodekafonRecord();
	if (n > 0 && n <= DODEKAFON_SIZE) {
		record[0] = n;
		usedValue[n - 1] = 1;
		length = 1;
	}
}

void DodekafonRecord::OutputDebugString(const char* strIn, DWORD mode) const
{
#ifdef _DEBUG_OUTPUTSTR_

	std::string s;
	s = strIn;
	if (mode & LengthMode) {
		s += " size:";
		s += std::to_string(length);
	}
	if (mode & NoMoreMode) {
		s += " noMore:";
		if (noMore)
			s += "TRUE";
		else
			s += "FALSE";
	}
	s += "\n";
	if (mode & RecordMode) {
		for (int i = 0; i < length; ++i) {
			if (i > 0)
				s += ", ";
			s += std::to_string(record[i]);
		}
		s += "\n";
	}
	if (mode & UsedValueMode) {
		for (int i = 0; i < DODEKAFON_SIZE; ++i) {
			if (usedValue[i] > 0)
				s += std::to_string(i + 1);
			s += ", ";
		}
		s += "\n";
	}
	if (mode & UsedDistanceMode) {
		for (int i = 0; i < DODEKAFON_SIZE - 1; ++i) {
			if (usedDistance[i] > 0)
				s += std::to_string(i + 1);
			s += ", ";
		}
		s += "\n";
	}
	::OutputDebugStringA(s.c_str());
#endif

}


std::size_t DodekafonRecord::ValueNext(DiffIteratorType& diffIterator) const
{
	std::size_t ret = 0;
	std::size_t diffPrev = diffIterator.first;
	std::size_t i0 = DODEKAFON_SIZE - 1;
	if (diffPrev > 0)
		i0 = diffPrev - 1;
	for (std::size_t i = i0; i > 0; --i) {
		if (usedDistance[i - 1] == 1)
			continue;

		std::size_t diff = i;

		if (this->record[length - 1] > diff &&
			usedValue[this->record[length - 1] - diff - 1] == 0)
			ret = this->record[length - 1] - diff;
		else if (this->record[length - 1] + diff <= DODEKAFON_SIZE &&
			usedValue[this->record[length - 1] + diff - 1] == 0)
			ret = this->record[length - 1] + diff;
		else
			continue;
		return ret;

	}
	return ret;
}

bool DodekafonRecord::IsThereNext() const
{
	return !noMore;
}

DodekafonRecord DodekafonRecord::FindNext(DiffIteratorType& diffIterator) const
{
	DodekafonRecord r(*this);
	std::size_t vNext = r.ValueNext(diffIterator);
	if (vNext > 0) {
		if (length > 0) {
			std::size_t vDistance = labs(vNext - this->record[length - 1]);
			r.usedDistance[vDistance - 1] = 1;
		}
		r.record[length] = vNext;
		r.usedValue[vNext - 1] = 1;
		++r.length;
		diffIterator = DiffIteratorType{ 0,false };
	}
	else { // step back
		while (r.length > 1) {
#ifdef _DEBUG_OUTPUTSTR_1_	
			if (r.length == 8 &&
				r.record[0] == 3 &&
				r.record[1] == 10 &&
				r.record[2] == 2 &&
				r.record[3] == 11 &&
				r.record[4] == 1 &&
				r.record[5] == 12 &&
				r.record[6] == 6 &&
				r.record[7] == 5
				) {
				r.OutputDebugString("Spec state -", AllMode);
			}
#endif

#ifdef _DEBUG_OUTPUTSTR_1_	
			r.OutputDebugString("FindNext step back from -", AllMode);
#endif
			std::size_t nn = r.record[r.length - 1];
			r.record[r.length - 1] = 0;
			r.usedValue[nn - 1] = 0;
			std::size_t vDistance = labs(nn - r.record[r.length - 2]);
			std::size_t nn2 = nn + vDistance * 2;
			if (nn < r.record[r.length - 2] &&
				nn2 <= DODEKAFON_SIZE &&
				r.usedValue[nn2 - 1] == 0) {
				r.usedValue[nn2 - 1] = 1;
				r.record[r.length - 1] = nn2;
				diffIterator = DiffIteratorType{ vDistance , false };
				diffIterator = DiffIteratorType{ 0 , false };
				break;
			}

			r.usedDistance[vDistance - 1] = 0;
			--r.length;

#ifdef _DEBUG_OUTPUTSTR_1_
			if (r.length <= 3) {
				std::string s;

				s = "FindNext step back size:";
				s += std::to_string(r.length);
				s += "\n";
				::OutputDebugStringA(s.c_str());
			}
#endif

			diffIterator = DiffIteratorType{ vDistance , false };
			std::size_t lengthPrev = r.length;

#ifdef _DEBUG_OUTPUTSTR_1_	
			r.OutputDebugString("FindNext step back to - size:");
#endif
			break;
			/*
			r = r.FindNext(diffIterator1);
			r.OutputDebugString("FindNext step back next - size:");

			if (r.length > lengthPrev)
				break;
			*/
		}
		if (length == 1) {
			r.noMore = true;
		}
	}
	return r;
}

bool DodekafonRecord::operator == (const DodekafonRecord& r2) const
{
	for (std::size_t i = 0; i < DODEKAFON_SIZE; ++i) {
		if (this->record[i] != r2.record[i])
			return false;
	}
	return true;
}

bool DodekafonRecord::IsNewValid(const std::vector<DodekafonRecord>& records)
{
	if (length == DODEKAFON_SIZE) {
		/*
		for (const DodekafonRecord& ri : records) {
			if (ri == *this)
				return false; // never run here, because the enumeration order
		}
		*/
		return true;
	}
	return false;
}

