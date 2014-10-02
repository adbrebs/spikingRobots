#ifndef W_MAINWINDOW_H
#define W_MAINWINDOW_H

#include <QMainWindow>

class Settings;
class graph_Simulation;
class Evolution;
class h_Simulation;

class w_Evolution;

namespace Ui {
class w_MainWindow;
}

class w_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    w_MainWindow(Settings* settings);
    ~w_MainWindow();

public:
    graph_Simulation* getGraph_sim() const {return graph_sim;}

    void launchEvolution(int numSim);

private:
    Settings* s;

    h_Simulation* simEv;
    Evolution* evolution;
    h_Simulation* simulation;
    graph_Simulation* graph_sim;

    QDockWidget *statDock;

    w_Evolution* w_evolution;

    Ui::w_MainWindow *ui;
};


#endif
