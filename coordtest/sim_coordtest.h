#pragma once
#include "simulation.h"
#include "randgen.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

/*                                             2015.07.19.  */
/* Prisoner's Dilemma game with D, C, and Mixed strategies  */
/* z=4, stochastic imitation or best response               */
/* MC simulations                                           */

class Sim_coordtest: public Simulation
{
public:
	Sim_coordtest(size_t width = 300);						/*UC: declares a simulation with given width */
	constexpr static auto name() { return "coordtest"; }
	std::vector<SimParameter> parameters() override;
	uint32_t animationDelay() const override;
	uint32_t renderFrameSkip() const override;
	size_t displayWidth() const override;
	void step() override;
	void reset() override;
	size_t stepTargetNumber() const override;			// declarations of overrides to these functions,
	void createDomains();													  //allowing them to return values specific to
	uint8_t bisectStep(SimParameter, double) override;
	std::vector<QPair<double, double>> bisectionAnalysis(SimParameter, SimParameter) override;
	double bisect(SimParameter) override;
private:
	RandomGenerator rng;						/* initializes a random number generator */
	static constexpr uint8_t Ns = 2;           /* number of species/strategies               */
	SimParameter cop{"CoP", 1.0, "coordination payoff"};
	SimParameter crp{"CrP", 0.2, "cross-dependent payoff"};
	SimParameter sep{"SeP", 0.005, "self-dependent payoff"};
	SimParameter startselect{"StartSelect", 1, "Initial configuration. 1: interface 2: random"};
	SimParameter range1Min{"range1Min", -1, "minimum of horizontal range"};
	SimParameter range1Max{"range1Max", 1, "maximum of horizontal range"};
	SimParameter biStep{"biStep", 0.1, "step on horizontal range"};
	SimParameter range2Min{"range2Min", -1, "minimum of vertical range"};
	SimParameter range2Max{"range2Max", 1, "maximum of vertical range"};
	const SimParameter zz{"zz", 4, "number of neighbors"};
	const SimParameter Temp{"Temp", 0.3, "temperature"};
	//const SimParameter Su{"Su", 2, "Sampling unit for the Monte Carlo step"};
	std::vector<size_t> csok, nov;				/* declaration of vectors used to resolve indexing issues */
	std::vector<uint8_t> data2;					/* declaration of a temporary structure that keeps track changes in a step */
	double pm[Ns][Ns];							/* declaration of the payoff matrix */
	//size_t res[Ns];
};
