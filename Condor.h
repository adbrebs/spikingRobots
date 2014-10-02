#ifndef CONDOR_H
#define CONDOR_H

#include <QVector>

class Settings;
class h_Simulation;
class Evolution;

class Condor
{
public:
    Condor(Settings* settings);
    ~Condor();

    // Experiments
    void launchTest();

private:
    // Experiments
    void testLesion();
    void testEvolution();

    // Lesion
    QVector< QVector<qreal> > lesion(h_Simulation *simulation, h_Robot *robot, QVector<QVector<qreal> > *vector, int numMean);

    // Evolution
    void launchEvolution();
    void saveResults();

private:
    Settings* s;

    h_Simulation* simEv;
    Evolution* evolution;
};

#endif // CONDOR_H
