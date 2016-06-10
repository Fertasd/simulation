#pragma once

#include <atomic>
#include <limits>

/******* RANDOM NUMBER GENERATOR BY ZIFF **********/
/*implemented as a class*/

class RandomGenerator
{
public:
	RandomGenerator(int32_t seed);

	int32_t next();
	double nextNormal();

	static constexpr int32_t RIMAX = std::numeric_limits<int32_t>::max();/*the maximum value the generator can return*/
private:
	static constexpr int32_t A = 471, B = 1586, C = 6988, D = 9689, M = 16383; /* additional constants and variables
																				 for the algorithm */
	std::atomic<int32_t> nd;
	std::atomic<int32_t> ra[M+1];
};
