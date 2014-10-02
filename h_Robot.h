#ifndef H_ROBOT_H
#define H_ROBOT_H

#include <QVector>

#include "h_Item.h"

class NeuralNet;
class h_EatableItem;

class h_Robot : public h_Item
{
public:
    h_Robot(Settings *settings, h_Simulation *aSimulation, qreal aRadius);
    virtual ~h_Robot();

    virtual void move();

    void updateWeights(const QVector<qreal>* pGenome);
    void updateWeightsFromFile(QString filename);

    NeuralNet* getNeuralNet() const {return neuralNet;}

    /*virtual*/ bool isRobot() const {return true;}
    /*virtual*/ void reset(qreal _x, qreal _y, int _angle);
    void reset();

    virtual QVector<qreal> getSensors() const = 0;
    virtual qreal getFitness() const = 0;

    int gLSAngle1() {return lSAngle1;}
    int gLSAngle2() {return lSAngle2;}
    int gMSAngle1() {return mSAngle1;}
    int gMSAngle2() {return mSAngle2;}
    int gRSAngle1() {return rSAngle1;}
    int gRSAngle2() {return rSAngle2;}
    int gMaxSensorIntensity() {return maxSensorIntensity;}

protected:
    void updatePosition();
    void checkBorder(int mode);


protected:
    qreal maxWheelSpeed;
    qreal minWheelSpeed;
    int maximumAngle; // Maximum angle that the robot can rotate at each iteration

    NeuralNet* neuralNet;

    // angles of the sensors
    int lSAngle1;
    int lSAngle2;
    int mSAngle1;
    int mSAngle2;
    int rSAngle1;
    int rSAngle2;
    int maxSensorIntensity; // Max value returned by the captors
};




class h_Robot_1 : public h_Robot
{
public:
    h_Robot_1(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

    /*virtual*/ void move();
    /*virtual*/ QVector<qreal> getSensors() const;
    /*virtual*/ qreal getFitness() const;
    /*virtual*/ void reset(qreal _x, qreal _y, int _angle);

    qreal getSensorScope() const {return sensorScope;}

    int getFoodEaten() const {return foodEaten;}
    int getPoisonEaten() const {return poisonEaten;}

protected:
    virtual qreal getIntensity(int angle1, int angle2, int angleItem, qreal distanceItem) const;
    virtual QVector<qreal> getSensors_1(QList<h_EatableItem*> items) const;
    virtual QVector<qreal> getSensors_2(QList<h_EatableItem*> items) const;
    virtual QVector<qreal> getSensors_3(QList<h_EatableItem*> items) const;
    virtual QVector<qreal> getSensors_4(QList<h_EatableItem*> items) const;

    void eat();
    void eat(QList<h_EatableItem*> items);

    void setFoodEaten(int amount) {foodEaten = amount;}
    virtual void addFoodEaten(int amount) {foodEaten += amount;}

    void setPoisonEaten(int amount) {poisonEaten = amount;}
    virtual void addPoisonEaten(int amount) {poisonEaten += amount;}

protected:
    qreal sensorScope;

    int foodEaten;
    int poisonEaten;
};




class h_Robot_zigzag : public h_Robot
{
public:
    h_Robot_zigzag(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

    /*virtual*/ void move();
    /*virtual*/ QVector<qreal> getSensors() const {QVector<qreal> sensorsInputs;return sensorsInputs;}
    /*virtual*/ qreal getFitness() const;
    /*virtual*/ void reset(qreal _x, qreal _y, int _angle);

private:
    int numberRotations;

    int pAngle;
    int ppAngle;

    QVector< int > pastRotationAngles;
    QVector< int > pastAngles;
    int numberIterations;
};




class h_Robot_explo : public h_Robot_1
{
public:
    h_Robot_explo(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

protected:
    /*virtual*/ QVector<qreal> getSensors_2(QList<h_EatableItem*> items) const;
};



class h_Robot_switch : public h_Robot_1
{
public:
    h_Robot_switch(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

    /*virtual*/ qreal getFitness() const;

    /*virtual*/ void reset(qreal _x, qreal _y, int _angle);

protected:
    /*virtual*/ void addFoodEaten(int amount);
    /*virtual*/ void addPoisonEaten(int amount);

protected:
    QList<int> eatings;
    QList<int> timeEatings;

};



class h_Robot_switch_1 : public h_Robot_switch
{
public:
    h_Robot_switch_1(Settings *settings, h_Simulation *aSimulation, qreal aRadius);
    /*virtual*/ QVector<qreal> getSensors() const;
};


class h_Robot_switch_2 : public h_Robot_switch
{
public:
    h_Robot_switch_2(Settings *settings, h_Simulation *aSimulation, qreal aRadius);
    /*virtual*/ QVector<qreal> getSensors() const;
};


class h_Robot_smell : public h_Robot_1
{
public:
    h_Robot_smell(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

    /*virtual*/ QVector<qreal> getSensors() const;


};



#endif // H_ROBOT_H
