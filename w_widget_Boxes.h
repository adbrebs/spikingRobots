#ifndef W_WIDGET_BOXES_H
#define W_WIDGET_BOXES_H

class w_MainWindow;

class graph_Simulation;
class Evolution;
class h_Simulation;

#include <QWidget>

namespace Ui {
    class w_widget_Boxes;
}

class w_widget_Boxes : public QWidget
{
    Q_OBJECT

public:
    explicit w_widget_Boxes(QWidget *parent = 0);
    ~w_widget_Boxes();

    void init(w_MainWindow* aMainWindow, Evolution* anEvolution, h_Simulation* aH_simulation, graph_Simulation* aGraph_simulation);

public slots:
    void changeTab(int newTab);

private:
    Ui::w_widget_Boxes *ui;

    graph_Simulation* graph_sim;
};

#endif // W_WIDGET_BOXES_H
