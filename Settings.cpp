#include <ctime>
#include <cstdlib>
#include <QSettings>
#include <math.h>
#include <QFile>
#include <QTextStream>

#include "h_Robot.h"
#include "NeuralNet.h"
#include "h_Simulation.h"
#include "graph_Robot.h"
#include "graph_Simulation.h"

#include "Settings.h"


Settings::Settings(QSettings* pQSettings)
{
    appMode                 = pQSettings->value("appMode", graphical).toInt();
    simMode                 = pQSettings->value("simMode", standard).toInt();



    //////////////////////////////////
    // Evolution
    //////////////////////////////////
    pQSettings->beginGroup("Evolution");
    ev.popSize              = pQSettings->value("popSize", 100).toInt();
    ev.numFoods             = pQSettings->value("numFoods", 10).toInt();
    ev.numPoisons           = pQSettings->value("numPoisons", 20).toInt();
    ev.numWallPoisons       = pQSettings->value("numWallPoisons", 100).toInt();
    ev.lengthSim            = pQSettings->value("lengthSim", 500).toInt();
    ev.lengthLastSim        = pQSettings->value("lengthLastSim", 500).toInt();
    ev.avgLastSim           = pQSettings->value("avgLastSim", 5).toInt();
    ev.crossOverType        = pQSettings->value("crossOverType", "barycentric").toString();
    ev.crossOverRate        = pQSettings->value("crossOverRate", 0.5).toReal();
    ev.mutationRate         = pQSettings->value("mutationRate", 0.05).toReal();
    ev.mutationAmpli        = pQSettings->value("mutationAmpli", 0.6).toReal();
    ev.worldWidth           = pQSettings->value("worldWidth", 800).toReal();

    // Fitness function
    ev.fitFoodCoeff         = pQSettings->value("Fitness/fitFoodCoeff", 1).toInt();
    ev.fitPoisCoeff         = pQSettings->value("Fitness/fitPoisCoeff", -2).toInt();
    pQSettings->endGroup();

    //////////////////////////////////
    // Graphical Simulation
    //////////////////////////////////
    pQSettings->beginGroup("GraphicalSimulation");
    sim.numRobots           = pQSettings->value("numRobots", 1).toInt();
    sim.numFoods            = pQSettings->value("numFoods", 10).toInt();
    sim.numPoisons          = pQSettings->value("numPoisons", 20).toInt();
    sim.numWallPoisons      = pQSettings->value("numWallPoisons", 100).toInt();
    sim.worldWdith          = pQSettings->value("worldWdith", 800).toReal();

    sim.numBunches          = pQSettings->value("explo/numBunches", 5).toInt();
    sim.radiusBunch         = pQSettings->value("explo/radiusBunch", 30).toReal();

    sim.timeOutInterval     = pQSettings->value("timeOutInterval", 1000/10).toReal();
    pQSettings->endGroup();


    //////////////////////////////////
    // Robot
    //////////////////////////////////
    pQSettings->beginGroup("Robot");
    rob.robotModel          = pQSettings->value("robotModel", foodPoison).toInt();

    rob.sensorScope         = pQSettings->value("sensorScope", 300).toReal();
    rob.maximumAngle        = pQSettings->value("maximumAngle", 1).toReal();
    rob.maxWheelSpeed       = pQSettings->value("maxWheelSpeed", 20).toReal();
    rob.minWheelSpeed       = pQSettings->value("minWheelSpeed", 0).toReal();

    // angles of the sensors
    rob.lSAngle1            = pQSettings->value("Sensors/lSAngle1", 90).toInt();
    rob.lSAngle2            = pQSettings->value("Sensors/lSAngle2", 23).toInt();
    rob.mSAngle1            = pQSettings->value("Sensors/mSAngle1", 22).toInt();
    rob.mSAngle2            = pQSettings->value("Sensors/mSAngle2", -22).toInt();
    rob.rSAngle1            = pQSettings->value("Sensors/rSAngle1", -23).toInt();
    rob.rSAngle2            = pQSettings->value("Sensors/rSAngle2", -90).toInt();
    rob.maxIntensity        = pQSettings->value("Sensors/maxIntensity", 2).toInt();
    rob.captorFood          = pQSettings->value("Sensors/captorFood", true).toBool();
    rob.captorPoison        = pQSettings->value("Sensors/captorPoison", true).toBool();

    rob.borderMode          = pQSettings->value("borderMode", through).toInt();
    pQSettings->endGroup();


    //////////////////////////////////
    // Neural Network
    //////////////////////////////////
    pQSettings->beginGroup("NeuralNetwork");
    nn.neuronModel          = pQSettings->value("neuronModel", QIF).toInt();
    nn.neuralNetModel       = pQSettings->value("neuralNetModel", spiking1Layer).toInt();

    nn.numberNeurons        = pQSettings->value("numberNeurons", 6).toInt();
    nn.numberSensors        = numCaptors();

    nn.dt                   = pQSettings->value("dt", 1).toReal();
    nn.stepTime             = pQSettings->value("stepTime", 30).toInt();

    // Integrate and fire
    nn.potResting_lif       = pQSettings->value("LIF/potResting_lif", -65).toReal();
    nn.potThreshold_lif     = pQSettings->value("LIF/potThreshold_lif", -50).toReal();
    nn.potSpiking_lif       = pQSettings->value("LIF/potSpiking_lif", 30).toReal();

    nn.maxWeight            = pQSettings->value("maxWeight", 1).toReal();
    nn.minWeight            = pQSettings->value("minWeight", -1).toReal();
    nn.maxRefPeriodTime     = pQSettings->value("maxRefPeriodTime", 2).toInt();
    nn.minRefPeriodTime     = pQSettings->value("minRefPeriodTime", 1).toInt();
    nn.maxDelayTime         = pQSettings->value("maxDelayTime", 2).toInt();
    nn.minDelayTime         = pQSettings->value("minDelayTime", 1).toInt();
    nn.noiseMode            = pQSettings->value("noiseMode", "random").toString();
    nn.noiseCurrent         = pQSettings->value("noiseCurrent", 0.1).toReal(); // In percentage between 0 and 1
    nn.curFactor            = pQSettings->value("curFactor", 20).toReal();
    nn.curRecFactor         = pQSettings->value("curRecFactor", 1).toReal();
    nn.curSensFactor        = pQSettings->value("curSensFactor", 5).toReal();
    pQSettings->endGroup();


    //////////////////////////////////
    // For Condor
    //////////////////////////////////
    pQSettings->beginGroup("Condor");

    cond.serialNumber       = pQSettings->value("serialNumber", 0).toInt();
    cond.experiment         = pQSettings->value("experiment", "evolution").toString();

    cond.numGenerations     = pQSettings->value("evolution/numGenerations", 100).toInt();
    cond.numSimulations     = pQSettings->value("lesion/numSimulations", 200).toInt();
    pQSettings->endGroup();
}


