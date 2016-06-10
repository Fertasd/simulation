#include "sim_lv6s2a.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_LV6s2a::Sim_LV6s2a(size_t width) : Simulation(width), rng(2593), csok(width), nov(width), data2(width * width),
	pm{ {0, 1, 1, 2, 3, 3}, /* UC: the payoff matrix is defined here */
		{3, 0, 1, 1, 2, 3},
		{3, 3, 0, 1, 1, 2},
		{2, 3, 3, 0, 1, 1},
		{1, 2, 3, 3, 0, 1},
		{1, 1, 2, 3, 3, 0}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table */
	palette().push_back(0x00FF0000);
	palette().push_back(0x0033CC00);
	palette().push_back(0xFF000000);
	palette().push_back(0x00FFFF00);
	palette().push_back(0x000000FF);

	reset();		/* assigns a random strategy distribution to the simulation */
}

uint32_t Sim_LV6s2a::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_LV6s2a::renderFrameSkip() const
{
	return 1200;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_LV6s2a::displayWidth() const
{
	return 120;
}

void Sim_LV6s2a::step()		/* defines a simulation step */
{
	#pragma omp parallel for
	for (size_t eMCS = 0; eMCS < width(); eMCS++) {/* begins changes */
		auto i1 = rng.next() % width();
		auto j1 = rng.next() % width();
		auto neighbor = rng.next() % 4;
		auto i2 = 0, j2 = 0;
		switch (neighbor) {
		case 0:  i2 = nov[i1];   j2 = j1;        break;
		case 1:  i2 = i1;        j2 = nov[j1];   break;
		case 2:  i2 = csok[i1];  j2 = j1;        break;
		case 3:  i2 = i1;        j2 = csok[j1];  break;
		} /* case neighbor */
		auto s1 = at(i1, j1);
		auto s2 = at(i2, j2);
		double r = 0;
		uint8_t sk = pm[s1][s2];

		switch (sk) {
		case 0: break;
		case 1: at(i2,j2) = s1;
			//Nst[s1]++;
			//Nst[s2]--;
			break;
		case 2: r = rng.nextNormal();
			if (r<Px) {
				at(i1, j1) = s2;
				at(i2, j2)=s1;
			}
			break;
		case 3: at(i1, j1) = s2;
			//Nst[s1]--;
			//Nst[s2]++;
			break;
		} /*case sk */
	} /* done with changes */
}

void Sim_LV6s2a::reset()		/* creates a random strategy distribution across the whole grid */
{
	for (size_t i = 0; i < width(); i++) {      /* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			at(i, j) = rng.next() % Ns;
			//Nst[at(i,j)]++
		}
	}
}

size_t Sim_LV6s2a::stepTargetNumber() const
{
	return 5000;		/* returns the maximum number of steps, currently unused */
}
