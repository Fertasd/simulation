#include "sim_nm.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_NM::Sim_NM(size_t width) : Simulation(width), csok(width), nov(width), data2(width * width),pos(width*width),
	pm{ {0.0, tempt},		/* UC: define the payoff matrix here */
		{0.0, 1.0}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0xFF000000);		/* UC: defines the color table */
	palette().push_back(0x00FFFFFF);

	reset();		/* assigns a random strategy distribution to the simulation */
}

std::vector<SimParameter> Sim_NM::parameters()
{
	return {tempt};
}

uint32_t Sim_NM::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_NM::displayWidth() const
{
	return 120;
}

uint32_t Sim_NM::animationDelay() const
{
	return 1000;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}
void Sim_NM::step()		/* defines a simulation step */
{
	#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < width(); i++) {/* begins changes */
		for (size_t j = 0; j < width(); j++) {
			auto s0 = at(i,j);
			pos[i*width()+j] += pm[s0][s0];
			auto in = i, jn = nov[j];
			auto sn = at(in, jn);
			pos[i*width()+j] += pm[s0][sn];
			pos[in*width()+jn] += pm[sn][s0];
			in = nov[i];
			sn = at(in,jn);
			pos[i*width()+j] += pm[s0][sn]; pos[in*width()+jn] += pm[sn][s0];
			jn = j;
			sn = at(in, jn);
			pos[i*width()+j] += pm[s0][sn]; pos[in*width()+jn] += pm[sn][s0];
			jn = csok[j];
			sn = at(in,jn);
			pos[i*width()+j] += pm[s0][sn]; pos[in*width()+jn] += pm[sn][s0];
		}
	}
	#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			data2[i*width()+j] = at(i,j);
			uint32_t po1 = pos[i*width()+j], po2 = po1;
			auto in = i, jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = j;
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = nov[i];
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = i;
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = csok[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = j;
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
			in = csok[i];
			jn = nov[j];
			po2 = pos[in*width()+jn];
			if (po2>po1) { po1 = po2; data2[i*width()+j] = at(in,jn); }
		}
	}/* done with changes */

	for (size_t i = 0; i < width(); i++) {
		for (size_t j = 0; j < width(); j++) {
			//auto s0 = 3*s2[i][j];
			at(i,j) = data2[i*width()+j];
			pos[i*width()+j] = 0.0;
		}
	}/* document changes */
}

void Sim_NM::reset()		/* creates a random strategy distribution across the whole grid */
{
	for (size_t i = 0; i < width(); i++) {      /* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			at(i, j) = rng.next() % Ns;
			//Nst[at(i,j)]++
		}
	}
}

size_t Sim_NM::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}
