#include "Evolution.h"
#include "w_MainWindow.h"
#include "graph_Simulation.h"
#include "h_Simulation.h"

#include "w_widget_Boxes.h"
#include "ui_w_widget_Boxes.h"

w_widget_Boxes::w_widget_Boxes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_widget_Boxes)
{
    ui->setupUi(this);
}

w_widget_Boxes::~w_widget_Boxes()
{
    delete ui;
}


void w_widget_Boxes::init(w_MainWindow* aMainWindow, Evolution* anEvolution,
                          h_Simulation* aH_simulation, graph_Simulation* aGraph_simulation)
{
    graph_sim = aGraph_simulation;

    ui->w_widget_Evo->init(aMainWindow, anEvolution);
    ui->w_widget_Sim->init(aMainWindow, aH_simulation, aGraph_simulation);

    QObject::connect(ui->w_tabWidget, SIGNAL(currentChanged(int)),
                     this, SLOT(changeTab(int)));
}


// Avoid that simulation runs when we are on the evolution tab
/*slot*/ void w_widget_Boxes::changeTab(int newTab)
{
    if(ui->w_tabWidget->currentWidget() != ui->w_widget_Sim)
        graph_sim->pauseSim();
    else
        graph_sim->resumeSim();
}
