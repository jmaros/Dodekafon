//
// Testdata.h
//
#pragma once

#include "DodekaBase.h"

#if defined (DEBUG_SOLVE_DODEKAFON)

#include <array>
#include <tuple>
#include <vector>

namespace Dodekafon {
	using std::array;
	using std::vector;

	using Array11 = const std::array<std::tuple<size_t, size_t, int>, 11>;
	using Array12 = const std::array<size_t, 12>;
	//
	// Testdata is generated by Multithreaded Dodekafon
	// invented by Gyula Kiss
	//
	extern const array<Array11, 1328> Testdata11;
	extern const array<Array12, 1328> Testdata12;
}

#endif // DEBUG_SOLVE_DODEKAFON
