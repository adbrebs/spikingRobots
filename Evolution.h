#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <QObject>
#include <QPointF>
#include <QVector>

class QFile;
class Settings;
class Individual;
class h_Robot;
class EvoSim;
class h_Simulation;


class Evolution : public QObject
{
    Q_OBJECT
public:
    Evolution(Settings* settings, h_Simulation *aSim);
    ~Evolution();

    void evolve(int numSim);

    void updateSimulation(h_Simulation* aSimulation) const;
    void updateRobot(h_Robot* aRobot, Individual* individual) const;

    QList<Individual *> getIndividuals() const {return individuals;}

    void SaveBestNetworkOnFile(); // Condor (randomly generated filenames
    void SaveBestNetworkOnFile(QString filename);
    void SaveEvolutionOnFile(); // Condor (randomly generated filenames
    void SaveEvolutionOnFile(QString filename);

private:
    void recalculateFitness();
    void recalculateFitness(int aNumberOfIterations, bool orderAtEnd);
    void updateIndividuals();
    int select(qreal sumFitness, qreal minFitness) const;
    void giveBirth(const Individual* parent1, const Individual* parent2);
    void orderEvolvedPopulation(int aNumberOfIterations);

    qreal sumFitness() const; // Sum of all the individuals' fitness values
    qreal sumFitness(int pop) const;
    void sumFoodPoison(int pop, qreal *sumFood, qreal *sumPoison) const; // Sum of all the individuals' eaten foods and poisons

    void saveHistory();

signals:
    void nextGeneration(int sim, int numSim, int numGeneration);
    void newFitness(int numGeneration, const QVector< QVector<QPointF> >* history);

private:
    Settings* s;

    EvoSim* evSim;
    QList<Individual *> individuals;
    QList<Individual *> nextIndividuals;

    int numberIndividuals;
    int numberOfIterations; // Number of robots' steps in one simulation
    qreal crossOverRate;
    qreal mutationAmpli;
    qreal mutationRate;

    int numberGenerations;

    QVector< QVector<QPointF> > history; // Save the fitness of each iteration
};



class EvoSim
{
public:
    EvoSim(h_Simulation *aSimulation);

    void setRobot(h_Robot* aRobot) {robot = aRobot;}
    h_Robot* getRobot() {return robot;}
    void resetRobot();
    void resetItems();

    h_Simulation* getSim() {return simulation;}

private:
    h_Simulation* simulation;
    h_Robot* robot;
};

#endif // EVOLUTION_H
