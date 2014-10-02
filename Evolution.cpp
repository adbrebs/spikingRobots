#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QDataStream>
#include <QTextStream>

#include "Settings.h"
#include "Individual.h"
#include "NeuralNet.h"
#include "h_Robot.h"
#include "h_EatableItem.h"
#include "h_Simulation.h"

#include "Evolution.h"


Evolution::Evolution(Settings* settings, h_Simulation *aSim)
    : s(settings),
      evSim(new EvoSim(aSim)),
      numberIndividuals(s->ev.popSize),
      numberOfIterations(s->ev.lengthSim),
      crossOverRate(s->ev.crossOverRate),
      mutationAmpli(s->ev.mutationAmpli),
      mutationRate(s->ev.mutationRate),
      numberGenerations(0)
{
    // Init data history (save fitness)
    for(int i = 0; i<3; ++i)
        history.append(QVector<QPointF>());

    // Create individuals
    for(int i=0; i < numberIndividuals; ++i)
        individuals.append(new Individual(s, this, evSim->getRobot()));
}


Evolution::~Evolution()
{
    foreach(Individual* individual, individuals)
        delete individual;
    
    delete evSim;
}


bool lessFitThan(const Individual* ind1, const Individual* ind2)
{
    return ind1->getFitness() > ind2->getFitness();
}


void Evolution::evolve(int numSim)
{
    for (int i=0; i < numSim; ++i)
    {
        // Print the number of the current simulation
        qDebug("%d", i);
        
        // Evaluate the result of the simulation and update individuals fitness
        // Sort individuals according to their fitness
        recalculateFitness();
        
        // Selection and reproduction of the best individuals (Update their genoms)
        updateIndividuals();
        
        emit(nextGeneration(i, numSim, ++numberGenerations));
        QCoreApplication::processEvents(); // In order to refresh the progress bar
    }
    
    // Final run with longer simulation length
    orderEvolvedPopulation(s->ev.lengthLastSim);
}


void Evolution::updateIndividuals()
{
    int numberSelected = (int) (crossOverRate*numberIndividuals);
    
    // Best individuals survive and are selected for the next generation
    for(int i=0; i < numberIndividuals-numberSelected; ++i)
        nextIndividuals.append(individuals[i]);
    
    // Sum of the fitness values of all the individuals
    qreal sumFit = sumFitness();
    
    // Minimum fitness;
    qreal minFit = individuals.last()->getFitness();
    
    for(int i=0; i<numberSelected; ++i)
    {
        // indices of parents to select for reproduction
        int i1 = select(sumFit, minFit);
        int i2 = select(sumFit, minFit);
        
        giveBirth(individuals[i1], individuals[i2]);
    }
    
    // Update generation
    for(int i=0; i < numberSelected; ++i)
        delete individuals[numberIndividuals-1-i];
    individuals = nextIndividuals;
    nextIndividuals.clear();
}


// This work even with negativ fitnesses
int Evolution::select(qreal sumFitness, qreal minFitness) const
{
    qreal k = s->randNumber(0,sumFitness - numberIndividuals*minFitness);
    
    int individualIndex = 0;
    qreal buffFitness = 0;
    while(individualIndex < numberIndividuals)
    {
        buffFitness += individuals[individualIndex]->getFitness() - minFitness;
        if(k <= buffFitness)
            return individualIndex;
        else
            individualIndex++;
    }
    qDebug("Error");
    return 0;
}


void Evolution::giveBirth(const Individual *parent1, const Individual *parent2)
{
    QString crossOverType = s->ev.crossOverType;

    QVector<qreal> childGenome;
    
    QVector<qreal> genome1 = parent1->getGenome();
    QVector<qreal> genome2 = parent2->getGenome();
    QVector<qreal> maxValues = parent1->getMaxValues();
    QVector<qreal> minValues = parent1->getMinValues();
    QVector<bool> canChange = parent1->getCanChange();

    // CROSS-OVER
    if(crossOverType == "barycentric")
    {
        qreal ret = s->randNumber(-0.5, 1.5);
        
        for(int i=0; i<genome1.size(); ++i)
        {
            // If the gene is set not to change
            if(!canChange[i])
            {
                childGenome.append(genome1[i]);
                continue;
            }
            childGenome.append(ret*genome1[i] + (1-ret)*genome2[i]);
        }
    }
    else if(crossOverType == "onePoint")
    {
        qreal pos = s->randInteger(0, genome1.size()-1);

        for(int i=0; i<genome1.size(); ++i)
        {
            // If the gene is set not to change
            if(!canChange[i])
            {
                childGenome.append(genome1[i]);
                continue;
            }
            if(pos>i)
                childGenome.append(genome1[i]);
            else
                childGenome.append(genome2[i]);
        }
    }


    // MUTATION
    for(int i=0; i<genome1.size(); ++i)
    {
        if(!canChange[i])
            continue;

        qreal mutRet = s->randNumber(0,1); // Gives a value between 0.0 and 1.0
        if(mutRet < mutationRate)
        {
            qreal mutDiff = s->randNumber(mutationAmpli * minValues[i], mutationAmpli * maxValues[i]);
            childGenome[i] += mutDiff;
        }

        // check that the gene still stay between -1 and 1
        if(childGenome[i] > maxValues[i])
            childGenome[i] = maxValues[i];
        else if(childGenome[i] < minValues[i])
            childGenome[i] = minValues[i];
    }

    // Create and add the child
    Individual* child = new Individual(s, this, evSim->getRobot(),
                                       childGenome, maxValues, minValues, canChange);
    nextIndividuals.append(child);
}


