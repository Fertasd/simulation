#include "coordtest/sim_coordtest.h"
#include <stdlib.h>
#include <exception>

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_coordtest::Sim_coordtest(size_t width) : Simulation(width), csok(width), nov(width), data2(width * width)
  /* UC: the payoff matrix is defined here */
{
	/* upon the creation of an object of this class, the following happens: */
	res().resize(2);
	auto changedEvent = [=] (double) {
		pm[0][0] = cop+sep+crp; pm[0][1] = -cop+sep-crp;
		pm[1][0] = -cop-sep+crp; pm[1][1] = cop-sep-crp;
	};

	cop.onChanged(changedEvent);
	crp.onChanged(changedEvent);
	sep.onChanged(changedEvent);
	changedEvent(0);

	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table */
	palette().push_back(0xFF00FFFF);
	//palette().push_back(0xFFFFFFFF);
	selectParameter1().linkTo(sep);
	selectParameter2().linkTo(crp);

	reset();		/* assigns a random strategy distribution to the simulation */
}

std::vector<SimParameter> Sim_coordtest::parameters()
{
	return {cop, sep, crp, startselect, range1Min, range1Max, biStep, range2Min, range2Max, zz, Temp};
}

uint32_t Sim_coordtest::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_coordtest::renderFrameSkip() const
{
	return 50;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_coordtest::displayWidth() const
{
	return 500;
}

void Sim_coordtest::step()		/* defines a simulation step */
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

void Sim_coordtest::reset()		/* creates a specific strategy distribution across the grid */
{
	switch (static_cast<uint8_t>(startselect.value())){
	case 1:
		for (size_t ix = 0; ix < 100; ix++) /* c region */
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = 0;

		for (size_t ix = 100; ix < width(); ix++) /* d region */
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = 1;
		break;
	case 2:
		for (size_t ix = 0; ix < width(); ix++) /* random strategy distribution */
			for (size_t jx = 0; jx < width(); jx++)
				 at(ix, jx) = rng.next() % Ns;
		break;
	case 3:
		createDomains();
		break;
	case 4:
		for (size_t ix = 0; ix < 100; ix++) {/* c region*/
			for (size_t jx = 0; jx < width(); jx++){
				 at(ix, jx) = 0;
				 res()[0] += 1;
			}
		}
		break;
	case 5:
		for (size_t ix = 0; ix < width(); ix++) {/* random strategy distribution*/
			for (size_t jx = 0; jx < width(); jx++){
				 at(ix, jx) = rng.next() % Ns;
				 res()[at(ix, jx)] += 1;
			}
		}
		break;
	case 6:
		createDomains();
		break;
	case 7:
		for (size_t ix = 0; ix < width(); ix++){
			for (size_t jx = 0; jx < ix; jx++)
				at(ix, jx) = 0;
			for (size_t kx = ix; kx < width(); kx++)
				at(ix, kx) = 1;
		}
		break;
	}
}

size_t Sim_coordtest::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}

void Sim_coordtest::createDomains()
{
	auto target1 = rng.next() % 10;
	auto target2 = rng.next() % 10;
	for (size_t ix = 0; ix < width(); ix++){
		for (size_t jx = 0; jx < width(); jx++){
			if ((ix % 10) < target1) {
				if ((jx % 10) < target2){
					if (rng.nextNormal() < 0.05)
						at(ix, jx) = 0;
					else
						at(ix, jx) = 1;
				}
				else
					at(ix, jx) = rng.next() % Ns;
			}
			else
				at(ix, jx) = rng.next() % Ns;
		}
	}
}

uint8_t Sim_coordtest::bisectStep(SimParameter param, double newValue)
{
	param = newValue;
	reset();
	for (size_t i = 0; i < 200000; i++){
		if (res()[0] == width()*width()){
			return 0;
			break;
		}
		if (res()[1] == width()*width()){
			return 1;
			break;
		}
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
			  res()[SX] -= 1;
			  res()[SY] += 1;
			  at(ix, jx) = SY;

		  }
	}
		 //}
	}
	throw std::runtime_error("no absorbing state found");
}

std::vector<QPair<double, double>> Sim_coordtest::bisectionAnalysis(SimParameter param1, SimParameter param2)
{
	std::vector<QPair<double, double>> retval;
	for (double i = range1Min; i < range1Max; i += biStep){
		param1 = i;
		retval.push_back( QPair<double, double>(i, bisect(param2)) );
	}
	return retval;
}

double Sim_coordtest::bisect(SimParameter param)
{
	double minp = range2Min, maxp = range2Max, retek = 0;
	uint8_t minstrat = bisectStep(param,minp);
	while (maxp - minp > 0.000001){
		double halfp = minp + ((abs(minp)+maxp)/2);
		retek = halfp;
		if (bisectStep(param, halfp) == minstrat){
			minp = halfp;
			}
		else maxp = halfp;
			}
	return retek;
}




