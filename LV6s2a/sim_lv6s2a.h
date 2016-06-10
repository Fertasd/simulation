#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* lv6s1x.c                                          2003.04.18.  */
/* 6 species each having two predators and two preys              */
/* allowing site exchange between neutral pairs                   */
/* MC simulations                                                 */
/* pair correlations and fluctuations                             */

class Sim_LV6s2a : public Simulation
{
public:
	Sim_LV6s2a(size_t width = 500);					/*UC: declares a simulation with given width */
	QString name() const override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	void step() override;
	void reset() override;
	size_t stepTargetNumber() const override;		/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
private:
	RandomGenerator rng;						/* initializes a random number generator */
	static constexpr uint8_t Ns = 6;           /* number of species               */
	static constexpr double K = 0.5;           /* temptation to choose defection, not used in this game  */
	static constexpr double Px = 0.1;			/* exchange rate between neutral pairs */
	static constexpr size_t Su = 2;				/* sampling unit, currently unused */
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	const double pm[Ns][Ns];					/* declaration of the payoff matrix */
};
