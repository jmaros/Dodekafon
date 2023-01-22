/*
DodekafonRecord.h
*/

#pragma once


#ifndef _DODEKAFONRECORD_H_
#define _DODEKAFONRECORD_H_

#include <array>

#define DODEKAFON_SIZE 12

#define _DEBUG_OUTPUTSTR_
//#define _DEBUG_OUTPUTSTR_1_
#define _DEBUG_OUTPUTSTR_0_

typedef std::pair<std::size_t, bool> DiffIteratorType;

class DodekafonRecord {
private:
	std::array<int, DODEKAFON_SIZE> record{ 0 };
	std::array<int, DODEKAFON_SIZE> usedValue{ 0 };
	std::array<int, DODEKAFON_SIZE> usedDistance{ 0 };
	std::size_t length{};
	bool noMore{ false };
public:
	enum OutputDebugStringModeEnum {
		RecordMode = 1,
		UsedValueMode = 2,
		UsedDistanceMode = 4,
		LengthMode = 8,
		NoMoreMode = 0x10,
		AllMode = 0x1f
	};
public:
	DodekafonRecord() {}
	DodekafonRecord(std::size_t n);

	bool operator == (const DodekafonRecord& r2) const;

	bool IsThereNext() const;
	std::size_t ValueNext(DiffIteratorType& diffIterator) const;
	DodekafonRecord FindNext(DiffIteratorType& diffIterator) const;
	bool IsNewValid(const std::vector<DodekafonRecord>& records);
	void OutputDebugString(const char* strIn, DWORD mode = AllMode) const;
};


#endif // _DODEKAFONRECORD_H_
