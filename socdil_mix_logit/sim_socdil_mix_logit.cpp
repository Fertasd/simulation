#include "sim_socdil_mix_logit.h"

/* UC stands for user-controlled, modify the values in these rows to tailor the simulation to your needs */

Sim_socdil_mix_logit::Sim_socdil_mix_logit(size_t width) : Simulation(width), csok(width), nov(width), data2(width * width),
	pm{ {P, T, w*T}, /* UC: the payoff matrix is defined here */
		{S, R, w*R+(1-w)*S},
		{w*S, w*R+(1-w)*T, w*w*R+w*(1-w)*(T+S)}}
{	/* upon the creation of an object of this class, the following happens: */
	for (size_t i = 0; i < width; i++) { nov[i] = i + 1; csok[i] = i - 1; }
	nov[width - 1] = 0;   csok[0] = width - 1;		/* the index-resolving vectors get defined */

	palette().push_back(0x00FFFFFF);		/* UC: defines the color table */
	palette().push_back(0xFF00FFFF);
	palette().push_back(0xFFFFFF00);

	reset();		/* assigns a random strategy distribution to the simulation */
}

std::vector<SimParameter> Sim_socdil_mix_logit::parameters()
{
	return {T, R, S, P, w, zz, Temp};
}

uint32_t Sim_socdil_mix_logit::animationDelay() const
{
	return 0;		/* UC: returns the animation delay of the simulation(how long it waits before displaying the next state, in ms) */
}

uint32_t Sim_socdil_mix_logit::renderFrameSkip() const
{
	return 50;		/* UC: returns the frame skip of the simulation(how many steps are done in the background) */
}

size_t Sim_socdil_mix_logit::displayWidth() const
{
	return 150;
}

void Sim_socdil_mix_logit::step()		/* defines a simulation step */
{
	#pragma omp parallel for
	for(size_t ii = 0; ii < width(); ii++)
		  {
		   size_t ix = rng.next() % width();
		   size_t jx = rng.next() % width();

		   auto ip = nov[ix], jp = nov[jx];
		   auto im = csok[ix], jm = csok[jx];

		   double po0=0.0, po1 = 0.0, po2 = 0.0;
		   po0+=pm[0][at(ip, jx)]; po1+=pm[1][at(ip, jx)];  po2+=pm[2][at(ip, jx)];
		   po0+=pm[0][at(ix, jp)]; po1+=pm[1][at(ix, jp)];  po2+=pm[2][at(ix, jp)];
		   po0+=pm[0][at(im, jx)]; po1+=pm[1][at(im, jx)];  po2+=pm[2][at(im, jx)];
		   po0+=pm[0][at(ix, jm)]; po1+=pm[1][at(ix, jm)];  po2+=pm[2][at(ix, jm)];

		   auto x0 = exp(po0/Temp);
		   auto x1 = exp(po1/Temp);
		   auto x2 = exp(po2/Temp);
		   auto xs = x0 + x1 + x2;
		   po0 = x0/xs;  po1 = po0+ x1/xs;
		   uint8_t SY = 1;
		   double r = rng.nextNormal();
		   if ( r<po0) { SY = 0; }
		   if ( r>po1) { SY = 2; }
		   at(ix, jx) = SY;
}
//}
}

void Sim_socdil_mix_logit::reset()		/* creates a random strategy distribution across the whole grid */
{
	for (size_t ix = 0; ix < width(); ix++) { /* random strategy distribution */
		for (size_t jx = 0; jx < width(); jx++) {
			 at(ix,jx) = rng.next() % Ns;
								}
	}

}

size_t Sim_socdil_mix_logit::stepTargetNumber() const
{
	return 50000;		/* returns the maximum number of steps, currently unused */
}

uint8_t Sim_socdil_mix_logit::bisectStep(SimParameter, double)
{
	return 0;
}

std::vector<QPair<double, double> > Sim_socdil_mix_logit::bisectionAnalysis(SimParameter, SimParameter)
{
	return {};
}

double Sim_socdil_mix_logit::bisect(SimParameter)
{
	return 0;
}

