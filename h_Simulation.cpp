#include <math.h>
#include <QPointF>

#include "Settings.h"
#include "NeuralNet.h"
#include "h_World.h"
#include "h_Robot.h"
#include "h_EatableItem.h"

#include "h_Simulation.h"


const int _foodRadius = 3;
const int _poisonRadius = 3;
const int _robotRadius = 10;


//////////////////////////////////////////
// h_Simulation
//////////////////////////////////////////
h_Simulation::h_Simulation(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth)
    : s(settings),
      iteration(0),
      numberRobots(aNumberRobot),
      numberFoods(aNumberFoods),
      numberPoisons(aNumberPoisons)
{
    world = new h_World(this, worldWidth, worldWidth);
}


void h_Simulation::initSimulation()
{
    // Create robots
    for (int i = 0; i < numberRobots; ++i)
        robots.append(s->createRobot(s, this, _robotRadius));

    // Create Eatable items
    initEatableItems();
}


void h_Simulation::initEatableItems()
{
    resetParameters();

    for (int i = 0; i < numberFoods; ++i)
        foods.append(new h_Food(s, this, _foodRadius));

    for (int i = 0; i < numberPoisons; ++i)
        poisons.append(new h_Poison(s, this, _poisonRadius));
}


h_Simulation::~h_Simulation()
{
    delete world;
    foreach(h_EatableItem* food, foods)
        delete food;
    foreach(h_EatableItem* poison, poisons)
        delete poison;
    foreach(h_Robot* robot, robots)
        delete robot;
}


/*virtual*/ void h_Simulation::run(int aNumOfIterations)
{
    for(int i = 0; i<aNumOfIterations; i++)
    {
        iteration++;
        foreach(h_Robot* robot, robots)
            robot->move();
    }
}


/*virtual*/ void h_Simulation::run()
{
    run(1);
}


void h_Simulation::resetEverything()
{
    resetParameters();
    resetFoods();
    resetPoisons();
    resetRobots();
}


void h_Simulation::resetRobots()
{
    foreach(h_Robot* robot, robots)
        resetRobot(robot);
}


/*virtual*/ void h_Simulation::resetRobot(h_Robot* robot)
{
    robot->getNeuralNet()->reInitPotentials();
}


void h_Simulation::resetFoods()
{
    foreach (h_EatableItem *food, foods)
        resetFood(food);
}


void h_Simulation::resetPoisons()
{
    foreach (h_EatableItem *poison, poisons)
        resetPoison(poison);
}


h_EatableItem* h_Simulation::addFood(QPointF* ppt)
{
    foods.append(new h_Food(s, this, _foodRadius, ppt));

    return foods.last();
}


h_EatableItem* h_Simulation::addPoison(QPointF* ppt)
{
    poisons.append(new h_Poison(s, this, _poisonRadius, ppt));

    return poisons.last();
}


void h_Simulation::removeItem(h_EatableItem* item)
{
    if(item->isFood())
        foods.removeOne(item);
    else if(item->isPoison())
        poisons.removeOne(item);

    delete item;
}


void h_Simulation::removeAllItems()
{
    // Remove Food items
    removeItems(&foods);

    // Remove Poison items
    removeItems(&poisons);
}


void h_Simulation::removeItems(QList<h_EatableItem *>* pItems)
{
    int size = pItems->size();

    for(int i = 0; i < size ; ++i)
    {
        h_EatableItem* curItem = pItems->last();
        pItems->removeLast();
        delete curItem;
    }
}


void h_Simulation::reInitAllEatableItems()
{
    removeAllItems();
    initEatableItems();
}


// Update the neural net of the first robot in the list
void h_Simulation::updateRobotFromFile(QString filename)
{
    robots.first()->updateWeightsFromFile(filename);
}



//////////////////////////////////////////
// h_SimStandard
//////////////////////////////////////////
h_SimStandard::h_SimStandard(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth)
    : h_Simulation(settings, aNumberRobot, aNumberFoods, aNumberPoisons, worldWidth)
{
}


/*virtual*/ void h_SimStandard::resetRobot(h_Robot* robot)
{
    h_Simulation::resetRobot(robot);

    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();
    qreal newAngle = s->randInteger(0, 360);

    robot->reset(wWidth/2, wHeigh/2, newAngle);
}


/*virtual*/ void h_SimStandard::resetFood(h_EatableItem *food)
{
    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();

    qreal newX = s->randNumber(0, wWidth);
    qreal newY = s->randNumber(0, wHeigh);
    qreal newAngle = s->randInteger(0, 360);

    food->reset(newX, newY, newAngle);
}


