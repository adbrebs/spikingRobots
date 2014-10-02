#include "Settings.h"
#include "graph_Simulation.h"
#include "Evolution.h"
#include "h_Simulation.h"

#include "w_MainWindow.h"
#include "ui_w_MainWindow.h"




w_MainWindow::w_MainWindow(Settings* settings)
    : s(settings),
      ui(new Ui::w_MainWindow)
{
    // Set graphical interface
    ui->setupUi(this);

    // Set title
    setWindowTitle(QString::fromUtf8("Evolving robots controlled by spiking neural networks - Alexandre de Brébisson"));

    // Creation of the simulation for the evolution
    simEv = s->createSim(s, 1, s->ev.numFoods,
                              s->ev.numPoisons, s->ev.worldWidth);

    // Creation of the evolution
    evolution = new Evolution(s, simEv);

    // Creation of the simulation for graphical visualization
    simulation = s->createSim(s, s->sim.numRobots, s->sim.numFoods, s->sim.numPoisons,
                                   s->sim.worldWdith);

    // Creation of the graphical simulation
    graph_sim = new graph_Simulation(s, simulation, this);

    // Connections
    ui->w_central_widget->init(this, evolution, simulation, graph_sim);

    graph_sim->startSim();
}


w_MainWindow::~w_MainWindow()
{
    delete simEv;
    delete evolution;
    delete simulation;
    delete graph_sim;
    delete ui;
}


void w_MainWindow::launchEvolution(int numSim)
{
    graph_sim->pauseSim();
    evolution->evolve(numSim);
    evolution->updateSimulation(simulation);
}
