#include "simulationmanager.h"
#include "LV6s2a/sim_lv6s2a.h"
#include "NM/sim_nm.h"
#include "pd3s/sim_pd3s.h"
#include "rps_ca/sim_rps_ca.h"
#include "tox2x/sim_tox2x.h"

/* UC: include the header(.h) file for any simulation you want to display here in a similar fashion
	example: #include "newsim/sim_newsim.h" */

SimulationManager::SimulationManager(QObject *parent)
	: QAbstractItemModel(parent), _data({
										std::make_shared<Sim_LV6s2a>(),
										std::make_shared<Sim_NM>(),
										std::make_shared<Sim_pd3s>(),
										std::make_shared<Sim_RPS_CA>(),
										std::make_shared<Sim_tox2x>()
										/* UC: add a similar line with the class name of the simulation
										   you're adding to the display and have commas after all
											but the last row, example:
											std::make_shared<Sim_newSim>() */
										})		/* gives the Simulation Manager the simulations to work with */
{

}


QModelIndex SimulationManager::index(int row, int column, const QModelIndex &) const
{
	return createIndex(row, column, row);		/* used to call the index of elements */
}

QModelIndex SimulationManager::parent(const QModelIndex &) const
{
	return QModelIndex();						/* always returns an invalid index, has to exist but is unused */
}

int SimulationManager::rowCount(const QModelIndex &) const
{
	return _data.size();						/* returns the length of the list of simulations */
}

int SimulationManager::columnCount(const QModelIndex &) const
{
	return 1;									/* returns the number of columns in the list of simulations, always 1 */
}

QVariant SimulationManager::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)				/* if called to display, returns the name of the simulation with a given index,
												  otherwise returns invalid(default) */
		return _data[index.row()]->name();
	return QVariant();
}

std::shared_ptr<Simulation> SimulationManager::getSimulation(size_t index) const
{
	return _data[index]; /* returns a pointer to the simulation with given index */
}
