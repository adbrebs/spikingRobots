#ifndef NEURALNET_H
#define NEURALNET_H

#include <QVector>
#include <QList>
#include <QPair>
#include <QStringList>

class Settings;



class Layer
{
    friend class NeuralNet;
    friend class NeuralNet_1;
    friend class NeuralNet_2;
    friend class NeuralNet_zigzag;
    friend class NeuralNet_CTRNN;

public:
    Layer(Settings* settings, int aNumNeurons);

    virtual void initialize() = 0;
    virtual void reInitPotentials() = 0;
    virtual void computePotentials(int time) = 0;

    void updateFromGenome(const QVector<qreal>* pGenome, int *ppt);
    void appendParameters(QVector<qreal>* pGenome, QVector<qreal> *pMaxValues, QVector<qreal> *pMinValues, QVector<bool>* pCanChange) const;

    void writeOut(QDataStream & out);
    void writeIn(QDataStream & in);

    int getNumNeurons() const {return numNeurons;}
    int getNumParameters() const;

    const QStringList getNeuronNames() const {return neuronNames;}
    const QList<int> getNeuronColors() const {return neuronColors;}

    virtual bool isSpiking() const {return false;}
    virtual bool isCTRNN() const {return false;}

    virtual qreal getOutput(int neuron) const = 0;

    virtual qreal getPlotDisplay1(int neuron) const = 0; // For first plot
    virtual const QList<qreal>* getPlotDisplay2(int neuron) const = 0; // For second plot

protected:
    virtual void updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt) {}
    virtual void appendSpecificParameters(QVector<qreal>*, QVector<qreal>*, QVector<qreal>*, QVector<bool>* pCanChange) const {}

    virtual void writeOutSpecific(QDataStream & out) {}
    virtual void writeInSpecific(QDataStream & in) {}

    virtual int getNumSpecificParameters() const {return 0;}

protected:
    Settings*                   s;

    int                         numNeurons;

    qreal                       dt; // For Euler

    QStringList                 neuronNames;
    QList<int>                  neuronColors;
};



class Layer_Spiking : public Layer
{
    friend class NeuralNet;
    friend class NeuralNet_1;
    friend class NeuralNet_2;
    friend class NeuralNet_zigzag;

public:
    Layer_Spiking(Settings* settings, int aNumNeurons);

    /*virtual*/ void initialize();
    /*virtual*/ void reInitPotentials();
    /*virtual*/ void computePotentials(int time) = 0;

    qreal getMFR(int neuron) const {return meanFiringRates[neuron];}
    const QList<qreal>* getPPot(int neuron) const {return &potentials[neuron];}

    qreal getPotThreshold() const {return potentialThreshold;}
    qreal getPotSpiking() const {return potSpiking;}
    qreal getPotResting() const {return potentialResting;}

    /*virtual*/ bool isSpiking() const {return true;}

    /*virtual*/ qreal getOutput(int neuron) const {return getMFR(neuron);}

    /*virtual*/ qreal getPlotDisplay1(int neuron) const {return getMFR(neuron);}
    /*virtual*/ const QList<qreal>* getPlotDisplay2(int neuron) const {return getPPot(neuron);}

protected:
    virtual void initSpecificParameters() {} // if the neuron has specific parameters

    virtual void reInitSpecificPotentials() {}

    /*virtual*/ void updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt) {}
    /*virtual*/ void appendSpecificParameters(QVector<qreal>*, QVector<qreal>*, QVector<qreal>*, QVector<bool>* pCanChange) const {}

    /*virtual*/ void writeOutSpecific(QDataStream & out) {}
    /*virtual*/ void writeInSpecific(QDataStream & in) {}

    /*virtual*/ int getNumSpecificParameters() const {return 0;}

protected:
    QVector< QList<qreal> >     potentials;
    QVector< QList< int > >     firings;
    QVector< qreal >            currents;
    QVector< qreal >            meanFiringRates;
    QVector< int >              lastSpikes;

    qreal                       noiseCurrent;
    qreal                       curFactor;
    qreal                       potentialResting;
    qreal                       potentialThreshold;
    qreal                       potSpiking;

    int                         maxNumSpikes;

    int                         stepTime;
};



class Layer_QIF : public Layer_Spiking
{
public:
    Layer_QIF(Settings* settings, int aNumNeurons);

