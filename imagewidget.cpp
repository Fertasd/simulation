#include "imagewidget.h"
#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
}

void ImageWidget::setSimulation(const std::shared_ptr<Simulation> &sim)
{
	_simulation = sim;			/* links a simulation pointer to the display */
}

void ImageWidget::setDisplayWidth(size_t dpw)
{
	_dpwidth = dpw;
}



void ImageWidget::paintEvent(QPaintEvent*)		/* defines what happens when the display gets painted */
{
	if (!_simulation)			/* if there is no simulation, no painting occurs */
		return;

	const int minside = std::min(width(), height());
	QRect square((width() - minside)/2,0, minside, minside);
	//const int step = minside/(int)_simulation->displayWidth();
	const int step = minside/static_cast<int>(_dpwidth);

	QPainter painter(this); /* creates a painter object(QPainter) that fills every square with the color corresponding to its strategy */
	//for (size_t i = 0; i < _simulation->displayWidth(); ++i)
	for (int i = 0; i < static_cast<int>(_simulation->width()); ++i)
	{
		//for (size_t j = 0; j < _simulation->displayWidth(); ++j)
		for (int j = 0; j < static_cast<int>(_simulation->width()); ++j)
		{
			painter.fillRect(j*step + square.x(), i*step + square.y(), step, step,_simulation->palette()[_simulation->at(i,j)]);
		}
	}
}
