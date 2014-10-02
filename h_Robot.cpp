#include <math.h>
#include <QFile>
#include <QDataStream>

#include "Settings.h"
#include "NeuralNet.h"
#include "h_Simulation.h"
#include "h_World.h"
#include "h_EatableItem.h"
#include "h_Robot.h"



h_Robot::h_Robot(Settings* settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Item(settings, aSimulation, aRadius),
      maxWheelSpeed(s->rob.maxWheelSpeed),
      minWheelSpeed(s->rob.minWheelSpeed),
      maximumAngle(s->rob.maximumAngle),
      lSAngle1(s->rob.lSAngle1),
      lSAngle2(s->rob.lSAngle2),
      mSAngle1(s->rob.mSAngle1),
      mSAngle2(s->rob.mSAngle2),
      rSAngle1(s->rob.rSAngle1),
      rSAngle2(s->rob.rSAngle2),
      maxSensorIntensity(s->rob.maxIntensity)
{
    // Initialize the neural net controller
    neuralNet = s->createNeuralNet(s);
    neuralNet->initializeNetwork();

    // Initialize position
    reset();
}


h_Robot::~h_Robot()
{
    delete neuralNet;
}


/*virtual*/ void h_Robot::move()
{
    // Probe its nearby environment
    QVector<qreal> sensorsIntensities = getSensors();

    // Run the controller
    neuralNet->run(sensorsIntensities, maxSensorIntensity);

    // Update position
    updatePosition();

    // Prevent the robot from leaving the world
    checkBorder(s->rob.borderMode);
}


void h_Robot::updatePosition()
{
    int leftMotorNeuron = Neuron::leftMotor;
    int rightMotorNeuron = Neuron::rightMotor;

    const Layer* recLayer = neuralNet->getMotorLayer();
    qreal leftMFR = recLayer->getOutput(leftMotorNeuron);
    qreal rightMFR = recLayer->getOutput(rightMotorNeuron);

    qreal leftWheelSpeed = minWheelSpeed + leftMFR*(maxWheelSpeed - minWheelSpeed);
    qreal rightWheelSpeed = minWheelSpeed + rightMFR*(maxWheelSpeed - minWheelSpeed);

    angle += maximumAngle*(rightWheelSpeed - leftWheelSpeed);
    qreal distance = (leftWheelSpeed + rightWheelSpeed)/2;
    x += cos(s->radian(angle)) * distance;
    y += sin(s->radian(angle)) * distance;

    emit positionChanged(this);
}


// Check that the robot doesn't leave the field by crossing a border
void h_Robot::checkBorder(int mode)
{
    qreal wWidth = simulation->getWorld()->getWidth(); // half width
    qreal wHeight = simulation->getWorld()->getHeigh(); // half height

    switch (mode)
    {

    case 0 :
    {
        if(x > wWidth)
            x = wWidth;
        else if(x < 0)
            x = 0;

        if(y > wHeight)
            y = wHeight;
        else if(y < 0)
            y = 0;
        break;
    }

    case 1 :
    {
        if(x > wWidth)
            x -= wWidth;
        else if(x < 0)
            x += wWidth;

        if(y > wHeight)
            y -= wHeight;
        else if(y < 0)
            y += wHeight;
        break;
    }
    }
}


void h_Robot::updateWeights(const QVector<qreal>* pGenome)
{
    neuralNet->updateNeuralNet(pGenome);
}


void h_Robot::updateWeightsFromFile(QString filename)
{
    QFile* pFile = new QFile(filename);
    pFile->open(QIODevice::ReadOnly);
    QDataStream in(pFile);
    neuralNet->writeIn(in);
    delete pFile;
}


/*virtual*/ void h_Robot::reset(qreal _x, qreal _y, int _angle)
{
    h_Item::reset(_x, _y, _angle);
}


void h_Robot::reset()
{
    simulation->resetRobot(this);
}









//////////////////////////////////////////
// h_Robot_1
//////////////////////////////////////////
h_Robot_1::h_Robot_1(Settings* settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot(settings, aSimulation, aRadius),
      sensorScope(s->rob.sensorScope),
      foodEaten(0),
      poisonEaten(0)
{
}


/*virtual*/ void h_Robot_1::move()
{
    h_Robot::move();

    // Eating phase
    eat();
}


void h_Robot_1::eat()
{
    // Food items
    QList<h_EatableItem*> foods = simulation->getFoods();
    eat(foods);

    QList<h_EatableItem*> poisons = simulation->getPoisons();
    eat(poisons);
}


void h_Robot_1::eat(QList<h_EatableItem*> items)
{
    foreach(h_EatableItem* item, items)
    {
        if(!item->getIsEaten() &&  dist(this, item) < radius)
        {
            if(item->isFood()) // TODO : c'est moche (faire deux fonctions eat(food) eat(poison))
            {
                addFoodEaten(1);
                simulation->foodIsEaten(item);
            }
            else if(item->isPoison())
            {
                addPoisonEaten(1);
                simulation->poisonIsEaten(item);
            }
        }
    }
}


/*virtual*/ qreal h_Robot_1::getFitness() const
{
    return s->ev.fitFoodCoeff * foodEaten
            + s->ev.fitPoisCoeff * poisonEaten;
}


/*virtual*/ void h_Robot_1::reset(qreal _x, qreal _y, int _angle)
{
    h_Robot::reset(_x, _y, _angle);
    foodEaten = 0;
    poisonEaten = 0;
}

// get food intensity between angle1 and angle2
// the closer the food item is, the higher the intensity is
qreal h_Robot_1::getIntensity(int angle1, int angle2, int angleItem, qreal distanceItem) const
{
    int angle1ref = angle1+angle;
    int angle2ref = angle2+angle;
    int rangeSensor = s->normAngle(angle1 - angle2);
    int i1 = s->normAngle(angle1ref - angleItem);
    int i2 = s->normAngle(angleItem - angle2ref);
    if(i1 < rangeSensor && i2 < rangeSensor)
        return maxSensorIntensity * (1-(distanceItem/sensorScope));
    else
        return 0;
}


/*virtual*/ QVector<qreal> h_Robot_1::getSensors() const
{
    QVector<qreal> sensors;

    if(s->rob.captorFood)
    {
        QVector<qreal> sensorsFood = getSensors_2(simulation->getFoods());
        sensors += sensorsFood;
    }
    if(s->rob.captorPoison)
    {
        QVector<qreal> sensorsPoison = getSensors_2(simulation->getPoisons());
        sensors += sensorsPoison;
    }

    return sensors;
}


// Get Sensor intensities
QVector<qreal> h_Robot_1::getSensors_1(QList<h_EatableItem*> items) const
{
    qreal leftIntensity = 0;
    qreal rightIntensity = 0;
    qreal middleIntensity = 0;

    foreach(h_EatableItem* item, items)
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            leftIntensity += getIntensity(lSAngle1, lSAngle2, angleItem, distance);
            rightIntensity += getIntensity(rSAngle1, rSAngle2, angleItem, distance);
            middleIntensity += getIntensity(mSAngle1, mSAngle2, angleItem, distance);
        }
    }

    QVector<qreal> sensors;
    sensors.append(leftIntensity);
    sensors.append(rightIntensity);
    sensors.append(middleIntensity);

    return sensors;
}