void Evolution::recalculateFitness()
{
    recalculateFitness(numberOfIterations, true);

    // Save information about the generation
    saveHistory();

    emit newFitness(numberGenerations, &history);
}


void Evolution::recalculateFitness(int aNumberOfIterations, bool orderAtEnd)
{
    // Do a simulation for each robot alone in its environment
    foreach(Individual *individual, individuals)
    {
        individual->updateRobot();
        evSim->resetItems();

        // Run one simulation
        evSim->getSim()->run(aNumberOfIterations);

        individual->updatefitness();
    }

    // best robots are at the beginning of the list
    if(orderAtEnd)
        qSort(individuals.begin(), individuals.end(), lessFitThan);
}


// Used after the end of the evolution to update the robots of another
// simulation according to the best ones returned by the evolution
void Evolution::updateSimulation(h_Simulation* aSimulation) const
{
    for (int i = 0; i < aSimulation->getNumberRobots(); ++i)
    {
        h_Robot* curRobot = aSimulation->getRobots()[i];
        Individual* curIndividual = individuals[i]; // Best individual
        // Update robots' brains according to the results of the evolution
        updateRobot(curRobot, curIndividual);
    }
}


// Update a robot's neural network according to the genome of an individual
void Evolution::updateRobot(h_Robot* aRobot, Individual* individual) const
{
    aRobot->updateWeights(individual->getPGenome());
}


// This is done to avoid statistical problems
void Evolution::orderEvolvedPopulation(int aNumberOfIterations)
{
    int avg=s->ev.avgLastSim;

    QVector<qreal> meanFitness;
    for(int i=0; i<numberIndividuals; ++i)
        meanFitness.append(0);

    for(int i=0; i<avg; ++i)
    {
        recalculateFitness(s->ev.lengthLastSim, false);
        for(int k=0; k<numberIndividuals; ++k)
            meanFitness[k]+=individuals[k]->getFitness();
    }

    for(int i=0; i<numberIndividuals; ++i)
        individuals[i]->setFitness(meanFitness[i]/(qreal)avg);

    qSort(individuals.begin(), individuals.end(), lessFitThan);
}


void Evolution::SaveBestNetworkOnFile()
{
    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("'evolution_'yyyy_MM_dd_hh_mm'_'");

    QString folderName = "./results/" + QString::number(s->cond.serialNumber) + "/";
    QString filename = folderName + date + QString::number(s->cond.serialNumber) + ".net";

    SaveBestNetworkOnFile(filename);
}


void Evolution::SaveBestNetworkOnFile(QString filename)
{
    QFile* pFile = new QFile(filename);
    pFile->open(QIODevice::WriteOnly);
    QDataStream out(pFile);
    individuals.first()->updateRobot(); // Set the network corresponding to the genome
    NeuralNet* curNet = individuals.first()->getRobot()->getNeuralNet();
    curNet->writeOut(out);
    delete pFile;
}


void Evolution::SaveEvolutionOnFile()
{
    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString date = dateTime.toString("'evolution_'yyyy_MM_dd_hh_mm'_'");

    QString folderName = "./results/" + QString::number(s->cond.serialNumber) + "/";
    QString filename = folderName + date + QString::number(s->cond.serialNumber) + ".ev";

    SaveEvolutionOnFile(filename);
}


void Evolution::SaveEvolutionOnFile(QString filename)
{
    QFile* pFile = new QFile(filename);
    pFile->open(QIODevice::WriteOnly);

    QTextStream out(pFile);
    out << filename << endl;
    out << s->cond.serialNumber << endl;
    out << "Population size : " << numberIndividuals << endl;
    out << "Number of iterations : " << numberOfIterations << endl;
    out << "CrossOver rate : " << crossOverRate << endl;
    out << "Mutation rate : " << mutationRate << endl;
    out << "Number of food items : " << s->ev.numFoods << endl;
    out << "Number of poison items : " << s->ev.numPoisons << endl;
    out << "END_INFO" << endl << endl;

    for(int i = 0; i<history[0].size(); ++i)
        out << history[0][i].x() << " "
            << history[0][i].y() << " "
            << history[1][i].y() << " "
            << history[2][i].y() << endl;

    delete pFile;
}


qreal Evolution::sumFitness() const
{
    return sumFitness(numberIndividuals);
}


// Sum the fitness values of the 'pop' best individuals
qreal Evolution::sumFitness(int pop) const
{
    qreal sumFit = 0;
    for(int i = 0; i < pop; ++i)
        sumFit += individuals[i]->getFitness();
    return sumFit;
}


void Evolution::saveHistory()
{
    // Mean of fitness
    qreal sumFit = sumFitness(numberIndividuals/2);

    // Average for one individual
    sumFit = sumFit/(numberIndividuals/2);

    history[0].append(QPointF(numberGenerations, sumFit));
}





//////////////////////////////////////////
// EvoSim
//////////////////////////////////////////
EvoSim::EvoSim(h_Simulation* aSimulation)
    : simulation(aSimulation),
      robot(simulation->getRobots().first())
{
}


void EvoSim::resetRobot()
{
    simulation->resetRobot(robot);
}


void EvoSim::resetItems()
{
    simulation->resetEverything();
}
