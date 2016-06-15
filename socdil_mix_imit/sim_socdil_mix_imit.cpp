#include "sim_socdil_mix_imit.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_socdil_mix_imit::Sim_socdil_mix_imit(size_t width) : Simulation(width), csok(width), nov(width), data2(width * width),
	pm{ {P, T, w*T}, /* UC: the payoff matrix is defined here */
		{S, R, w*R+(1-w)*S},
		{w*S, w*R+(1-w)*T, w*w*R+w*(1-w)*(T+S)}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table */
	palette().push_back(0xFF00FFFF);
	palette().push_back(0xFFFFFFFF);

	reset();		/* assigns a random strategy distribution to the simulation */
}

std::vector<SimParameter> Sim_socdil_mix_imit::parameters()
{
	return {T, R, S, P, w, zz, Temp};
}

uint32_t Sim_socdil_mix_imit::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_socdil_mix_imit::renderFrameSkip() const
{
	return 50;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_socdil_mix_imit::displayWidth() const
{
	return 150;
}

void Sim_socdil_mix_imit::step()		/* defines a simulation step */
{
	#pragma omp parallel for
	 for(size_t ii=0; ii<width(); ii++)
	  {
	   size_t ix = rng.next() % width();
	   size_t jx = rng.next() % width();
	   uint8_t SX = at(ix, jx);
	   size_t ri = rng.next() % static_cast<uint32_t>(zz);
	   auto ip = nov[ix], jp = nov[jx];
	   auto im = csok[ix], jm = csok[jx];
	   auto iy = 0, jy = 0;

	  switch (ri) {
		 case 0:   iy = ip;   jy = jx;   break;
		 case 1:   iy = ix;   jy = jp;   break;
		 case 2:   iy = im;   jy = jx;   break;
		 case 3:   iy = ix;   jy = jm;   break;
						  }

	   uint8_t SY = at(iy, jy);
	   /*if (SX != SY) {*/

	   double PX=0.0;
	   PX+=pm[SX][at(ip, jx)];
	   PX+=pm[SX][at(ix, jp)];
	   PX+=pm[SX][at(im, jx)];
	   PX+=pm[SX][at(ix, jm)];

	   ip = nov[iy];      jp = nov[jy];
	   im = csok[iy];     jm = csok[jy];

	   double PY=0.0;
	   PY+=pm[SY][at(ip, jy)];
	   PY+=pm[SY][at(iy, jp)];
	   PY+=pm[SY][at(im, jy)];
	   PY+=pm[SY][at(iy, jm)];
	  double prd = 1.0 / (1 + exp(-(PY - PX)/Temp));
	  double r = rng.nextNormal();
	  if ( r<prd) {
		   at(ix, jx) = SY;
	  }
}
//}
}

void Sim_socdil_mix_imit::reset()		/* creates a specific strategy distribution across the grid */
{
	for (size_t ix = 0; ix < 100; ix++) { /* c region */
		for (size_t jx = 0; jx < width(); jx++) {
			 at(ix, jx) = 0;
								}
	}
	for (size_t ix = 100; ix < width(); ix++) { /* d region */
		for (size_t jx = 0; jx < width(); jx++) {
			 at(ix, jx) = 1;
								}
	}
}

size_t Sim_socdil_mix_imit::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}


