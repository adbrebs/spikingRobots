#include <QFile>
#include <QTextStream>

#include "Settings.h"
#include "Evolution.h"
#include "h_Robot.h"
#include "NeuralNet.h"

#include "Individual.h"


Individual::Individual(Settings* settings, Evolution* anEvolution, h_Robot* aRobot)
{
    init(settings, anEvolution, aRobot);
    int sizeGenome = robot->getNeuralNet()->getNumParameters();

    // Especially important for the highest and lowest values of each parameters
    robot->getNeuralNet()->appendAllParameters(&genome, &maxValues, &minValues, &canChange);

    // TODO : ameliorer un peu les choses
    // Reinitialize the robots genome
    for(int i = 0; i < sizeGenome ; ++i)
        if(canChange[i])
            genome[i] = s->randNumber(minValues[i], maxValues[i]);
}


Individual::Individual(Settings *settings, Evolution* anEvolution, h_Robot *aRobot,
                       QVector<qreal> aGenome, QVector<qreal> aMaxValues, QVector<qreal> aMinValues, QVector<bool> aCanChange)
{
    init(settings, anEvolution, aRobot);
    genome = aGenome;
    maxValues = aMaxValues;
    minValues = aMinValues;
    canChange = aCanChange;
}


void Individual::init(Settings *settings, Evolution* anEvolution, h_Robot *aRobot)
{
    s = settings;
    evolution = anEvolution;
    robot = aRobot;
    fitness = 0;
}


Individual::~Individual()
{
}


void Individual::updatefitness()
{
    fitness = robot->getFitness();
}


void Individual::updateRobot()
{
    robot->updateWeights(&genome);
}

