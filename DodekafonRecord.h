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

using  DiffIteratorType = std::pair<std::size_t, bool>;

template <typename T>
T Distance (T v1, T v2)
{
	return v1 < v2 ? v2 - v1 : v1 - v2;
}

template <typename T>
/* Here should require concept for enum class type on T */
std::underlying_type_t<T> UnderlyingType (T value)
{
	using UTT = std::underlying_type_t<T>;
	return static_cast<UTT>(value);
}

template <typename T>
/* Here should require concept for enum class type on T */
std::underlying_type_t<T> ETest (T value, T testValue)
{
	return (UnderlyingType(value) & UnderlyingType(testValue));
}

class DodekafonRecord {
private:
	std::array<std::size_t, DODEKAFON_SIZE> record{ 0 };
	std::array<std::size_t, DODEKAFON_SIZE> usedValue{ 0 };
	std::array<std::size_t, DODEKAFON_SIZE> usedDistance{ 0 };
	std::size_t length{};
	bool noMore{ false };
public:
	enum class OutputDebugStringModeEnum : uint16_t {
		RecordMode = 1,
		UsedValueMode = 2,
		UsedDistanceMode = 4,
		LengthMode = 8,
		NoMoreMode = 0x10,
		AllMode = 0x1f
	};
	using ODE = OutputDebugStringModeEnum;
public:
	DodekafonRecord() {}
	DodekafonRecord(std::size_t n);

	bool operator == (const DodekafonRecord& r2) const;

	bool IsThereNext() const;
	std::size_t ValueNext(DiffIteratorType& diffIterator) const;
	DodekafonRecord FindNext(DiffIteratorType& diffIterator) const;
	bool IsNewValid(const std::vector<DodekafonRecord>& records);
	void OutputDebugString(const char* strIn, ODE mode = ODE::AllMode) const;
	const std::array<size_t, DODEKAFON_SIZE>& GetRecord() const;
};


#endif // _DODEKAFONRECORD_H_
