#ifndef GRAPH_SIMULATION_H
#define GRAPH_SIMULATION_H

#include <QList>
#include <QPointF>

class QTimer;
class QGraphicsItem;
class QGraphicsRectItem;

class Settings;
class w_MainWindow;
class h_Simulation;
class graph_Scene;
class graph_Robot;
class graph_EatableItem;

#include <QObject>

class graph_Simulation : public QObject
{
    Q_OBJECT

public:
    graph_Simulation(Settings* settings, h_Simulation* aSimulation, w_MainWindow *aMainWindow);
    ~graph_Simulation();

    void deleteGraph_Items();

    void changeSimulation(h_Simulation* newSimulation);

    void startSim();
    void pauseSim();
    void resumeSim();

public:
    void addAllItemsToScene();
    void clearAllItems();
    void resetAllItems();

    void resetGraphRobot(graph_Robot* graph_robot);
    void resetGraphFood(graph_EatableItem *graph_food);
    void resetGraphPoison(graph_EatableItem *graph_poison);

    void addFood(QPointF pt);
    void addPoison(QPointF pt);

    void removeItem(graph_EatableItem* graph_eatableItem);

    void reInitializeAllItems();
    void reIniRobotsPos();

    void addGraphicsItem(QGraphicsItem *item);
    void removeGraphicsItem(QGraphicsItem *item);

public:
    QList<graph_Robot *> getGraph_Robots() const {return graph_robots;}
    QList<graph_EatableItem *> getgraph_Foods() const {return graph_foods;}
    QList<graph_EatableItem *> getgraph_Poisons() const {return graph_poisons;}
    graph_Scene* getScene() const {return scene;}

    qreal getTimeOutInterval() const;
    void setTimeOutInterval(qreal time);

    int getNumIteration() const {return numIteration;}
    h_Simulation* getSim() const {return simulation;}

    QPointF convertCoordinates(QPointF pt); // convert graphicsscene coordinatates to normal one

    void changeRobotsPathLengths(int newPath);

private:
    void addEatableItemsToScene();
    void createGraphEatableItems();
    void clearItems(QList<graph_EatableItem *>* pListItems);

public slots:
    void iterate();

signals:
    void sendIteration(int iteration);

private:
    Settings* s;

    h_Simulation* simulation;
    w_MainWindow* mainWindow;
    graph_Scene* scene;

    qreal timeOutInterval; // Time between two simulation steps
    QTimer* timer;
    int numIteration; // increment with the simulation steps

    QList<graph_Robot *> graph_robots;
    QList<graph_EatableItem *> graph_foods;
    QList<graph_EatableItem *> graph_poisons;
};

#endif // GRAPH_SIMULATION_H