void Settings::writeSettingsToFile(QSettings* pQSettings)
{
    pQSettings->setValue("appMode", appMode);
    pQSettings->setValue("simMode", simMode);

    //////////////////////////////////
    // Evolution
    //////////////////////////////////
    pQSettings->beginGroup("Evolution");
    pQSettings->setValue("popSize", ev.popSize);
    pQSettings->setValue("numFoods", ev.numFoods);
    pQSettings->setValue("numPoisons", ev.numPoisons);
    pQSettings->setValue("numWallPoisons", ev.numWallPoisons);
    pQSettings->setValue("lengthSim", ev.lengthSim);
    pQSettings->setValue("lengthLastSim", ev.lengthLastSim);
    pQSettings->setValue("avgLastSim", ev.avgLastSim);
    pQSettings->setValue("crossOverType", ev.crossOverType);
    pQSettings->setValue("crossOverRate", ev.crossOverRate);
    pQSettings->setValue("mutationRate", ev.mutationRate);
    pQSettings->setValue("mutationAmpli", ev.mutationAmpli);
    pQSettings->setValue("worldWidth", ev.worldWidth);

    pQSettings->setValue("Fitness/fitFoodCoeff", ev.fitFoodCoeff);
    pQSettings->setValue("Fitness/fitPoisCoeff", ev.fitPoisCoeff);
    pQSettings->endGroup();


    //////////////////////////////////
    // Graphical Simulation
    //////////////////////////////////
    pQSettings->beginGroup("GraphicalSimulation");
    pQSettings->setValue("numRobots", sim.numRobots);
    pQSettings->setValue("numFoods", sim.numFoods);
    pQSettings->setValue("numPoisons", sim.numPoisons);
    pQSettings->setValue("numWallPoisons", sim.numWallPoisons);
    pQSettings->setValue("worldWdith", sim.worldWdith);
    pQSettings->setValue("explo/numBunches", sim.numBunches);
    pQSettings->setValue("explo/radiusBunch", sim.radiusBunch);
    pQSettings->setValue("timeOutInterval", sim.timeOutInterval);
    pQSettings->endGroup();


    //////////////////////////////////
    // Robot
    //////////////////////////////////
    pQSettings->beginGroup("Robot");
    pQSettings->setValue("robotModel", rob.robotModel);

    pQSettings->setValue("sensorScope", rob.sensorScope);
    pQSettings->setValue("maximumAngle", rob.maximumAngle);
    pQSettings->setValue("maxWheelSpeed", rob.maxWheelSpeed);
    pQSettings->setValue("minWheelSpeed", rob.minWheelSpeed);

    pQSettings->setValue("Sensors/lSAngle1", rob.lSAngle1);
    pQSettings->setValue("Sensors/lSAngle2", rob.lSAngle2);
    pQSettings->setValue("Sensors/mSAngle1", rob.mSAngle1);
    pQSettings->setValue("Sensors/mSAngle2", rob.mSAngle2);
    pQSettings->setValue("Sensors/rSAngle1", rob.rSAngle1);
    pQSettings->setValue("Sensors/rSAngle2", rob.rSAngle2);
    pQSettings->setValue("Sensors/maxIntensity", rob.maxIntensity);

    pQSettings->setValue("Sensors/captorFood", rob.captorFood);
    pQSettings->setValue("Sensors/captorPoison", rob.captorPoison);
    pQSettings->setValue("borderMode", rob.borderMode);
    pQSettings->endGroup();

    //////////////////////////////////
    // Neural Network
    //////////////////////////////////
    pQSettings->beginGroup("NeuralNetwork");
    pQSettings->setValue("neuronModel", nn.neuronModel);
    pQSettings->setValue("neuralNetModel", nn.neuralNetModel);

    pQSettings->setValue("numberNeurons", nn.numberNeurons);
    pQSettings->setValue("numberSensors", nn.numberSensors);

    pQSettings->setValue("dt", nn.dt);
    pQSettings->setValue("stepTime", nn.stepTime);

    pQSettings->setValue("LIF/potResting_lif", nn.potResting_lif);
    pQSettings->setValue("LIF/potThreshold_lif", nn.potThreshold_lif);
    pQSettings->setValue("LIF/potSpiking_lif", nn.potSpiking_lif);

    pQSettings->setValue("maxWeight", nn.maxWeight);
    pQSettings->setValue("minWeight", nn.minWeight);
    pQSettings->setValue("maxRefPeriodTime", nn.maxRefPeriodTime);
    pQSettings->setValue("minRefPeriodTime", nn.minRefPeriodTime);
    pQSettings->setValue("maxDelayTime", nn.maxDelayTime);
    pQSettings->setValue("minDelayTime", nn.minDelayTime);
    pQSettings->setValue("noiseMode", nn.noiseMode);
    pQSettings->setValue("noiseCurrent", nn.noiseCurrent);
    pQSettings->setValue("curFactor", nn.curFactor);
    pQSettings->setValue("curRecFactor", nn.curRecFactor);
    pQSettings->setValue("curSensFactor", nn.curSensFactor);
    pQSettings->endGroup();


    //////////////////////////////////
    // For Condor
    //////////////////////////////////
    pQSettings->beginGroup("Condor");
    pQSettings->setValue("serialNumber", cond.serialNumber);
    pQSettings->setValue("experiment", cond.experiment);

    pQSettings->setValue("evolution/numGenerations", cond.numGenerations);
    pQSettings->setValue("lesion/numSimulations", cond.numSimulations);
    pQSettings->endGroup();
}