    /*virtual*/ void computePotentials(int time);

protected:
    /*virtual*/ void initSpecificParameters(); // if the neuron has specific parameters
    virtual qreal nextPotential(int neuron, qreal prevPot, qreal current) const;

    /*virtual*/ void updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt);
    /*virtual*/ void appendSpecificParameters(QVector<qreal>*, QVector<qreal>*, QVector<qreal>*, QVector<bool>* pCanChange) const;

    /*virtual*/ void writeOutSpecific(QDataStream & out);
    /*virtual*/ void writeInSpecific(QDataStream & in);

    /*virtual*/ int getNumSpecificParameters() const {return refPeriods.size();}

private:
    QVector< qreal >            refPeriods;
    qreal                       maxRefPeriodTime;
    qreal                       minRefPeriodTime;

    qreal potC;
    QVector< qreal > R;
    QVector< qreal > tau;
    QVector< qreal > a;
};




class Layer_Izhi : public Layer_Spiking
{
public:
    Layer_Izhi(Settings* settings, int aNumNeurons);

    /*virtual*/ void computePotentials(int time);

protected:
    /*virtual*/ void initSpecificParameters(); // if the neuron has specific parameters
    /*virtual*/ void reInitSpecificPotentials();

    /*virtual*/ void updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt);
    /*virtual*/ void appendSpecificParameters(QVector<qreal>*, QVector<qreal>*, QVector<qreal>*, QVector<bool>* pCanChange) const;

    /*virtual*/ void writeOutSpecific(QDataStream & out);
    /*virtual*/ void writeInSpecific(QDataStream & in);

    /*virtual*/ int getNumSpecificParameters() const;

private:
    QVector< QList<qreal> > recPotentials;
    QVector< qreal > a;
    QVector< qreal > b;
    QVector< qreal > c;
    QVector< qreal > d;
};




class Layer_CTRNN : public Layer
{
    friend class NeuralNet_CTRNN;

public:
    Layer_CTRNN(Settings* settings, int aNumNeurons);

    /*virtual*/ void initialize();
    /*virtual*/ void computePotentials(int time);
    /*virtual*/ void reInitPotentials();

    /*virtual*/ bool isCTRNN() const {return true;}

    /*virtual*/ qreal getOutput(int neuron) const {return potentials[neuron];}

    /*virtual*/ qreal getPlotDisplay1(int neuron) const {return potentials[neuron];}
    /*virtual*/ const QList<qreal>* getPlotDisplay2(int neuron) const {return NULL;}

protected:
    /*virtual*/ void updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt);
    /*virtual*/ void appendSpecificParameters(QVector<qreal>*, QVector<qreal>*, QVector<qreal>*, QVector<bool>* pCanChange) const;

    /*virtual*/ void writeOutSpecific(QDataStream & out);
    /*virtual*/ void writeInSpecific(QDataStream & in);

    /*virtual*/ int getNumSpecificParameters() const;

private:
    QVector< qreal > potentials;

    QVector< qreal > internalCurrents;
    QVector< qreal > externalCurrents;

    QVector< qreal > tau;
    QVector< qreal > bias;
};




class NeuralNet
{
public:
    NeuralNet(Settings* settings);
    virtual ~NeuralNet();

    void initializeNetwork();

    void run(const QVector<qreal> sensorsInputs, const qreal maxIntensity);

    void updateNeuralNet(const QVector<qreal>* pGenome);
    void reInitPotentials();
    void appendAllParameters(QVector<qreal>* pGenome, QVector<qreal> *pMaxValues, QVector<qreal> *pMinValues, QVector<bool>* pCanChange) const;

    const QVector< QVector< QVector<qreal> >* > getWeights() const {return weights;}
    QVector< QVector< QVector<qreal> >* >* getPWeights() {return &weights;}

    const QVector< Layer* > getLayers() const {return layers;}
    virtual const Layer* getMotorLayer() const = 0; // Return the layer that contains the motor neurons

    int getNumberNeurons() const {return numNeurons;}
    int getStepTime() const {return stepTime;}
    qreal getDt() const {return dt;}

    int getNumParameters() const; // For genome initialization

    void writeOut(QDataStream & out);
    void writeIn(QDataStream & in);

protected:
    virtual void initializeWeights() {}
    virtual void initializeDelays() {}

    virtual void appendSpecificParameters(QVector<qreal>*) const {} // if the neuron has specific parameters
    virtual void updateSpecificParameters(const QVector<qreal>*, int) {} // if the neuron has specific parameters

