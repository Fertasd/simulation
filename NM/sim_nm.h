#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/* NMCA.c                                     2007.11.02.  */
/* Nowak-May cellular automaton of PD game                 */
/* z=9, start from random initial state                    */
/* MC simulations                                          */

class Sim_NM : public Simulation
{
public:
	Sim_NM(size_t width = 500);						/*UC: declares a simulation with given width */
	constexpr static auto name() { return "NM"; }
	std::vector<SimParameter> parameters() override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	void step() override;
	void reset() override;
	size_t stepTargetNumber() const override;		/* declarations of overrides to these functions,
														  allowing them to return values specific to
														  this kind of simulation */
	uint8_t bisectStep(SimParameter, double) override;
	std::vector<QPair<double, double>> bisectionAnalysis(SimParameter, SimParameter) override;
	double bisect(SimParameter) override;
private:
	RandomGenerator rng;							/* initializes a random number generator */
	static constexpr uint8_t Ns = 2;               /* number of species/strategies               */
	const SimParameter tempt{"tempt", 1.1, "Temptation to choose defection"};
	std::vector<size_t> csok, nov;					/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	std::vector<std::atomic<uint32_t>> pos;					/* declaration of another temporary structure, will hopefully be dealt with
												  soon as it's from the old code and is unnecessary */
	const double pm[Ns][Ns];							/* declaration of the payoff matrix */
};
