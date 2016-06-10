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
	std::vector<SimParameter> parameters() override;
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
	SimParameter K{"K", 0.5};            /* temptation to choose defection, this game uses tempt instead  */
	const SimParameter tempt{"tempt", 1.0005, "Temptation to choose defection"};
	static constexpr uint8_t zz = 4;			/* number of immediate neighbors */
	const SimParameter Temp{"Temp", 0.1, "Temperature (noise, possibility of trembles)"};
	const SimParameter q{"q", 0.002, "q... a parameter in the payoff matrix"};
	const SimParameter xx{"xx", 0.0, "Probability of interacting with far neighbors"};
	const SimParameter yy{"yy", 0.0, "Probability of spontaneously choosing a random strategy"};
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	const double pm[Ns][Ns];							/* declaration of the payoff matrix */
};