// Only closest items
QVector<qreal> h_Robot_1::getSensors_2(QList<h_EatableItem*> items) const
{
    qreal leftIntensity = 0;
    qreal rightIntensity = 0;
    qreal middleIntensity = 0;

    foreach(h_EatableItem* item, items)
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            qreal left = getIntensity(lSAngle1, lSAngle2, angleItem, distance);
            if(left > leftIntensity)
                leftIntensity = left;
            qreal right = getIntensity(rSAngle1, rSAngle2, angleItem, distance);
            if(right > rightIntensity)
                rightIntensity = right;
            qreal middle = getIntensity(mSAngle1, mSAngle2, angleItem, distance);
            if(middle > middleIntensity)
                middleIntensity = middle;
        }
    }

    QVector<qreal> sensors;
    sensors.append(leftIntensity);
    sensors.append(rightIntensity);
    sensors.append(middleIntensity);

    return sensors;
}


// Several items with maximum intensity
QVector<qreal> h_Robot_1::getSensors_3(QList<h_EatableItem*> items) const
{
    qreal leftIntensity = 0;
    qreal rightIntensity = 0;
    qreal middleIntensity = 0;

    foreach(h_EatableItem* item, items)
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            leftIntensity += getIntensity(lSAngle1, lSAngle2, angleItem, distance);
            rightIntensity += getIntensity(rSAngle1, rSAngle2, angleItem, distance);
            middleIntensity += getIntensity(mSAngle1, mSAngle2, angleItem, distance);
        }
    }

    QVector<qreal> sensors;
    sensors.append(leftIntensity);
    sensors.append(rightIntensity);
    sensors.append(middleIntensity);

    // Check that the intensity is not higher than maxIntensity
    for(int i = 0; i < sensors.size(); ++i)
    {
        if(sensors[i] > maxSensorIntensity)
        {
            for(int k = 0; k < sensors.size(); ++k)
                sensors[k] /= sensors[i];
        }
    }

    return sensors;
}


