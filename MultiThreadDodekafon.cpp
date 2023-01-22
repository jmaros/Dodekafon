/*
MultiThreadDodekafon.cpp
*/


#include <vector>
#include <string>

#include <windows.h>
#include <process.h>
#include <Debugapi.h>

#include "DodekafonRecord.h"

#define MAX_THREADS  DODEKAFON_SIZE

static HANDLE  hThreads[MAX_THREADS] = { NULL };
static HANDLE  hTimerMutex;

struct ThreadData {
	std::vector<DodekafonRecord> * resultPtr{nullptr};
	std::size_t startValue;
	DWORD timeTick{0};

	ThreadData() {};
	ThreadData(std::vector<DodekafonRecord>* resultPtrIn, std::size_t startValueIn, DWORD timeTickIn) :
		resultPtr(resultPtrIn),
		startValue (startValueIn),
		timeTick (timeTickIn)
	{}
};

void DodekafonProcess(void * dataPtr)
{
	ThreadData* threadDataPtr = (ThreadData*)dataPtr;
	if (threadDataPtr != nullptr && threadDataPtr->resultPtr != nullptr) {
		DodekafonRecord start(threadDataPtr->startValue);
		DiffIteratorType diffIterator{ 0,false };
		for (DodekafonRecord r = start; r.IsThereNext(); r = r.FindNext(diffIterator)) {
			if (r.IsNewValid(*threadDataPtr->resultPtr)) {
				threadDataPtr->resultPtr->push_back(r);
			}
		}
		WaitForSingleObject(hTimerMutex, INFINITE);
		DWORD tt1 = GetTickCount();
		ReleaseMutex(hTimerMutex);
		threadDataPtr->timeTick = tt1 - threadDataPtr->timeTick;
	}
}


int SimpleDodekafonV1_MultiThread(bool listMode, std::array<DWORD, DODEKAFON_SIZE>& timetickArray)
{
	std::vector<DodekafonRecord> resultThreads[DODEKAFON_SIZE];
	std::vector<DodekafonRecord> result;
	ThreadData threadData[DODEKAFON_SIZE];

	DWORD tt0 = GetTickCount();
	for (std::size_t i = 0; i < DODEKAFON_SIZE; ++i) {
		threadData[i] = ThreadData(&resultThreads[i], i + 1, tt0);
		hThreads[i] = (HANDLE)_beginthread(DodekafonProcess, 0, (void*)(ThreadData *)&threadData[i]);
	}
	for (std::size_t i = 0; i < DODEKAFON_SIZE; ++i) {
		WaitForSingleObject(hThreads[i], INFINITE);
		for (const DodekafonRecord& r : resultThreads[i]) {
			result.push_back(r);
		}
		timetickArray[i] += threadData[i].timeTick;
	}
	DWORD tt1 = GetTickCount();
	if (listMode) {
		for (const DodekafonRecord& r : result) {
<<<<<<< HEAD
			r.OutputDebugString("Valid record -", DodekafonRecord::RecordMode);
=======
			r.OutputDebugString("Valid record - ", DodekafonRecord::RecordMode);
>>>>>>> 0adca89e30f7f1ff4d96df45e7f6bddb4f7667ff
		}
#ifdef _DEBUG_OUTPUTSTR_
		std::string s;
		s = "Found records:";
		s += std::to_string(result.size ());
		s += "\n Ellapsed mSec:";
		s += std::to_string(tt1 - tt0);
		s += "\n";
		::OutputDebugStringA(s.c_str());
		s = "TimeTick:";
		for (DWORD tt : timetickArray) {
			s += std::to_string(tt);
			s += ", ";
		}
		s += "\n";
		::OutputDebugStringA(s.c_str());
#endif
	}
	return result.size();
}


int MultiThreadDodekafonV1()
{
	std::array<DWORD, DODEKAFON_SIZE> timetickArray{ 0 };
	DWORD tc = GetTickCount();
	int ret = 0;
	const int num = 100;
	for (int i = 0; i < num; ++i) {
		ret = SimpleDodekafonV1_MultiThread(i == 0, timetickArray);
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
	s = "TimeTick:";
	for (DWORD tt : timetickArray) {
		s += std::to_string(tt / num);
		s += ", ";
	}
	s += "\n";
	::OutputDebugStringA(s.c_str());
#endif
	return ret;
}


