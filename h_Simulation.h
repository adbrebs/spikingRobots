#ifndef H_SIMULATION_H
#define H_SIMULATION_H

#include <QList>
#include <QVector>
#include <QPointF>

class Settings;
class h_World;
class h_Robot;
class h_EatableItem;


#include <QObject>

/*
  This class manages the two worlds with the robots, food items and poison items
*/
class h_Simulation : public QObject
{
    Q_OBJECT

public:
    h_Simulation(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth);
    virtual ~h_Simulation();
    void initSimulation();

    h_World* getWorld() const {return world;}
    int getIteration() const {return iteration;}

    void resetEverything();

    virtual void resetParameters() {}

    void resetRobots();
    virtual void resetRobot(h_Robot* robot);
    QList<h_Robot *> getRobots() const {return robots;}
    int getNumberRobots() const {return numberRobots;}

    void resetFoods();
    virtual void resetFood(h_EatableItem *food) = 0;
    virtual void foodIsEaten(h_EatableItem *food) = 0;
    QList<h_EatableItem *> getFoods() const {return foods;}
    int getNumberFoods() const {return numberFoods;}

    void resetPoisons();
    virtual void resetPoison(h_EatableItem *poison) = 0;
    virtual void poisonIsEaten(h_EatableItem *poison) = 0;
    QList<h_EatableItem *> getPoisons() const {return poisons;}
    int getNumberPoisons() const {return numberPoisons;}

    void removeItem(h_EatableItem* item);
    void removeAllItems();

    h_EatableItem* addFood(QPointF* ppt);
    h_EatableItem* addPoison(QPointF* ppt);

    void reInitAllEatableItems();

    void updateRobotFromFile(QString filename);

public slots:
    virtual void run();
    virtual void run(int aNumOfIterations); // A run of numberOfIterations iterations

protected:
    void initEatableItems();
    void removeItems(QList<h_EatableItem *> *pItems);

protected:
    Settings* s;

    int iteration;

    h_World* world;
    QList<h_Robot *> robots;
    QList<h_EatableItem *> foods;
    QList<h_EatableItem *> poisons;
    QList<h_EatableItem *> poisonWall;

    int numberRobots;
    int numberFoods;
    int numberPoisons;
};




class h_SimStandard : public h_Simulation
{
public:
    h_SimStandard(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth);

    /*virtual*/ void resetRobot(h_Robot* robot);
    /*virtual*/ void resetFood(h_EatableItem *food);
    /*virtual*/ void foodIsEaten(h_EatableItem *food) {resetFood(food);}
    /*virtual*/ void resetPoison(h_EatableItem *poison);
    /*virtual*/ void poisonIsEaten(h_EatableItem *poison) {resetPoison(poison);}
};





class h_SimExplo : public h_Simulation
{
public:
    h_SimExplo(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth);

    /*virtual*/ void resetParameters();

    /*virtual*/ void resetRobot(h_Robot* robot);
    /*virtual*/ void resetFood(h_EatableItem *food);
    /*virtual*/ void foodIsEaten(h_EatableItem *food);
    /*virtual*/ void resetPoison(h_EatableItem *poison);
    /*virtual*/ void poisonIsEaten(h_EatableItem *poison);

private:
    int numBunches;
    qreal radiusBunch;
    QVector<QPointF> bunches;
};




class h_SimMemo : public h_Simulation
{
public:
    h_SimMemo(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth);

    /*virtual*/ void resetParameters();

    /*virtual*/ void resetRobot(h_Robot* robot);
    /*virtual*/ void resetFood(h_EatableItem *food);
    /*virtual*/ void foodIsEaten(h_EatableItem *food);
    /*virtual*/ void resetPoison(h_EatableItem *poison);
    /*virtual*/ void poisonIsEaten(h_EatableItem *poison);

private:
    int counter;
};



#endif // H_SIMULATION_H