// Only THE closest item
QVector<qreal> h_Robot_1::getSensors_4(QList<h_EatableItem*> items) const
{
    qreal leftIntensity = 0;
    qreal rightIntensity = 0;
    qreal middleIntensity = 0;

    // Find the closest item
    qreal minDistance = sensorScope;
    h_EatableItem* closestItem = NULL;
    foreach(h_EatableItem* item, items)
    {
        qreal distance = dist(this, item);
        if(distance < minDistance)
        {
            minDistance = distance;
            closestItem = item;
        }
    }

    if(minDistance < sensorScope - 1)
    {
        qreal diffY = closestItem->getY() - getY();
        qreal diffX = closestItem->getX() - getX();
        int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

        qreal left = getIntensity(lSAngle1, lSAngle2, angleItem, minDistance);
        if(left > leftIntensity)
            leftIntensity = left;
        qreal right = getIntensity(rSAngle1, rSAngle2, angleItem, minDistance);
        if(right > rightIntensity)
            rightIntensity = right;
        qreal middle = getIntensity(mSAngle1, mSAngle2, angleItem, minDistance);
        if(middle > middleIntensity)
            middleIntensity = middle;
    }

    QVector<qreal> sensors;
    sensors.append(leftIntensity);
    sensors.append(rightIntensity);
    sensors.append(middleIntensity);

    return sensors;
}



//////////////////////////////////////////
// h_Robot_zigzag
//////////////////////////////////////////
h_Robot_zigzag::h_Robot_zigzag(Settings* settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot(settings, aSimulation, aRadius),
      numberRotations(0),
      pAngle(angle),
      ppAngle(angle),
      numberIterations(0)
{
    pastAngles.append(angle);
}


/*virtual*/ void h_Robot_zigzag::move()
{
    h_Robot::move();

    numberIterations++;
    pastAngles.append(angle);

    if(numberIterations > 1)
    {
        qreal diff1 = s->normAngle180(angle - pAngle);
        qreal diff2 = s->normAngle180(angle - ppAngle);
        if(diff1 * diff2 < 0)
        {
            numberRotations++;
            pastRotationAngles.append(pAngle);
        }
    }

    if(angle != pAngle)
    {
        ppAngle = pAngle;
        pAngle = angle;
    }
}


/*virtual*/ qreal h_Robot_zigzag::getFitness() const
{
    qreal buff = 1;
    for(int i =1; i < pastRotationAngles.size(); ++i)
    {
        int normAngle = s->normAngle180(pastRotationAngles[i] - pastRotationAngles[i-1]);
        buff *= pow(normAngle, 2);
    }

    return buff;
}


/*virtual*/ void h_Robot_zigzag::reset(qreal _x, qreal _y, int _angle)
{
    h_Robot::reset(_x, _y, _angle);

    pastAngles.clear();
    pastRotationAngles.clear();
    pastAngles.append(angle);
    numberRotations = 0;
    pAngle = angle;
    ppAngle= angle;
    numberIterations = 0;
}





//////////////////////////////////////////
// h_Robot_explo
//////////////////////////////////////////
h_Robot_explo::h_Robot_explo(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot_1(settings, aSimulation, aRadius)
{
}


/*virtual*/ QVector<qreal> h_Robot_explo::getSensors_2(QList<h_EatableItem*> items) const
{
    qreal middleIntensity = 0;

    foreach(h_EatableItem* item, items)
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            qreal middle = getIntensity(mSAngle1, mSAngle2, angleItem, distance);
            if(middle > middleIntensity)
                middleIntensity = middle;
        }
    }

    QVector<qreal> sensors;
    sensors.append(middleIntensity);

    return sensors;
}





