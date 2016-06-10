#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* pd24nmt.c                                 2007.11.12.  */
/* evolutionary PD game with bad and good moving teachers */
/* z=24, start from random initial state                  */
/* MC simulations                                         */

class Sim_pd3s : public Simulation
{
public:
	Sim_pd3s(size_t width = 500);					/*UC: declares a simulation with given width */
	constexpr static auto name() { return "PD3S"; }
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
	static constexpr uint8_t Ns = 3;           /* number of species               */
	static constexpr double K = 0.5;            /* temptation to choose defection, this game uses tempt instead  */
	static constexpr double tempt = 1.0005;		/* UC: temptation to choose defection */
	static constexpr uint8_t zz = 4;			/* number of immediate neighbors */
	static constexpr double Temp = 0.1;			/* UC: Temperature(noise, possibility of trembles) */
	static constexpr double q = 0.002;			/* UC: q... a parameter in the payoff matrix(???) */
	static constexpr double xx = 0.0;			/* probability of far neighbors(???) */
	static constexpr double yy = 0.0;			/* UC: probability of spontaneously choosing a random strategy */
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	const double pm[Ns][Ns];							/* declaration of the payoff matrix */
};
