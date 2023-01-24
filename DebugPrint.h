//
// DebugPrint.h
//
#pragma once
#include <iomanip>
#include <iostream>

void DebugPrint();

template<typename T>
void DebugPrint(T const& t)
{
	std::cout << t;
	DebugPrint();
}

template<typename T, typename...Rest>
void DebugPrint(T const& t,
				Rest const&...	rest)
{
	std::cout << t;
	DebugPrint(rest...);
}

template<typename...Params>
void DebugLine (Params const&...	rest)
{
	static int n = 0;
	DebugPrint("Debug line ", std::setw(3), ++n, rest...);
}

template<typename...Params>
void DebugNewLine (Params const&...	rest)
{
	DebugLine(rest...);
	DebugPrint("\n");
}
