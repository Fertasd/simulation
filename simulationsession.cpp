#include "simulationsession.h"

SimulationSession::SimulationSession(QObject *parent) : QObject(parent), _animationDelay(200), _renderFrameSkip(0)
{
}

void SimulationSession::simulation(const std::shared_ptr<Simulation> &simulation)
{
	_simulation = simulation;
	emit simulationChanged(_simulation);
	animationDelay(_simulation->animationDelay());
	renderFrameSkip(_simulation->renderFrameSkip());
	displayWidth(_simulation->displayWidth());
}

void SimulationSession::animationDelay(uint32_t delay)
{
	_animationDelay = delay;
	emit animationDelayChanged(_animationDelay);
}

void SimulationSession::renderFrameSkip(uint32_t frames)
{
	_renderFrameSkip = frames;
	emit renderFrameSkipChanged(_renderFrameSkip);
}

void SimulationSession::displayWidth(size_t dpwidth)
{
	_displayWidth = dpwidth;
	emit displayWidthChanged(_displayWidth);
}
