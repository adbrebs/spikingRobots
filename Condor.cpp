#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QTextStream>

#include "Settings.h"
#include "Evolution.h"
#include "h_Simulation.h"
#include "h_Robot.h"
#include "NeuralNet.h"

#include "Condor.h"

Condor::Condor(Settings* settings)
    : s(settings)
{
    // Set a unique number to define uniquely the application process
    s->setSerialNumber(qrand());

    simEv = s->createSim(s, 1, s->ev.numFoods,
                         s->ev.numPoisons, s->ev.worldWidth);

    evolution = new Evolution(s, simEv);
}


Condor::~Condor()
{
    delete simEv;
    delete evolution;
}


void Condor::launchTest()
{
    QString experiment = s->cond.experiment;

    if(experiment == "evolution")
        testEvolution();
    else if(experiment == "lesion")
        testLesion();
}


void Condor::launchEvolution()
{
    evolution->evolve(s->cond.numGenerations);
}


void Condor::saveResults()
{
    // Setup the folders where results will be saved
    if(!QDir("./results/").exists())
        QDir().mkdir("./results/");
    QString folderName = "./results/" + QString::number(s->cond.serialNumber) + "/";
    if(!QDir(folderName).exists())
        QDir().mkdir(folderName);

    // Save results and config file
    evolution->SaveEvolutionOnFile();
    evolution->SaveBestNetworkOnFile();
    QSettings qSettings2(folderName + "config.ini", QSettings::IniFormat);
    s->writeSettingsToFile(&qSettings2);
}


void Condor::testLesion()
{
    int numMean = s->cond.numSimulations;

    h_Simulation* simulation = s->createSim(s, 1, s->sim.numFoods, s->sim.numPoisons,
                                            s->sim.worldWdith);

    QString testFile = "./results/test.net";
    simulation->updateRobotFromFile(testFile);

    h_Robot* robot = simulation->getRobots().first();
    NeuralNet* net = robot->getNeuralNet();

    QList< QVector< QVector<qreal> > > results;


    foreach(QVector< QVector<qreal> >* weight, net->getWeights())
        results.append(lesion(simulation, robot, weight, numMean));



    // Create/Open data file
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("'test_'yyyy_MM_dd_hh_mm'_'");

    // Write in the file
    for(int u=0; u < results.size(); ++u)
    {
        QString filename = "./results/" + date + QString::number(s->cond.serialNumber)
                + "_200_simulations_" + QString::number(u+1) + ".txt";

        QVector< QVector<qreal> > result = results[u];

        s->saveMatrixInFile(filename, &result);
    }



    // Write the weights files
    for(int u=0; u < net->getWeights().size(); ++u)
    {
        QString filename = "./results/" + date + QString::number(s->cond.serialNumber)
                + "_200_simulations_" + QString::number(u+1) + "weights.txt";

        s->saveMatrixInFile(filename, net->getWeights().at(u));
    }
}


QVector< QVector<qreal> > Condor::lesion(h_Simulation* simulation, h_Robot* robot, QVector< QVector<qreal> >* vector, int numMean)
{
    QVector< QVector<qreal> > results;

    for(int i=0; i < vector->size(); ++i)
    {
        QVector<qreal> buffResults;
        qDebug("%d", i);

        for(int k=0; k < vector->first().size(); ++k)
        {
            qreal buffWeight = (*vector)[i][k];
            (*vector)[i][k] = 0;

            buffResults.append(0);
            for(int m=0; m < numMean; ++m)
            {
                simulation->resetEverything();
                simulation->run(s->ev.lengthSim);
                buffResults.last() += robot->getFitness();
            }
            buffResults.last() /= ((qreal)numMean);

            // restore weight
            (*vector)[i][k] = buffWeight;
        }
        results.append(buffResults);
    }


    return results;
}


void Condor::testEvolution()
{
    launchEvolution();
    saveResults();
}
