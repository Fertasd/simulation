#include "mainwindow.h"
#include <QtWidgets>
#include "imagewidget.h"

template<typename _SignatureT> struct overload { };
template<typename _ReturnT, typename... _ArgsT>
struct overload<_ReturnT(_ArgsT...)>
{
	template<typename _ClassT>
	inline constexpr static auto get(_ReturnT(_ClassT::*ptr)(_ArgsT...)){ return ptr; }
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), shouldSimulate(false)
{   
	setWindowTitle("Simulation");			/* initializes the main window with a timer built in,
											  UC: sets initial size and window title */
	resize(800, 600);

	auto* timer = new QTimer(this);

	auto* mainPanel = new QWidget;
	setCentralWidget(mainPanel);
	auto* layout = new QHBoxLayout;
	mainPanel->setLayout(layout);			/* creates a main panel(QWidget), adds a horizontal box layout
											  (QHBoxLayout as the main interface */

	auto* image = new ImageWidget;
	layout->addWidget(image, 1);			/* creates a surface(ImageWidget) for displaying images,
											  adds it to the layout with weighing 1(helps with resizing issues) */
	connect(&session, &SimulationSession::simulationChanged, [=](const std::shared_ptr<Simulation>& sim){
		shouldSimulate = false;
		timer->stop();
		image->setSimulation(sim);
		image->update();
	});

	connect(&session, &SimulationSession::displayWidthChanged, [=](size_t dpw){
		shouldSimulate = false;
		timer->stop();
		image->setDisplayWidth(dpw);
		image->update();
	});

	auto* buttonLayout = new QVBoxLayout;
	//buttonLayout->setMaximumSize(60, 300);
	//buttonLayout->setSizeConstraint(QLayout::SetMaximumSize);
	layout->addLayout(buttonLayout, 0);		/* creates a vertical box layout(QVBoxLayout) as a surface for
											  displaying buttons and the simulation list, then adds it to the
												main layout with weighing 0(helps with resizing issues) */

	auto* startButton = new QPushButton("Start/Continue");	/* creates a button with a given name */
	buttonLayout->addWidget(startButton);					/* adds it to the vertical layout */
	auto* pauseButton = new QPushButton("Pause");			/* creates a button with a given name */
	buttonLayout->addWidget(pauseButton);					/* adds it to the vertical layout */
	auto* resetButton = new QPushButton("Reset");			/* creates a button with a given name */
	buttonLayout->addWidget(resetButton);					/* adds it to the vertical layout */

	auto* simList = new QListView;							/* creates an object(QListView) for displaying the list */
	buttonLayout->addWidget(simList);						/* adds it to the vertical layout */
	simList->setResizeMode(QListView::Adjust);				/* allows the list to be resized if necessary*/
	simList->setModel(&simulationManager);					/* specifies the Simulation Manager as the source of the list */

	auto* formLayout = new QFormLayout;
	buttonLayout->addLayout(formLayout);

	auto* dper = new QSpinBox;

	formLayout->addRow("Display width", dper);
	dper->setMinimum(60);
	dper->setMaximum(500);

	connect(dper, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.displayWidth(static_cast<size_t>(newValue)); });
	connect(&session, &SimulationSession::displayWidthChanged, [=](size_t newValue){ dper->setValue(static_cast<int>(newValue)); });

	auto* delayer = new QSpinBox;
	formLayout->addRow("Animation delay", delayer);
	delayer->setMinimum(0);
	delayer->setMaximum(3600000);

	connect(delayer, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.animationDelay(static_cast<uint32_t>(newValue)); });
	connect(&session, &SimulationSession::animationDelayChanged, [=](uint32_t newValue){ delayer->setValue(static_cast<int>(newValue)); });

	auto* skipper = new QSpinBox;
	formLayout->addRow("Frame skip", skipper);
	skipper->setMinimum(1);
	skipper->setMaximum(50000);

	connect(skipper, overload<void(int)>::get(&QSpinBox::valueChanged), [=](int newValue){ session.renderFrameSkip(static_cast<uint32_t>(newValue)); });
	connect(&session, &SimulationSession::renderFrameSkipChanged, [=](uint32_t newValue){ skipper->setValue(static_cast<int>(newValue)); });

	connect(simList->selectionModel(), &QItemSelectionModel::currentRowChanged, [=](const QModelIndex &current, const QModelIndex&){
		session.simulation(simulationManager.getSimulation(static_cast<size_t>(current.row())));
	});										/* a setup that links the selection of an item on the list to a series of events:
											  stops the timer, creates a simulation of the corresponding type, adjusts the timer
												timer appropriately and adds it to the display, then displays it */

	connect(timer, &QTimer::timeout, [=]{
		timer->stop();
		for (size_t i = 0; i < session.renderFrameSkip() && shouldSimulate; ++i)
		{
			session.simulation()->step(); /* do a certain number of simulation steps */
			QApplication::processEvents();
		}
		image->update();		/* display the current state of the simulation */
		if (shouldSimulate)
			timer->start();
	});
	connect(&session, &SimulationSession::animationDelayChanged, [=](uint32_t delay){ timer->setInterval(static_cast<int>(delay)); });
	connect(startButton, &QPushButton::clicked, [=]{ if (session.simulation()) { shouldSimulate = true; timer->start(); } });		/* on clicking the start and stop buttons, the timer starts and stops */
	connect(pauseButton, &QPushButton::clicked, [=]{ shouldSimulate = false; timer->stop(); });
	connect(resetButton, &QPushButton::clicked, [=]{
		if (session.simulation())
		{
			shouldSimulate = false;
			timer->stop();
			session.simulation()->reset();
			image->update();
		}
	}); /* connects the clicked() event of the reset button with a series of events: the timer stops, the simulation is reset and displayed */
}
