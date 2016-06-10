#include "simulation.h"

SimParameter::Data::Data(const QString &name, double value, const QString &description)
	: _name(name), _value(value), _description(description)
{

}

SimParameter::SimParameter(const QString& name, double value, const QString &description)
	: _data(std::make_shared<Data>(name, value, description))
{
}

Simulation::Simulation(size_t width) : _data(width * width), _width(width)
{
	/* conveniently empty, as this is a parent class with virtual functions
	 * that get defined in the child classes(specific simulations) */
}

Simulation::~Simulation()
{
}
