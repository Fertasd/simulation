#include "simulation.h"

Simulation::Simulation(size_t width) : _data(width * width), _width(width)
{
	/* conveniently empty, as this is a parent class with virtual functions
	 * that get defined in the child classes(specific simulations) */
}

Simulation::~Simulation()
{
}

void Simulation::setSync(size_t i, size_t j, uint8_t value)
{
	std::unique_lock<std::mutex> lock(_dataMutex);
	at(i, j) = value;
}
