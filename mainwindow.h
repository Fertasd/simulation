#pragma once

#include <thread>
#include <QMainWindow>
#include "simulationmanager.h"
#include "simulationsession.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);  /* creates the display window */
private:
	SimulationManager simulationManager;		/* adds a simulation manager object to the window */
	//std::shared_ptr<Simulation> simulation;    /* declares a pointer to a simulation */
	QTimer* timer;								/* declares a timer object */
	SimulationSession session;
	std::thread worker;
};
