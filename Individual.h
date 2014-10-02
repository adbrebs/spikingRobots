#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <QVector>

class QFile;

class Settings;
class Evolution;
class h_Robot;

class Individual
{
public:
    Individual(Settings *settings, Evolution *anEvolution, h_Robot *aRobot);
    Individual(Settings *settings, Evolution* anEvolution, h_Robot* aRobot,
               QVector<qreal> aGenome, QVector<qreal> aMaxValues, QVector<qreal> aMinValues, QVector<bool> aCanChange);
    void init(Settings *settings, Evolution* anEvolution, h_Robot *aRobot);
    ~Individual();
    //Mutate();

    void assignRobot(h_Robot* aRobot) {robot = aRobot;}
    void updateRobot();
    void updatefitness();

public:
    QVector<qreal> getGenome() const {return genome;}
    const QVector<qreal>* getPGenome() const {return &genome;}
    QVector<qreal> getMaxValues() const {return maxValues;}
    const QVector<qreal>* getPMaxValues() const {return &maxValues;}
    QVector<qreal> getMinValues() const {return minValues;}
    const QVector<qreal>* getPMinValues() const {return &minValues;}
    QVector<bool> getCanChange() const {return canChange;}
    const QVector<bool>* getPCanCahnge() const {return &canChange;}

    h_Robot* getRobot() const {return robot;}
    qreal getFitness() const {return fitness;}
    void setFitness(qreal _fitness) {fitness = _fitness;}

private:
    Settings* s;

    Evolution* evolution;
    h_Robot* robot;

    QVector<qreal> genome;
    QVector<qreal> maxValues; // Max values of the genome
    QVector<qreal> minValues;
    QVector<bool> canChange; // If cross overs or mutations can take place on the parameter

    qreal fitness;
};

#endif // INDIVIDUAL_H
