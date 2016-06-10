#pragma once

#include <memory>
#include <QObject>
#include "simulation.h"

class SimulationSession : public QObject
{
	Q_OBJECT
public:
	explicit SimulationSession(QObject *parent = 0);

	inline const std::shared_ptr<Simulation>& simulation() const { return _simulation; }
	void simulation(const std::shared_ptr<Simulation>& simulation);
	inline uint32_t animationDelay() const { return _animationDelay; }
	void animationDelay(uint32_t delay);
	inline uint32_t renderFrameSkip() const { return _renderFrameSkip; }
	void renderFrameSkip(uint32_t frames);
	inline size_t displayWidth() const { return _displayWidth; }
	void displayWidth(size_t dpwidth);

signals:
	void simulationChanged(const std::shared_ptr<Simulation>& newValue);
	void animationDelayChanged(uint32_t newValue);
	void renderFrameSkipChanged(uint32_t newValue);
	void displayWidthChanged(size_t newValue);
private:
	std::shared_ptr<Simulation> _simulation;
	uint32_t _animationDelay, _renderFrameSkip;
	size_t _displayWidth;
};
