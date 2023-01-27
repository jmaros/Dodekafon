//
// DebugPrint.cpp
//
#include "DebugPrint.h"

int  NextDebugLineNumber ()
{
	static int lineNumber = 0;
	return ++lineNumber;
}

void DebugPrint ()
{
}
