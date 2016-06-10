#include "sim_rps_ca.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_RPS_CA::Sim_RPS_CA(size_t width) : Simulation(width), rng(2593), csok(width), nov(width), data2(width * width),
	pm{ {0.0, 1.0, -1.0},	/* UC: define the payoff matrix here */
		{-1.0, 0.0, 1.0},
		{1.0, -1.0, 0.0}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0xFF000000);		/* UC: defines the color table */
	palette().push_back(0x00FFFFFF);
	palette().push_back(0x000000FF);

	reset();								/* assigns a random strategy distribution to the simulation */
}

QString Sim_RPS_CA::name() const
{
	return "RPS_CA";				/* UC: returns the name of the simulation */
}

uint32_t Sim_RPS_CA::renderFrameSkip() const
{
	return 1;						/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_RPS_CA::displayWidth() const
{
	return 120;
}

uint32_t Sim_RPS_CA::animationDelay() const
{
	return 200;						/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

void Sim_RPS_CA::step()				/* defines a simulation step */
{
	size_t su0 = 0, su1 = 1, su2 = 2;

	#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < width(); i++) {/* begins changes */
		for (size_t j = 0; j < width(); j++) {
			auto ip = nov[i], im = csok[i];
			auto jp = nov[j], jm = csok[j];
			double PX0 = 0.0, PX1 = 0.0, PX2 = 0.0;
			auto sn = at(ip, j);
			PX0 += pm[su0][sn];  PX1 += pm[su1][sn];  PX2 += pm[su2][sn];
			sn = at(i,jp);
			PX0 += pm[su0][sn];  PX1 += pm[su1][sn];  PX2 += pm[su2][sn];
			sn = at(im,j);
			PX0 += pm[su0][sn];  PX1 += pm[su1][sn];  PX2 += pm[su2][sn];
			sn = at(i,jm);
			PX0 += pm[su0][sn];  PX1 += pm[su1][sn];  PX2 += pm[su2][sn];

			auto x0 = exp(PX0 / K);
			auto x1 = exp(PX1 / K);
			auto x2 = exp(PX2 / K);
			auto xs = x0 + x1 + x2;
			auto r0 = x0 / xs, r1 = x1 / xs;
			uint8_t sr = 1;
			//auto r = (x / y);
			double h = rng.nextNormal();
			if (h<r0) { sr = 0; }
			if (h>(r0 + r1)) { sr = 2; }
			data2[i * width() + j] = sr;
		}
	}/* done with changes */

	for (size_t i = 0; i < width(); i++)
		for (size_t j = 0; j < width(); j++)
			at(i, j) = data2[i * width() + j];
	/* document changes */
}

void Sim_RPS_CA::reset() /* creates a random strategy distribution across the whole grid */
{
	for (size_t i = 0; i < width(); i++) {      /* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			at(i, j) = rng.next() % Ns;
			//Nst[at(i,j)]++
		}
	}
}

size_t Sim_RPS_CA::stepTargetNumber() const
{
	return 50000;   /* returns the maximum number of steps, currently unused */
}
