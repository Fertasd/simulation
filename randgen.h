#pragma once
#include <cmath>
#include <array>
#include <limits>
#include <mutex>
/******* RANDOM NUMBER GENERATOR BY ZIFF **********/
/*implemented as a class*/

class RandomGenerator
{
public:
	RandomGenerator(int32_t seed);

	int32_t next();
	int32_t nextSync();
	double nextNormal();
	double nextNormalSync();
	template<size_t N>
	std::array<int32_t, N> nextSync()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		std::array<int32_t, N> array;
		for (size_t i = 0; i < N; ++i)
			array[i] = next();
		return array;
	}

	static constexpr int32_t RIMAX = std::numeric_limits<int32_t>::max();/*the maximum value the generator can return*/
private:
	static constexpr int32_t A = 471, B = 1586, C = 6988, D = 9689, M = 16383; /* additional constants and variables
																				 for the algorithm */
	int32_t nd;
	int32_t ra[M+1];
	std::mutex _mutex;
};
