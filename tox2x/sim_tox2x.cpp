#include "sim_tox2x.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_tox2x::Sim_tox2x(size_t width) : Simulation(width), rng(2593), csok(width), nov(width), data2(width * width),
	pm{ {0, 4, 5, 1, 1, 3, 2, 3, 2}, /* UC: the payoff matrix is defined here */
		{5, 0, 4, 3, 1, 1, 2, 2, 3},
		{4, 5, 0, 1, 3, 1, 3, 2, 2},
		{2, 3, 2, 0, 4, 5, 1, 1, 3},
		{2, 2, 3, 5, 0, 4, 3, 1, 1},
		{3, 2, 2, 4, 5, 0, 1, 3, 1},
		{1, 1, 3, 2, 3, 2, 0, 4, 5},
		{3, 1, 1, 2, 2, 3, 5, 0, 4},
		{1, 3, 1, 3, 2, 2, 4, 5, 0}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table */
	palette().push_back(0x00FF0000);
	palette().push_back(0x0033CC00);
	palette().push_back(0x00FF6600);
	palette().push_back(0x00FFFF00);
	palette().push_back(0x000000FF);
	palette().push_back(0xFF000000);
	palette().push_back(0x0066FFFF);
	palette().push_back(0x009900CC);

	reset();		/* assigns a random strategy distribution to the simulation */
}

std::vector<SimParameter> Sim_tox2x::parameters()
{
	return {Px, alfa};
}

uint32_t Sim_tox2x::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_tox2x::renderFrameSkip() const
{
	return 1200;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_tox2x::displayWidth() const
{
	return 120;
}

void Sim_tox2x::step()		/* defines a simulation step */
{
	#pragma omp parallel for
	for (size_t eMCS = 0; eMCS < width(); eMCS++) {/* begins changes */
		auto i1 = rng.next() % width();
		auto j1 = rng.next() % width();
		int neighbor = rng.next() % 4;
		auto i2 = 0, j2 = 0;
		switch (neighbor) {
		case 0:  i2 = nov[i1];   j2 = j1;        break;
		case 1:  i2 = i1;        j2 = nov[j1];   break;
		case 2:  i2 = csok[i1];  j2 = j1;        break;
		case 3:  i2 = i1;        j2 = csok[j1];  break;
		} /* case neighbor */
		auto s1 = at(i1, j1);
		auto s2 = at(i2, j2);
		uint8_t sk = pm[s1][s2];
		double r = 0;

		switch (sk) {
		case 0: break;
		case 1: at(i2, j2) = s1;
			break;
		case 2: at(i1, j1) = s2;
			break;
		case 3: r = rng.nextNormal();
			if (r<Px) {
				at(i1, j1) = s2;
				at(i2, j2) = s1;
			}
			break;
		case 4: r = rng.nextNormal();
			if (r<alfa) {
				at(i2, j2) = s1;
			}
			break;
		case 5: r = rng.nextNormal();
			if (r<alfa) {
				at(i1, j1) = s2;
			}
			break;
		} /*case sk */
	} /* done with changes */
}

void Sim_tox2x::reset()		/* creates a random strategy distribution across the whole grid */
{
	for (size_t i = 0; i < width(); i++) {      /* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			at(i, j) = rng.next() % Ns;
			//Nst[at(i,j)]++
		}
	}
}

size_t Sim_tox2x::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}

