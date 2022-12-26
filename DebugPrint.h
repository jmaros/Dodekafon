// DebugPrint.h
#pragma once
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
