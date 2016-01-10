#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <QString>
#include <QList>

class QSettings;
class h_Robot;
class graph_Robot;
class graph_Simulation;
class NeuralNet;
class Layer;
class Layer_Spiking;
class h_Simulation;


class Settings
{
public:
    Settings(QSettings* pQSettings);

    h_Simulation* createSim(Settings* settings, int aNumberRobot, int aNumberFoods, int aNumberPoisons, qreal worldWidth) const;
    NeuralNet* createNeuralNet(Settings* settings) const;
    Layer_Spiking* createLayer(Settings* settings, int aNumNeurons) const;
    h_Robot* createRobot(Settings* settings, h_Simulation *aSimulation, qreal aRadius);
    graph_Robot* createGraphRobot(Settings* settings, graph_Simulation* aGraph_Sim, h_Robot* aRobot);

    void writeSettingsToFile(QSettings* pQSettings);
    bool saveMatrixInFile(QString filename, QVector<QVector<qreal> > *pMatrix);

    qreal randNumber(qreal min, qreal max);
    int randInteger(int min, int max);
    int normAngle(int angle); // between 0 and 360
    int normAngle180(int angle); // between -180 and 180
    qreal normAngle(qreal angle);
    qreal radian(qreal angle);
    qreal degree(qreal angle);
    int numCaptors();
    qreal sigmoid(qreal x);

    void setSerialNumber(int aNumber) {cond.serialNumber = aNumber;}


    struct EvolutionSettings
    {
        int     popSize;        // Size of the evolving population
        int     numFoods;       // Number of food items for the evolution simulations
        int     numPoisons;     // Number of poison items for the evolution simulations
        int     numWallPoisons;
        int     lengthSim;      // Number of times the robot advance in one cycle
        int     lengthLastSim;  // Number of times the robot advance in the last cycle of the evolution
        int     avgLastSim;     // Number of times we average some lengthLastSim-length experiments
        QString crossOverType;  // Barycentric or one point
        qreal   crossOverRate;
        qreal   mutationRate;
        qreal   mutationAmpli;
        qreal   worldWidth;

        // Fitness
        int     fitFoodCoeff;   // For the fitness function, measures the importance of food compared to poison
        int     fitPoisCoeff;
    } ev;

    struct GraphSimulationSettings
    {
        int     numRobots;      // must be less than ev.popSize
        int     numFoods;
        int     numPoisons;
        int     numWallPoisons;
        qreal   worldWdith;

        int     numBunches;     // Number of bunches of food
        qreal   radiusBunch;    // food is scattered in this radius circle

        qreal timeOutInterval;  // Time between two simulations
    } sim;

    struct RobotSettings
    {
        int     robotModel;

        qreal   sensorScope;
        qreal   maximumAngle;
        qreal   maxWheelSpeed;
        qreal   minWheelSpeed;

        // angles of the sensors
        int     lSAngle1;
        int     lSAngle2;
        int     mSAngle1;
        int     mSAngle2;
        int     rSAngle1;
        int     rSAngle2;
        int     maxIntensity;
        bool    captorFood;
        bool    captorPoison;

        int     borderMode;
    } rob;

    struct nnSettings
    {
        int     neuronModel;
        int     neuralNetModel;

        int     numberNeurons;
        int     numberSensors;

        qreal   dt;
        int     stepTime;

        // Integrate and fire
        qreal   potResting_lif;
        qreal   potThreshold_lif;
        qreal   potSpiking_lif;

        qreal   maxWeight;
        qreal   minWeight;
        int     maxRefPeriodTime; // Refractory period
        int     minRefPeriodTime;
        int     maxDelayTime;
        int     minDelayTime;
        QString noiseMode; // random, initial
        qreal   noiseCurrent; // in percentage
        qreal   curFactor;
        qreal   curRecFactor;
        qreal   curSensFactor;
    } nn;

    struct condSettings
    {
        int     serialNumber; // Number that uniquely identify the process
        QString experiment; // Evolution, lesion, etc

        int     numGenerations; // Number of cycles of the genetic algorithm (only used in Condor mode)

        int     numSimulations; // Number of simulations for the lesion
    } cond;


    int appMode; // Graphic or condor mode
    int simMode; // Mode of the simulation
};



enum simMode
{
    standard,
    exploration,
    memo
};

enum AppMode
{
    graphical,
    condor
};

enum neuronMode
{
    QIF,
    Izhi
};

enum neuralNetMode
{
    spiking1Layer,
    CTRNN1Layer,
    zigzagNN
};

enum robotMode
{
    foodPoison,
    foodPoison_explo,
    noSensor,
    switchFP1,
    smell,
    memory,
    switchFP2
};

enum RobotBorderMode // Way the robots cop with borders
{
    blocked,
    through // pass through borders
};

namespace Neuron // To have numbers for important neurons
{
enum Neuron
{
    leftMotor,
    rightMotor,
    noise
};
}

#endif
