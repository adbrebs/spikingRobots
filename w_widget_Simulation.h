#ifndef W_WIDGET_SIMULATION_H
#define W_WIDGET_SIMULATION_H

class w_MainWindow;

class h_Simulation;
class graph_Simulation;

#include <QWidget>

namespace Ui {
class w_widget_Simulation;
}

class w_widget_Simulation : public QWidget
{
    Q_OBJECT

public:
    explicit w_widget_Simulation(QWidget *parent = 0);
    ~w_widget_Simulation();

    void init(w_MainWindow* aMainWindow, h_Simulation* aH_simulation, graph_Simulation* aGraph_simulation);

public slots:
    void updateIterationValue(int iteration);
    void updateTimeOut(int time);
    void pauseButton(bool var);
    void pause();
    void resume();
    void loadBestNetFromFile();

    void addFood(bool checked);
    void addPoison(bool checked);
    void clearAllItems();
    void reInitAllItems();
    void reIniRobotsPos();
    void changePathLength(int newLength);

    void saveWholeViewToFile();
    void saveCurViewToFile();

private:
    void setScene();

private:
    Ui::w_widget_Simulation *ui;

    w_MainWindow* mainWindow;
    h_Simulation* h_simulation;
    graph_Simulation* graph_sim;

};

#endif // W_WIDGET_SIMULATION_H