    virtual void computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity) {}
    virtual void noisyCurrent() {}
    void computePotentials();

protected:
    Settings* s;

    int numNeurons;

    QVector< Layer* > layers;

    QVector< QVector< QVector<qreal> >* > weights;
    QVector< QVector< QVector<int> >* > delays;

    qreal noiseCurrent;

    qreal maxWeight;
    qreal minWeight;

    int maxDelayTime;
    int minDelayTime;

    int time; // Time in msec
    qreal dt; // For Euler
    int stepTime; // number of iterations to compute the mean firing rates. In msec

};






class NeuralNet_1 : public NeuralNet
{
public:
    NeuralNet_1(Settings* settings);

    const Layer* getSensorsLayer() const {return sensorsLayer;}
    const Layer* getRecLayer() const {return recLayer;}
    /*virtual*/ const Layer* getMotorLayer() const {return recLayer;}

protected:
    /*virtual*/ void initializeWeights();
    /*virtual*/ void initializeDelays();

    /*virtual*/ void computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity);

private:
    int numRecNeurons;
    int numSensNeurons;

    Layer_Spiking* sensorsLayer;
    Layer_Spiking* recLayer;

    QVector< QVector<qreal> > weightsRec;
    QVector< QVector<qreal> > weightsSensors;

    QVector< QVector<int> > delaysRec;
    QVector< QVector<int> > delaysSensors;

    qreal curRecFactor;
    qreal curSensFactor;
};



class NeuralNet_2 : public NeuralNet
{
public:
    NeuralNet_2(Settings* settings);

    const Layer* getRecLayer() const {return recLayer;}
    /*virtual*/ const Layer* getMotorLayer() const {return recLayer;}


protected:
    /*virtual*/ void initializeWeights();
    /*virtual*/ void initializeDelays();
    /*virtual*/ void computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity);
    /*virtual*/ void noisyCurrent();

private:
    int numRecNeurons;
    int numSensors;

    Layer_Spiking* recLayer;

    QVector< QVector<qreal> > weightsRec;
    QVector< QVector<qreal> > weightsSensors;

    QVector< QVector<int> > delaysRec;

    qreal curRecFactor;
    qreal curSensFactor;
};



class NeuralNet_zigzag : public NeuralNet
{
public:
    NeuralNet_zigzag(Settings* settings);

    const Layer* getRecLayer() const {return recLayer;}
    /*virtual*/ const Layer* getMotorLayer() const {return recLayer;}

protected:
    /*virtual*/ void initializeWeights();
    /*virtual*/ void initializeDelays();
    /*virtual*/ void computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity);

private:
    int numRecNeurons;

    Layer_Spiking* recLayer;

    QVector< QVector<qreal> > weightsRec;

    QVector< QVector<int> > delaysRec;

    qreal curRecFactor;
};




class NeuralNet_CTRNN : public NeuralNet
{
public:
    NeuralNet_CTRNN(Settings* settings);

    const Layer* getRecLayer() const {return recLayer;}
    /*virtual*/ const Layer* getMotorLayer() const {return recLayer;}

protected:
    /*virtual*/ void initializeWeights();
    /*virtual*/ void initializeDelays() {}
    /*virtual*/ void computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity);

private:
    int numRecNeurons;
    int numSensors;

    Layer_CTRNN* recLayer;

    QVector< QVector<qreal> > weightsRR;
    QVector< QVector<qreal> > weightsRS;

    qreal curRecFactor;
    qreal curSensFactor;
};









//class NeuralNet_LIF : public NeuralNet
//{
//public:
//    NeuralNet_LIF(Settings* settings);

//    const QVector< qreal > getMemoryFactors() const {return memoryFactors;}

//    /*virtual*/ int getNumParameters() const; // For genome initialization

//    /*virtual*/ void writeOut(QDataStream & out);
//    /*virtual*/ void writeIn(QDataStream & in);

//private:
//    /*virtual*/ void initSpecificParameters();
//    /*virtual*/ void appendSpecificParameters(QVector<qreal>* pGenome) const;
//    /*virtual*/ void updateSpecificParameters(const QVector<qreal>* pGenome, int pt);
//    /*virtual*/ qreal nextPotential(int neuron, qreal prevPot, qreal current) const;

//private:
//    QVector< qreal > memoryFactors;
//};





#endif // NEURALNET_H