h_Simulation* Settings::createSim(Settings* settings, int aNumberRobot, int aNumberFoods,
                                  int aNumberPoisons, qreal worldWidth) const
{
    h_Simulation* sim = NULL;


    if(simMode == standard)
        sim = new h_SimStandard(settings, aNumberRobot, aNumberFoods,
                                aNumberPoisons, worldWidth);
    else if(simMode == exploration)
        sim = new h_SimExplo(settings, aNumberRobot, aNumberFoods,
                             aNumberPoisons, worldWidth);
    else if(simMode == memo)
        sim = new h_SimMemo(settings, aNumberRobot, aNumberFoods,
                            aNumberPoisons, worldWidth);
    else
        qDebug("Error, no other simulation Mode");


    sim->initSimulation();

    return sim;
}


NeuralNet* Settings::createNeuralNet(Settings* settings) const
{
    NeuralNet* net = NULL;

    if(nn.neuralNetModel == zigzagNN)
        net = new NeuralNet_zigzag(settings);
    else if(nn.neuralNetModel == CTRNN1Layer)
        net = new NeuralNet_CTRNN(settings);
    else if(nn.neuralNetModel == spiking1Layer)
        net = new NeuralNet_2(settings);

    return net;
}


Layer_Spiking* Settings::createLayer(Settings* settings, int aNumNeurons) const
{
    Layer_Spiking* layer = NULL;

    if(nn.neuronModel == QIF)
        layer = new Layer_QIF(settings, aNumNeurons);
    else if(nn.neuronModel == Izhi)
        layer = new Layer_Izhi(settings, aNumNeurons);

    return layer;
}


