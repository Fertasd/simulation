#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* lv6s1x.c                                          2003.04.18.  */
/* 9 species each having two predators and two preys              */
/* allowing site exchange between neutral pairs                   */
/* MC simulations                                                 */
/* pair correlations and fluctuations                             */

class Sim_tox2x : public Simulation
{
public:
	Sim_tox2x(size_t width = 500);						/*UC: declares a simulation with given width */
	constexpr static auto name() { return "tox2x"; }
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	void step() override;
	void reset() override;
	size_t stepTargetNumber() const override;			/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
private:
	RandomGenerator rng;						/* initializes a random number generator */
	static constexpr uint8_t Ns = 9;           /* number of species/strategies               */
	static constexpr double K = 0.5;           /* UC: temptation to choose defection, not used for this simulation */
	static constexpr double Px = 0.066;			/* UC: exchange rate between neutral pairs */
	static constexpr double alfa = 1.0;			/* UC: reduction in invasion rates if < 1 */
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	const double pm[Ns][Ns];							/* declaration of the payoff matrix */
};
