#pragma once
#include <mutex>
#include <vector>
#include <QString>
#include <QColor>

class Simulation						/* the parent class for all simulations */
{
public:
	explicit Simulation(size_t width);					/* initializes a simulation */
	virtual ~Simulation() = default;
	virtual QString name() const = 0;					/* virtual declarations of the functions that give us the name,
														  animation delay(time between picture updates) and
														  render frame skip(how many steps the simulation runs before
														  updating the picture) of the simulation */
	virtual uint32_t animationDelay() const = 0;
	virtual uint32_t renderFrameSkip() const = 0;
	virtual size_t displayWidth() const = 0;
	virtual void step() = 0;							/* virtual declarations of the functions that describe a simulation step or a reset */
	virtual void reset() = 0;
	virtual size_t stepTargetNumber() const = 0;		/* virtually declares maximum number of steps run, currently unused */
	inline const std::vector<QColor>& palette() const { return _palette; }
	inline std::vector<QColor>& palette() { return _palette; }				/* declaration of the function that calls the color table */
	inline const std::vector<uint8_t>& data() const { return _data; }
	inline std::vector<uint8_t>& data() { return _data; }					/* declaration of a function that calls data from the simulation */
	inline size_t width() const { return _width; }							/* declares a function that returns the side length of a simulation */
	inline uint8_t& at(size_t i, size_t j) { return _data[i * _width + j]; }	/* declares a function that can efficiently call data from the simulation, unlike data() */
	void setSync(size_t i, size_t j, uint8_t value);
	inline const uint8_t& at(size_t i, size_t j) const { return _data[i * _width + j]; }
private:
	std::vector<uint8_t> _data; /*declaration of the vector that records the data of the simulation*/
	std::vector<QColor> _palette; /* declaration of the color table */
	size_t _width;				/* declaration of size length of the simulation */
	std::mutex _dataMutex;
};