h_Robot* Settings::createRobot(Settings* settings, h_Simulation *aSimulation, qreal aRadius)
{
    h_Robot* robot = NULL;

    if(rob.robotModel == foodPoison || rob.robotModel == memory)
        robot = new h_Robot_1(settings, aSimulation, aRadius);
    else if(rob.robotModel == foodPoison_explo)
        robot = new h_Robot_explo(settings, aSimulation, aRadius);
    else if(rob.robotModel == noSensor)
        robot = new h_Robot_zigzag(settings, aSimulation, aRadius);
    else if(rob.robotModel == switchFP1)
        robot = new h_Robot_switch_1(settings, aSimulation, aRadius);
    else if(rob.robotModel == switchFP2)
        robot = new h_Robot_switch_2(settings, aSimulation, aRadius);
    else if(rob.robotModel == smell)
        robot = new h_Robot_smell(settings, aSimulation, aRadius);

    return robot;
}


graph_Robot* Settings::createGraphRobot(Settings* settings, graph_Simulation* aGraph_Sim, h_Robot* aRobot)
{
    graph_Robot* graphRobot = NULL;

    if(rob.robotModel == foodPoison_explo)
        graphRobot = new graph_Robot_Explo(settings, aGraph_Sim, aRobot);
    else
        graphRobot = new graph_Robot(settings, aGraph_Sim, aRobot);

    return graphRobot;
}


//////////////////////
// Useful functions //
//////////////////////


// Pi
const double Pi = 3.14159265358979323846264338327950288419717;
double TwoPi = 2.0 * Pi;


qreal Settings::randNumber(qreal min, qreal max)
{
    if(min == max)
        return min;

    Q_ASSERT(max > min);
    qreal ret = (qreal)qrand() / (qreal)RAND_MAX; // Gives a value between 0.0 and 1.0
    ret *= max - min; // Gives a value between 0 and (max - min)
    ret += min; // Gives a value between min and max

    return ret;
}

int Settings::randInteger(int min, int max)
{
    if(min == max)
        return min;

    Q_ASSERT(max > min);
    int ret = qrand() % (max-min);
    ret += min;

    return ret;
}

// Normalize angle between 0 and 360 degrees
qreal Settings::normAngle(qreal angle)
{
    angle = (int)angle%360;
    if(angle < 0)
        angle += 360;

    return angle;
}

int Settings::normAngle(int angle)
{
    angle = angle%360;
    if(angle < 0)
        angle += 360;

    return angle;
}

int Settings::normAngle180(int angle)
{
    angle = angle%360;
    if(angle > 180)
        angle -= 360;
    else if(angle < -180)
        angle += 360;

    return angle;
}

// Transform a degree angle into a radian angle
qreal Settings::radian(qreal angle)
{
    return normAngle(angle)*TwoPi/360;
}


// Transform a radian angle into a degree angle
qreal Settings::degree(qreal angle)
{
    return normAngle(angle*360/TwoPi);
}


int Settings::numCaptors()
{
    if(rob.robotModel == foodPoison)
        return (rob.captorFood + rob.captorPoison) * 3;
    else if (rob.robotModel == noSensor || rob.robotModel == memory)
        return 0;
    else if (rob.robotModel == foodPoison_explo)
        return 2;
    else if(rob.robotModel == smell)
        return 4;
    else if(rob.robotModel == switchFP1 || rob.robotModel == switchFP2)
        return 1 + (rob.captorFood + rob.captorPoison) * 3;

    return 0;
}


qreal Settings::sigmoid(qreal x)
{
    return 1.0/(1+exp(-x));
}


bool Settings::saveMatrixInFile(QString filename, QVector< QVector<qreal> >* pMatrix)
{
    QFile* pFile = new QFile(filename);
    pFile->open(QIODevice::WriteOnly);

    // Write in the file
    QTextStream out(pFile);

    for(int i = 0; i<pMatrix->size(); ++i)
    {
        for(int k = 0; k<pMatrix->first().size(); ++k)
            out << pMatrix->at(i)[k] << " ";
        out << endl;
    }

    delete pFile;
}
