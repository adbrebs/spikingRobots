#include <QTimer>

#include "Settings.h"
#include "w_MainWindow.h"
#include "Individual.h"
#include "h_Simulation.h"
#include "h_World.h"
#include "h_Robot.h"
#include "h_EatableItem.h"
#include "graph_Scene.h"
#include "graph_Robot.h"
#include "graph_EatableItem.h"

#include "graph_Simulation.h"


graph_Simulation::graph_Simulation(Settings* settings, h_Simulation* aSimulation, w_MainWindow *aMainWindow)
    : s(settings),
      mainWindow(aMainWindow),
      timeOutInterval(s->sim.timeOutInterval),
      numIteration(0)
{
    // Update the simulation
    changeSimulation(aSimulation);

    timer = new QTimer;

    scene = new graph_Scene(s, simulation->getWorld());
    addAllItemsToScene();


    QObject::connect(timer, SIGNAL(timeout()), simulation, SLOT(run()));
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(iterate()));
}


graph_Simulation::~graph_Simulation()
{
    deleteGraph_Items();
    delete timer;
    delete scene;
}


void graph_Simulation::deleteGraph_Items()
{
    foreach (graph_Robot* graph_robot, graph_robots)
        delete graph_robot;
    foreach (graph_EatableItem* graph_food, graph_foods)
        delete graph_food;
    foreach (graph_EatableItem* graph_poison, graph_poisons)
        delete graph_poison;
}


void graph_Simulation::changeSimulation(h_Simulation* newSimulation)
{
    // If graphical items already exist, delete them
    deleteGraph_Items();

    simulation = newSimulation;

    // Creation of the food and poison items
    createGraphEatableItems();

    // Creation of the robots
    for (int i = 0; i < simulation->getNumberRobots(); ++i)
    {
        h_Robot* curRobot = simulation->getRobots()[i];
        graph_robots.append(s->createGraphRobot(s, this, curRobot));
    }
}


void graph_Simulation::createGraphEatableItems()
{
    // Creation of the food items
    for (int i = 0; i < simulation->getNumberFoods(); ++i)
    {
        h_EatableItem* curFood = simulation->getFoods()[i];
        graph_foods.append(new graph_Food(s, this, curFood));
    }

    // Creation of the poison items
    for (int i = 0; i < simulation->getNumberPoisons(); ++i)
    {
        h_EatableItem* curPoison = simulation->getPoisons()[i];
        graph_poisons.append(new graph_Poison(s, this, curPoison));
    }
}


void graph_Simulation::clearAllItems()
{
    // Clear food items
    clearItems(&graph_foods);

    // Clear Poison items
    clearItems(&graph_poisons);
}


void graph_Simulation::clearItems(QList<graph_EatableItem *>* pListItems)
{
    int numberItems = pListItems->size();
    for(int i = 0; i < numberItems ; ++i)
    {
        graph_EatableItem* curItem = pListItems->last();
        scene->removeItem(curItem);
        simulation->removeItem(curItem->getItem());

        pListItems->removeLast();
        delete curItem;
    }
}


void graph_Simulation::resetAllItems()
{
    foreach (graph_Robot* graph_robot, graph_robots)
        resetGraphRobot(graph_robot);

    foreach (graph_EatableItem* graph_food, graph_foods)
        resetGraphFood(graph_food);

    foreach (graph_EatableItem* graph_poison, graph_poisons)
        resetGraphPoison(graph_poison);
}


void graph_Simulation::resetGraphRobot(graph_Robot *graph_robot)
{
    simulation->resetRobot(graph_robot->getRobot());
}


void graph_Simulation::resetGraphFood(graph_EatableItem* graph_food)
{
    simulation->resetFood(graph_food->getItem());
}


void graph_Simulation::resetGraphPoison(graph_EatableItem* graph_poison)
{
    simulation->resetFood(graph_poison->getItem());
}


void graph_Simulation::addAllItemsToScene()
{
    foreach (graph_Robot* graph_robot, graph_robots)
        scene->addItem(graph_robot);

    addEatableItemsToScene();
}


void graph_Simulation::addEatableItemsToScene()
{
    foreach (graph_EatableItem* graph_food, graph_foods)
        scene->addItem(graph_food);

    foreach (graph_EatableItem* graph_poison, graph_poisons)
        scene->addItem(graph_poison);
}


void graph_Simulation::startSim()
{
    timer->start(timeOutInterval);
}


void graph_Simulation::pauseSim()
{
    timer->stop();
}


void graph_Simulation::resumeSim()
{
    timer->start(timeOutInterval);
}


/*slot*/ void graph_Simulation::iterate()
{
    numIteration++;
    emit sendIteration(numIteration);
}


void graph_Simulation::setTimeOutInterval(qreal time)
{
    timeOutInterval = time;
    timer->setInterval(timeOutInterval);
}


qreal graph_Simulation::getTimeOutInterval() const
{
    //return timer->interval();
    return timeOutInterval;
}


void graph_Simulation::removeItem(graph_EatableItem* graph_eatableItem)
{
    simulation->removeItem(graph_eatableItem->getItem());

    if(graph_eatableItem->isGraphFood())
        graph_foods.removeOne(graph_eatableItem);
    else if(graph_eatableItem->isGraphPoison())
        graph_poisons.removeOne(graph_eatableItem);

    delete graph_eatableItem;
}


void graph_Simulation::reInitializeAllItems()
{
    // Remove all graphical items
    clearAllItems();

    // Remove then re-create all items
    simulation->reInitAllEatableItems();

    // Recreate corresponding graphical items
    createGraphEatableItems();

    // Add new graphical items to scene
    addEatableItemsToScene();
}


void graph_Simulation::reIniRobotsPos()
{
    foreach(graph_Robot* graph_robot, graph_robots)
        resetGraphRobot(graph_robot);
}


void graph_Simulation::addFood(QPointF pt)
{
    // Conversion (the basic vectors and angle norms are different)
    QPointF npt = convertCoordinates(pt);

    h_EatableItem* food = simulation->addFood(&npt);
    graph_foods.append(new graph_Food(s, this, food));
    scene->addItem(graph_foods.last());
}


void graph_Simulation::addPoison(QPointF pt)
{
    // Conversion (the basic vectors and angle norms are different)
    QPointF npt = convertCoordinates(pt);

    h_EatableItem* poison = simulation->addPoison(&npt);
    graph_poisons.append(new graph_Poison(s, this, poison));
    scene->addItem(graph_poisons.last());
}


void graph_Simulation::addGraphicsItem(QGraphicsItem* item)
{
    scene->addItem(item);
}


void graph_Simulation::removeGraphicsItem(QGraphicsItem* item)
{
    scene->removeItem(item);
}


QPointF graph_Simulation::convertCoordinates(QPointF pt)
{
    int wWidth = simulation->getWorld()->getWidth();
    int wHeigh = simulation->getWorld()->getHeigh();

    return QPointF(pt.x() + wWidth/2,
                   wHeigh/2 - pt.y());
}


void graph_Simulation::changeRobotsPathLengths(int newPath)
{
    foreach (graph_Robot* graph_robot, graph_robots)
        graph_robot->setNumberLastPositions(newPath);
}
