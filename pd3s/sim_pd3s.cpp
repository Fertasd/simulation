#include "sim_pd3s.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_pd3s::Sim_pd3s(size_t width) : Simulation(width), rng(2593), csok(width), nov(width), data2(width * width),
	pm{ { 0.0, tempt, 0.0},
		{0.0, 1.0, 1.0},
		{-q, 1.0 - q, 1.0 - q}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0xFF000000);		/* UC: defines the color table */
	palette().push_back(0x00FFFFFF);
	palette().push_back(0x000000ED);

	reset();		/* assigns a random strategy distribution to the simulation */
}

QString Sim_pd3s::name() const
{
	return "PD3S";		/* UC: returns the name of the simulation */
}

uint32_t Sim_pd3s::renderFrameSkip() const
{
	return 1;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_pd3s::displayWidth() const
{
	return 120;
}

uint32_t Sim_pd3s::animationDelay() const
{
	return 200;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

void Sim_pd3s::step()		/* defines a simulation step */
{
	auto NeS = width()*width();

	#pragma omp parallel for
	for(size_t ii=0; ii<NeS; ii++)		/* begins changes */
	{
		auto ix = (int) (rng.next() % width());
		auto jx = (int) (rng.next() % width());
		auto SX = at(ix,jx);
		auto ri = rng.next() % zz;
		auto ip = nov[ix], jp = nov[jx];
		//auto iq = nov[ip], jq = nov[jp];
		auto im = csok[ix], jm = csok[jx];
		//auto il = csok[im], jl = csok[jm];
		auto iy = 0, jy = 0;
		switch (ri) {
		case 0:   iy = ip;   jy = jx;   break;
		case 1:   iy = ix;   jy = jp;   break;
		case 2:   iy = im;   jy = jx;   break;
		case 3:   iy = ix;   jy = jm;   break;
		} /* case neighbor */


		auto r = rng.nextNormal();
		if (r<xx) {
			iy = rng.next() % width();
			jy = rng.next() % width();

		}
		auto SY = at(iy,jy);
		if (SX != SY) {

			auto PX=0.0;
			size_t iz=ip,  jz=jx;
			auto s=at(iz,jz);
			PX+=pm[SX][s];
			iz=ix;
			jz=jp;
			s=at(iz,jz);
			PX+=pm[SX][s];
			iz=im;
			jz=jx;
			s=at(iz,jz);
			PX+=pm[SX][s];
			iz=ix;
			jz=jm;
			s=at(iz,jz);
			PX+=pm[SX][s];

			ip = nov[iy];      jp = nov[jy];
			//iq = nov[ip];      jq = nov[jp];
			im = csok[iy];     jm = csok[jy];
			//il = csok[im];     jl = csok[jm];

			auto PY=0.0;
			iz=ip;
			jz=jy;
			s=at(iz,jz);
			PY+=pm[SY][s];
			iz=iy;
			jz=jp;
			s=at(iz,jz);
			PY+=pm[SY][s];
			iz=im;
			jz=jy;
			s=at(iz,jz);
			PY+=pm[SY][s];
			iz=iy;
			jz=jm;
			s=at(iz,jz);
			PY+=pm[SY][s];

			auto prd = 1.0 / (1 + exp(-(PY - PX)/Temp));
			r = rng.nextNormal();
			if ( r<prd) {
				at(ix,jx) = SY;
			}
			r = rng.nextNormal();
			if (r<yy) {
				at(ix,jx) = (int) rng.next() % Ns;
			}
		} /* done with changes */


	}
}

void Sim_pd3s::reset()		/* creates a random strategy distribution across the whole grid */
{
	for (size_t i = 0; i < width(); i++) {      /* random strategy distribution */
		for (size_t j = 0; j < width(); j++) {
			at(i, j) = rng.next() % Ns;
			//Nst[at(i,j)]++
		}
	}
}  /*MCS*/

size_t Sim_pd3s::stepTargetNumber() const
{
	return 500000;		/* returns the maximum number of steps, currently unused */
}
