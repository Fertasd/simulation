#include "randgen.h"

RandomGenerator::RandomGenerator(int32_t seed) : nd(0) /*initializes the generator from a given seed*/
{
	ra[0] = (int32_t)fmod(16807.0*(double)seed, 2147483647.0);
	for (int32_t i = 1; i <= M; i++)
	{
		ra[i] = (int32_t)fmod(16807.0 * (double)ra[i - 1], 2147483647.0);
	}
}

int32_t RandomGenerator::next()
{
	return ++nd, ra[nd & M] = ra[(nd-A) & M] ^ ra[(nd-B) & M] ^ ra[(nd-C) & M] ^ ra[(nd-D) & M]; /*retrieves a random
																								   number from the
																								   generator*/
}

int32_t RandomGenerator::nextSync()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return next();
}

double RandomGenerator::nextNormal()
{
	return static_cast<double>(next()) / RIMAX;
}

double RandomGenerator::nextNormalSync()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return nextNormal();
}
