/*
MultiThreadDodekafon.cpp
*/


#include <vector>
#include <string>

#include <windows.h>
#include <process.h>
#include <Debugapi.h>
#include <conio.h>

#include "DodekafonRecord.h"

#include "BasicSoundPlayer.h"

#define MAX_THREADS  DODEKAFON_SIZE

static HANDLE  hThreads[MAX_THREADS] = { NULL };
static HANDLE  hTimerMutex;

enum ReportModeEnum {
	NoReportMode = 0,
	ListMode = 1,
	PlayMode = 2
};

struct ThreadData {
	std::vector<DodekafonRecord> * resultPtr{nullptr};
	std::size_t startValue;
	uint64_t timeTick{0Ui64};

	ThreadData() {};
	ThreadData(std::vector<DodekafonRecord>* resultPtrIn, std::size_t startValueIn, uint64_t timeTickIn) :
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
		uint64_t tt1 = GetTickCount64();
		ReleaseMutex(hTimerMutex);
		threadDataPtr->timeTick = tt1 - threadDataPtr->timeTick;
	}
}


size_t SimpleDodekafonV1_MultiThread(ReportModeEnum reportMode, std::array<uint64_t, DODEKAFON_SIZE>& timetickArray)
{
	std::vector<DodekafonRecord> resultThreads[DODEKAFON_SIZE];
	std::vector<DodekafonRecord> result;
	ThreadData threadData[DODEKAFON_SIZE];

	uint64_t tt0 = GetTickCount64();
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
	uint64_t tt1 = GetTickCount64();
	if (reportMode & ReportModeEnum::ListMode) {
		for (const DodekafonRecord& r : result) {
			r.OutputDebugString("Valid record -", DodekafonRecord::ODE::RecordMode);
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
		for (auto tt : timetickArray) {
			s += std::to_string(tt);
			s += ", ";
		}
		s += "\n";
		::OutputDebugStringA(s.c_str());
#endif
	}
	if (reportMode & ReportModeEnum::PlayMode) {
		BasicSoundPlayer player;
		player.PlayBasicSounds();
		Sleep(1000);
		int iRythm = 2;
		for (const DodekafonRecord& r : result) {
			player.PlayBasicSounds(r, iRythm);
			++iRythm;
			if (iRythm >= 5)
				iRythm = 2;
			Sleep(1000);
			if (_kbhit()) {
				break;
			}
		}
	}

	return result.size();
}


size_t MultiThreadDodekafonV1()
{
	std::array<uint64_t, DODEKAFON_SIZE> timetickArray{ 0 };
	uint64_t tc = GetTickCount64();
	size_t ret = 0;
	const int num = 100;
	for (int i = 0; i < num; ++i) {
		ReportModeEnum report(i == 0 ? ListMode : NoReportMode);
		ret = SimpleDodekafonV1_MultiThread(report, timetickArray);
	}
	uint64_t tc2 = GetTickCount64();
#ifdef _DEBUG_OUTPUTSTR_

	std::string s;
	s = "Found records:";
	s += std::to_string(ret);
	s += "\n Ellapsed mSec:";
	s += std::to_string((tc2 - tc) / num);

	s += "\n";
	::OutputDebugStringA(s.c_str());
	s = "TimeTick:";
	for (auto tt : timetickArray) {
		s += std::to_string(tt / num);
		s += ", ";
	}
	s += "\n";
	::OutputDebugStringA(s.c_str());
#endif
	SimpleDodekafonV1_MultiThread(PlayMode, timetickArray);
	return ret;
}