//////////////////////////////////////////
// h_Robot_switch
//////////////////////////////////////////
h_Robot_switch::h_Robot_switch(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot_1(settings, aSimulation, aRadius)
{
}


/*virtual*/ qreal h_Robot_switch::getFitness() const
{
    qreal fit = 0;

    for(int i = 0; i < eatings.length()-1; ++i)
    {
        if(eatings[i] != eatings[i+1])
            fit++;
        else
            fit--;
    }

    return fit;
}

/*virtual*/ void h_Robot_switch::reset(qreal _x, qreal _y, int _angle)
{
    h_Robot_1::reset(_x, _y, _angle);

    eatings.clear();
    timeEatings.clear();
}


/*virtual*/ void h_Robot_switch::addFoodEaten(int amount)
{
    h_Robot_1::addFoodEaten(amount);
    eatings.append(1);
    timeEatings.append(simulation->getIteration());
}


/*virtual*/ void h_Robot_switch::addPoisonEaten(int amount)
{
    h_Robot_1::addPoisonEaten(amount);
    eatings.append(-1);
    timeEatings.append(simulation->getIteration());
}



//////////////////////////////////////////
// h_Robot_switch_1
//////////////////////////////////////////
h_Robot_switch_1::h_Robot_switch_1(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot_switch(settings, aSimulation, aRadius)
{
}


/*virtual*/ QVector<qreal> h_Robot_switch_1::getSensors() const
{
    QVector<qreal> sensors = h_Robot_switch::getSensors();

    if(eatings.isEmpty())
        sensors.append(s->rob.maxIntensity);
    else
    {
        if(eatings.last() == 1)
            sensors.append(0);
        else
            sensors.append(s->rob.maxIntensity);
    }


    return sensors;
}



//////////////////////////////////////////
// h_Robot_switch_2
//////////////////////////////////////////
h_Robot_switch_2::h_Robot_switch_2(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot_switch(settings, aSimulation, aRadius)
{
}


/*virtual*/ QVector<qreal> h_Robot_switch_2::getSensors() const
{
    QVector<qreal> sensors = h_Robot_switch::getSensors();

    int tt = 1;

    if(!timeEatings.isEmpty() && timeEatings.last() > simulation->getIteration() - tt)
        sensors.append(s->rob.maxIntensity);
    else
        sensors.append(0);


    return sensors;
}



//////////////////////////////////////////
// h_Robot_smell
//////////////////////////////////////////
h_Robot_smell::h_Robot_smell(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Robot_1(settings, aSimulation, aRadius)
{
}


/*virtual*/ QVector<qreal> h_Robot_smell::getSensors() const
{
    QVector<qreal> buffIntensities;
    QVector<qreal> buffItemType;

    for(int i=0; i < 6 ;++i)
    {
        buffIntensities.append(0);
        buffItemType.append(0);
    }

    foreach(h_EatableItem* item, simulation->getFoods())
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            qreal left = getIntensity(lSAngle1, lSAngle2, angleItem, distance);
            if(left > buffIntensities[0])
            {
                buffIntensities[0] = left;
                buffItemType[0] = maxSensorIntensity;
            }
            qreal right = getIntensity(rSAngle1, rSAngle2, angleItem, distance);
            if(right > buffIntensities[1])
            {
                buffIntensities[1] = right;
                buffItemType[1] = maxSensorIntensity;
            }
            qreal middle = getIntensity(mSAngle1, mSAngle2, angleItem, distance);
            if(middle > buffIntensities[2])
            {
                buffIntensities[2] = middle;
                buffItemType[2] = maxSensorIntensity;
            }
        }
    }

    foreach(h_EatableItem* item, simulation->getPoisons())
    {
        qreal distance = dist(this, item);
        if(distance < sensorScope)
        {
            qreal diffY = item->getY() - getY();
            qreal diffX = item->getX() - getX();
            int angleItem = s->degree(atan2(diffY, diffX)); // angle between the horizontal and the item item

            qreal left = getIntensity(lSAngle1, lSAngle2, angleItem, distance);
            if(left > buffIntensities[3])
            {
                buffIntensities[3] = left;
                buffItemType[3] = 0;
            }
            qreal right = getIntensity(rSAngle1, rSAngle2, angleItem, distance);
            if(right > buffIntensities[4])
            {
                buffIntensities[4] = right;
                buffItemType[4] = 0;
            }
            qreal middle = getIntensity(mSAngle1, mSAngle2, angleItem, distance);
            if(middle > buffIntensities[5])
            {
                buffIntensities[5] = middle;
                buffItemType[5] = 0;
            }
        }
    }

    qreal highest = 0;
    qreal highestType =0;
    int ihighest = 0;
    for(int k=0; k<buffIntensities.size(); ++k)
        if(highest < buffIntensities[k])
        {
            highest = buffIntensities[k];
            ihighest = k;
        }

    QVector<qreal> sensors;
    for(int i = 0; i < 4; ++i)
        sensors.append(0);

    sensors[ihighest%3] = highest;
    sensors[3] = highestType;

    return sensors;
}
