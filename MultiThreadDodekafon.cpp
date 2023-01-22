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

static HANDLE  hRunMutex[MAX_THREADS] = { NULL };
static HANDLE  hThreads[MAX_THREADS] = { NULL };
static HANDLE  hTimerMutex;

struct ThreadData {
	HANDLE* hThreadPtr{nullptr};
	std::vector<DodekafonRecord> * resultPtr{nullptr};
	std::size_t startValue;
	DWORD timeTick{0};

	ThreadData() {};
	ThreadData(HANDLE* hThreadPtrIn, std::vector<DodekafonRecord>* resultPtrIn, std::size_t startValueIn, DWORD timeTickIn) :
		hThreadPtr(hThreadPtrIn),
		resultPtr(resultPtrIn),
		startValue (startValueIn),
		timeTick (timeTickIn)
	{}
};

void DodekafonProcess(void * dataPtr)
{
	ThreadData* threadDataPtr = (ThreadData*)dataPtr;
	if (threadDataPtr != nullptr && threadDataPtr->hThreadPtr != nullptr && threadDataPtr->resultPtr != nullptr) {
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
		threadDataPtr->timeTick = threadDataPtr->timeTick;
	}
}


int SimpleDodekafonV1_MultiThread(bool listMode)
{
	std::vector<DodekafonRecord> resultThreads[MAX_THREADS];
	std::vector<DodekafonRecord> result;
	ThreadData threadData[MAX_THREADS];

	DWORD tt0 = GetTickCount();
	for (std::size_t i = 0; i < DODEKAFON_SIZE; ++i) {
		threadData[i] = ThreadData(&hRunMutex[i], &resultThreads[i], i + 1, tt0);
		hRunMutex[i] = (HANDLE)CreateMutexW(NULL, TRUE, NULL);
		hThreads[i] = (HANDLE)_beginthread(DodekafonProcess, 0, (void*)(ThreadData *)&threadData[i]);
	}
	return result.size();
}


int MultiThreadDodekafonV1()
{
	DWORD tc = GetTickCount();
	int ret = 0;
	const int num = 10;
	for (int i = 0; i < num; ++i) {
		ret = SimpleDodekafonV1_MultiThread(i == 0);
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


