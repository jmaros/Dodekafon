/*
SimpleDodekafonV1.cpp
*/

#include <vector>
#include <string>

#include <windows.h>
#include <Debugapi.h>

#include "DodekafonRecord.h"

int SimpleDodekafonV1_SingleThread(bool listMode)
{
	std::vector<DodekafonRecord> result;

	for (std::size_t i = 1; i <= DODEKAFON_SIZE; ++i) {
		DodekafonRecord start(i);
		DiffIteratorType diffIterator{ 0,false };
		for (DodekafonRecord r = start; r.IsThereNext(); r = r.FindNext(diffIterator)) {
			if (r.IsNewValid(result)) {
				if (listMode)
					r.OutputDebugString("Valid record -", DodekafonRecord::RecordMode);
				result.push_back(r);
			}
		}

	}
	return result.size();
}


int SimpleDodekafonV1()
{
	DWORD tc = GetTickCount();
	int ret = 0;
	const int num = 100;
	for (int i = 0; i < num; ++i) {
		ret = SimpleDodekafonV1_SingleThread(i==0);
	}
	DWORD tc2 = GetTickCount();
#ifdef _DEBUG_OUTPUTSTR_

	std::string s;
	s = "Found records:";
	s += std::to_string(ret);
	s += "\n Ellapsed mSec:";
	s += std::to_string((tc2 - tc) / num);

	s += "\n";
	::OutputDebugStringA(s.c_str());
#endif
	return ret;
}