/*virtual*/ void h_SimStandard::resetPoison(h_EatableItem *poison)
{
    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();

    qreal newX = s->randNumber(0, wWidth);
    qreal newY = s->randNumber(0, wHeigh);
    qreal newAngle = s->randInteger(0, 360);

    poison->reset(newX, newY, newAngle);
}



//////////////////////////////////////////
// h_SimExplo
//////////////////////////////////////////
h_SimExplo::h_SimExplo(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth)
    : h_Simulation(settings, aNumberRobot, aNumberFoods, aNumberPoisons, worldWidth),
      numBunches(s->sim.numBunches),
      radiusBunch(s->sim.radiusBunch)
{
    qreal width = world->getWidth();
    qreal heigh = world->getHeigh();

    for(int i=0; i < numBunches; ++i)
        bunches.append(QPointF(s->randNumber(radiusBunch, width-radiusBunch),
                               s->randNumber(radiusBunch, heigh-radiusBunch)));
}


/*virtual*/ void h_SimExplo::resetParameters()
{
    qreal width = world->getWidth();
    qreal heigh = world->getHeigh();

    for(int i=0; i < numBunches; ++i)
    {
        bunches[i].setX(s->randNumber(radiusBunch, width-radiusBunch));
        bunches[i].setY(s->randNumber(radiusBunch, heigh-radiusBunch));
    }
}


/*virtual*/ void h_SimExplo::resetRobot(h_Robot* robot)
{
    h_Simulation::resetRobot(robot);

    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();
    qreal newAngle = s->randInteger(0, 360);

    robot->reset(wWidth/2, wHeigh/2, newAngle);
}


/*virtual*/ void h_SimExplo::resetFood(h_EatableItem *food)
{
    int bunch = s->randInteger(0, numBunches);
    QPointF centerBunch = bunches[bunch];

    qreal newX = s->randNumber(centerBunch.x()-radiusBunch, centerBunch.x()+radiusBunch);
    qreal newY = s->randNumber(centerBunch.y()-radiusBunch, centerBunch.y()+radiusBunch);
    qreal newAngle = s->randInteger(0, 360);

    food->reset(newX, newY, newAngle);
}


/*virtual*/ void h_SimExplo::foodIsEaten(h_EatableItem *food)
{
    food->reset(-1000, -1000, 0);
}


/*virtual*/ void h_SimExplo::resetPoison(h_EatableItem *poison)
{
    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();

    qreal newX = s->randNumber(0, wWidth);
    qreal newY = s->randNumber(0, wHeigh);
    qreal newAngle = s->randInteger(0, 360);

    poison->reset(newX, newY, newAngle);
}


/*virtual*/ void h_SimExplo::poisonIsEaten(h_EatableItem *poison)
{
    poison->reset(-1000, -1000, 0);
}




//////////////////////////////////////////
// h_SimMemo
//////////////////////////////////////////
h_SimMemo::h_SimMemo(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth)
    : h_Simulation(settings, aNumberRobot, aNumberFoods, aNumberPoisons, worldWidth),
      counter(0)
{
}


/*virtual*/ void h_SimMemo::resetParameters()
{
    counter = 0;
}


/*virtual*/ void h_SimMemo::resetRobot(h_Robot* robot)
{
    h_Simulation::resetRobot(robot);

    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();
    qreal newAngle = 90;

    robot->reset(wWidth/2, wHeigh/2, newAngle);
}


/*virtual*/ void h_SimMemo::resetFood(h_EatableItem *food)
{
    qreal wWidth = world->getWidth();
    qreal wHeigh = world->getHeigh();

    qreal step = sqrt(counter+3);
    qreal newX = wWidth/2 + 8*exp(step) * cos(step);
    qreal newY = wHeigh/2 + 8*exp(step) * sin(step);

    qreal newAngle = s->randInteger(0, 360);

    food->reset(newX, newY, newAngle);
}


/*virtual*/ void h_SimMemo::foodIsEaten(h_EatableItem *food)
{
    resetFood(food);
    counter++;
}


/*virtual*/ void h_SimMemo::resetPoison(h_EatableItem *poison)
{
    qreal newAngle = s->randInteger(0, 360);

    poison->reset(100, 100, newAngle);
}


/*virtual*/ void h_SimMemo::poisonIsEaten(h_EatableItem *poison)
{
    resetPoison(poison);
}
