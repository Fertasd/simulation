#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* RPS game on square lattice with stochastic CA rule      */
/* z=4, start from random initial state                    */
/* MC simulations with logit rule                          */

class Sim_RPS_CA : public Simulation
{
public:
	Sim_RPS_CA(size_t width = 500);				/*UC: declares a simulation with given width */
	constexpr static auto name() { return "RPS_CA"; }
	std::vector<SimParameter> parameters() override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	void step() override;
	void reset() override;
	size_t stepTargetNumber() const override;	/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
	uint8_t bisectStep(SimParameter, double) override;
	std::vector<QPair<double, double>> bisectionAnalysis(SimParameter, SimParameter) override;
	double bisect(SimParameter) override;
private:
	RandomGenerator rng;						/* initializes a random number generator */
	static constexpr uint8_t Ns = 3;           /* number of species               */
	const SimParameter K{"K", 0.5, "temperature"};
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	const double pm[Ns][Ns];							/* declaration of the payoff matrix */
};
