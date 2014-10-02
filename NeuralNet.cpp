#include <iostream>
#include <QDataStream>

#include "Settings.h"
#include "NeuralNet.h"









///////////////////////////////
// Layer
///////////////////////////////
Layer::Layer(Settings* settings, int aNumNeurons)
    : s(settings),
      numNeurons(aNumNeurons),
      dt(s->nn.dt)
{
}


void Layer::updateFromGenome(const QVector<qreal>* pGenome, int* ppt)
{
    updateSpecificFromGenome(pGenome, ppt);
}


void Layer::appendParameters(QVector<qreal>* pGenome, QVector<qreal>* pMaxValues, QVector<qreal>* pMinValues, QVector<bool>* pCanChange) const
{
    appendSpecificParameters(pGenome, pMaxValues, pMinValues, pCanChange);
}


void Layer::writeOut(QDataStream & out)
{
    // Specific parameters
    writeOutSpecific(out);
}


void Layer::writeIn(QDataStream & in)
{
    // Specific parameters
    writeInSpecific(in);
}


int Layer::getNumParameters() const
{
    int size = 0;

    size += getNumSpecificParameters();

    return size;
}





///////////////////////////////
// Layer_Spiking
///////////////////////////////
Layer_Spiking::Layer_Spiking(Settings *settings, int aNumNeurons)
    : Layer(settings, aNumNeurons),
      noiseCurrent(s->nn.noiseCurrent),
      curFactor(s->nn.curFactor),
      potentialResting(0),
      potentialThreshold(0.1),
      potSpiking(30),
      stepTime(s->nn.stepTime)
{
    maxNumSpikes = stepTime;
}


void Layer_Spiking::initialize()
{
    for(int i=0; i<numNeurons; ++i)
    {
        initSpecificParameters();

        // Initial firings
        QList<int> aNeuronFirings;
        for(int k=0; k<stepTime; ++k)
            aNeuronFirings.append(0);
        firings.append(aNeuronFirings);

        // Currents
        currents.append(0);

        // Initial last spikes
        lastSpikes.append(-100);

        // Initial Mean firing rates
        meanFiringRates.append(0);

        // Initial potentials
        QList<qreal> aNeuronPotentials;
        for(int k=0; k<stepTime; ++k)
            aNeuronPotentials.append(potentialResting);
        potentials.append(aNeuronPotentials);
    }
}


// Reinitializing potentials, MFRs and firings
void Layer_Spiking::reInitPotentials()
{
    for(int i=0; i < numNeurons; ++i)
    {
        for(int k=0; k < stepTime; ++k)
        {
            firings[i][k] = 0;
            //potentials[i][k] = potentialThreshold;
            potentials[i][k] = potentialResting;
        }
        meanFiringRates[i] = 0;
        lastSpikes[i] = -100;
        currents[i] = 0;
    }

    reInitSpecificPotentials();
}




///////////////////////////////
// Layer_QIF
///////////////////////////////
Layer_QIF::Layer_QIF(Settings *settings, int aNumNeurons)
    : Layer_Spiking(settings, aNumNeurons),
      maxRefPeriodTime(s->nn.maxRefPeriodTime),
      minRefPeriodTime(s->nn.minRefPeriodTime)
{
    if(s->nn.minRefPeriodTime != 0)
        maxNumSpikes = stepTime/s->nn.minRefPeriodTime;

    potentialResting = -65;
    potentialThreshold = -30;
    potSpiking = 30;

    potC = -50;

    for(int i = 0; i<numNeurons; ++i)
    {
        R.append(1);
        tau.append(5);
        a.append(0.2);
    }
}


// Calculate new potentials from the previous ones
/*virtual*/ void Layer_QIF::computePotentials(int time)
{
    for(int k = 0; k<numNeurons; ++k)
    {
        // Compute next potential
        qreal nextPot = potentials[k].first();

        for(int e=0; e < 1/dt; ++e)
            nextPot = nextPotential(k, nextPot, currents[k]);

        potentials[k].prepend(nextPot);


        // See if the neuron fires or not
        if(potentials[k].first() > potentialThreshold && time - lastSpikes[k] > refPeriods[k])
        {
            lastSpikes[k] = time;
            firings[k].prepend(1);
            potentials[k][1] = potSpiking;
            potentials[k].first() = potentialResting;
            meanFiringRates[k] += 1/(qreal)maxNumSpikes; // update the mean firing rate
        }
        else
        {
            firings[k].prepend(0);
            // When potential is below the resting potential, reset it.
            if(potentials[k].first() < potentialResting - 10)
                potentials[k].first() = potentialResting;
        }

        // Update MFR, firings, current
        potentials[k].removeLast();
        meanFiringRates[k] -= firings[k].last()/(qreal)maxNumSpikes;
        firings[k].removeLast();
        currents[k] = 0;

    }
}


/*virtual*/ void Layer_QIF::initSpecificParameters()
{
    // Refractory Periods
    refPeriods.append(s->randNumber(minRefPeriodTime, maxRefPeriodTime));
}


/*virtual*/ qreal Layer_QIF::nextPotential(int n, qreal prevPot, qreal current) const
{
    return prevPot
            + dt * (R[n]/tau[n]) * 20 * curFactor * current
            + dt * (a[n]/tau[n]) * (potentialResting - prevPot) * (potC - prevPot);
}


/*virtual*/ void Layer_QIF::updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt)
{
    // Refractory Period
    for(int i=0; i < numNeurons; ++i)
        refPeriods[i] = pGenome->at((*ppt)++);
}


/*virtual*/ void Layer_QIF::appendSpecificParameters(QVector<qreal>* pGenome, QVector<qreal>* pMaxValues, QVector<qreal>* pMinValues, QVector<bool>* pCanChange) const
{
    // Refractory Period
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(refPeriods[i]);
        pMaxValues->append(maxRefPeriodTime);
        pMinValues->append(minRefPeriodTime);
        pCanChange->append(false);
    }
}


/*virtual*/ void Layer_QIF::writeOutSpecific(QDataStream & out)
{
    // Refractory Period
    out << refPeriods;
}


/*virtual*/ void Layer_QIF::writeInSpecific(QDataStream & in)
{
    // Refractory Period
    in >> refPeriods;
}



///////////////////////////////
// Layer_Izhi
///////////////////////////////
Layer_Izhi::Layer_Izhi(Settings *settings, int aNumNeurons)
    : Layer_Spiking(settings, aNumNeurons)
{
    potentialResting = -65;
    potentialThreshold = 30;
    potSpiking = 30;

    maxNumSpikes = stepTime/10;
}


/*virtual*/ void Layer_Izhi::initSpecificParameters()
{
    a.append(0.02);
    b.append(0.2);
    c.append(-65 + s->randNumber(0,1.5));
    d.append(8 + s->randNumber(-1,0));

    // Initial recovery potentials
    QList<qreal> aNeuronPotentials;
    for(int k=0; k<stepTime; ++k)
        aNeuronPotentials.append(0);
    recPotentials.append(aNeuronPotentials);
}


/*virtual*/ void Layer_Izhi::reInitSpecificPotentials()
{
    for(int i=0; i < numNeurons; ++i)
        for(int k=0; k < stepTime; ++k)
            recPotentials[i][k] = 0;
}


// Calculate new potentials from the previous ones
/*virtual*/ void Layer_Izhi::computePotentials(int time)
{
    for(int k = 0; k<numNeurons; ++k)
    {
        // Compute next potential
        qreal nextPot = potentials[k].first();
        qreal nextRecPot = recPotentials[k].first();

        for(int e=0; e < 1/dt; ++e)
        {
            qreal buffPot = nextPot;
            qreal buffRecPot = nextRecPot;

            nextPot = buffPot + dt * (0.04*buffPot*buffPot + 5*buffPot + 140 - buffRecPot + curFactor * currents[k]);
            nextRecPot = buffRecPot + dt * (a[k] * (b[k]*buffPot - buffRecPot));
        }

        potentials[k].prepend(nextPot);
        recPotentials[k].prepend(nextRecPot);


        // See if the neuron fires or not
        if(potentials[k].first() > potentialThreshold)
        {
            firings[k].prepend(1);
            potentials[k][1] = potSpiking;
            potentials[k].first() = c[k];
            meanFiringRates[k] += 1/(qreal)maxNumSpikes; // update the mean firing rate
            recPotentials[k].first() += d[k];
        }
        else
        {
            firings[k].prepend(0);
            // When potential is below the resting potential, reset it.
            if(potentials[k].first() < potentialResting - 10)
                potentials[k].first() = potentialResting;
        }

        // Update MFR, firings, current
        potentials[k].removeLast();
        recPotentials[k].removeLast();
        meanFiringRates[k] -= firings[k].last()/(qreal)maxNumSpikes;
        firings[k].removeLast();
        currents[k] = 0;
    }
}


/*virtual*/ void Layer_Izhi::updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt)
{
    // a
    for(int i=0; i < numNeurons; ++i)
        a[i] = pGenome->at((*ppt)++);

    // b
    for(int i=0; i < numNeurons; ++i)
        b[i] = pGenome->at((*ppt)++);

    // c
    for(int i=0; i < numNeurons; ++i)
        c[i] = pGenome->at((*ppt)++);

    // d
    for(int i=0; i < numNeurons; ++i)
        d[i] = pGenome->at((*ppt)++);
}


/*virtual*/ void Layer_Izhi::appendSpecificParameters(QVector<qreal>* pGenome, QVector<qreal>* pMaxValues, QVector<qreal>* pMinValues, QVector<bool>* pCanChange) const
{
    // a
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(a[i]);
        pMaxValues->append(a[i]);
        pMinValues->append(a[i]);
        pCanChange->append(false);
    }

    // b
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(b[i]);
        pMaxValues->append(b[i]);
        pMinValues->append(b[i]);
        pCanChange->append(false);
    }

    // c
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(c[i]);
        pMaxValues->append(-65);
        pMinValues->append(-67);
        pCanChange->append(true);
    }

    // d
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(d[i]);
        pMaxValues->append(8);
        pMinValues->append(7);
        pCanChange->append(true);
    }
}


/*virtual*/ void Layer_Izhi::writeOutSpecific(QDataStream & out)
{
    out << a;
    out << b;
    out << c;
    out << d;
}


/*virtual*/ void Layer_Izhi::writeInSpecific(QDataStream & in)
{
    in >> a;
    in >> b;
    in >> c;
    in >> d;
}


/*virtual*/ int Layer_Izhi::getNumSpecificParameters() const
{
    int size = 0;

    size += a.size();
    size += b.size();
    size += c.size();
    size += d.size();

    return size;
}




///////////////////////////////
// Layer_CTRNN
///////////////////////////////
Layer_CTRNN::Layer_CTRNN(Settings *settings, int aNumNeurons)
    : Layer(settings, aNumNeurons)
{
}


/*virtual*/ void Layer_CTRNN::initialize()
{
    for(int i=0; i<numNeurons; ++i)
    {
        tau.append(s->randNumber(1,50));
        bias.append(s->randNumber(-1,1));

        potentials.append(0);

        internalCurrents.append(0);
        externalCurrents.append(0);
    }
}


/*virtual*/ void Layer_CTRNN::reInitPotentials()
{
    for(int i=0; i < numNeurons; ++i)
        potentials[i] = 0;
}


// Calculate new potentials from the previous ones
/*virtual*/ void Layer_CTRNN::computePotentials(int time)
{
    for(int k = 0; k<numNeurons; ++k)
        for(int e=0; e < 1/dt; ++e)
        {
            potentials[k] += (dt/tau[k]) * (-potentials[k]
                                            + s->nn.curRecFactor * internalCurrents[k]
                                            + s->nn.curSensFactor * externalCurrents[k]);

            if(potentials[k] > 20)
                potentials[k] = 20;
            else if(potentials[k] < -20)
                potentials[k] = -20;
        }
}


/*virtual*/ void Layer_CTRNN::updateSpecificFromGenome(const QVector<qreal>* pGenome, int *ppt)
{
    // tau
    for(int i=0; i < numNeurons; ++i)
        tau[i] = pGenome->at((*ppt)++);

    // bias
    for(int i=0; i < numNeurons; ++i)
        bias[i] = pGenome->at((*ppt)++);
}


/*virtual*/ void Layer_CTRNN::appendSpecificParameters(QVector<qreal>* pGenome, QVector<qreal>* pMaxValues, QVector<qreal>* pMinValues, QVector<bool>* pCanChange) const
{
    // tau
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(tau[i]);
        pMaxValues->append(50);
        pMinValues->append(1);
        pCanChange->append(true);
    }

    // bias
    for(int i=0; i < numNeurons; ++i)
    {
        pGenome->append(bias[i]);
        pMaxValues->append(1);
        pMinValues->append(-1);
        pCanChange->append(true);
    }
}


/*virtual*/ void Layer_CTRNN::writeOutSpecific(QDataStream & out)
{
    out << tau;
    out << bias;
}


/*virtual*/ void Layer_CTRNN::writeInSpecific(QDataStream & in)
{
    in >> tau;
    in >> bias;
}


/*virtual*/ int Layer_CTRNN::getNumSpecificParameters() const
{
    int size = 0;

    size += tau.size();
    size += bias.size();

    return size;
}




///////////////////////////////
// NeuralNet
///////////////////////////////
NeuralNet::NeuralNet(Settings* settings)
    : s(settings),
      numNeurons(0),
      noiseCurrent(s->nn.noiseCurrent),
      maxWeight(s->nn.maxWeight),
      minWeight(s->nn.minWeight),
      maxDelayTime(s->nn.maxDelayTime),
      minDelayTime(s->nn.minDelayTime),
      time(0),
      dt(s->nn.dt),
      stepTime(s->nn.stepTime)
{
}


/*virtual*/ NeuralNet::~NeuralNet()
{
    foreach(Layer* layer, layers)
        delete layer;
}


void NeuralNet::initializeNetwork()
{
    // Initialize layers
    foreach(Layer* layer, layers)
        layer->initialize();

    // Initialize number of neurons
    foreach(Layer* layer, layers)
        numNeurons += layer->getNumNeurons();

    // Initialize weights
    initializeWeights();

    // Initialize delys
    initializeDelays();
}


void NeuralNet::run(const QVector<qreal> sensorsInputs, const qreal maxIntensity)
{
    for(int u = 0; u<stepTime; ++u)
    {
        time++;

        // Compute currents
        computeCurrents(sensorsInputs, maxIntensity);

        // Compute next potentials from previous ones and currents and
        // Synchronous updating of the potentials and firings
        computePotentials();
    }
}


void NeuralNet::computePotentials()
{
    foreach(Layer* layer, layers)
        layer->computePotentials(time);
}


void NeuralNet::reInitPotentials()
{
    time = 0;
    foreach(Layer* layer, layers)
        layer->reInitPotentials();
}


void NeuralNet::updateNeuralNet(const QVector<qreal>* pGenome)
{
    int pt = 0;

    // Update weights
    foreach(QVector< QVector<qreal> >* weight, weights)
        for(int i=0; i<weight->size(); ++i)
            for(int k=0; k<weight->first().size(); ++k)
                (*weight)[i][k] = pGenome->at(pt++);

    // Update delays
    foreach(QVector< QVector<int> >* delay, delays)
        for(int i=0; i<delay->size(); ++i)
            for(int k=0; k<delay->first().size(); ++k)
                (*delay)[i][k] = pGenome->at(pt++);

    // Update other parameters
    foreach(Layer* layer, layers)
        layer->updateFromGenome(pGenome, &pt);

    // Reinitializing potentials, MFRs and firings
    reInitPotentials();
}


void NeuralNet::appendAllParameters(QVector<qreal>* pGenome, QVector<qreal>* pMaxValues, QVector<qreal>* pMinValues, QVector<bool>* pCanChange) const
{
    // Append weights
    foreach(QVector< QVector<qreal> >* weight, weights)
        for(int i = 0; i < weight->size(); ++i)
            for(int k = 0; k < weight->first().size(); ++k)
            {
                pGenome->append((*weight)[i][k]);
                pMaxValues->append(maxWeight);
                pMinValues->append(minWeight);
                pCanChange->append(true);
            }

    // Delays
    foreach(QVector< QVector<int> >* delay, delays)
        for(int i = 0; i < delay->size(); ++i)
            for(int k = 0; k < delay->first().size(); ++k)
            {
                pGenome->append((*delay)[i][k]);
                pMaxValues->append(maxDelayTime);
                pMinValues->append(minDelayTime);
                pCanChange->append(true);
            }

    // Append other parameters
    foreach(Layer* layer, layers)
        layer->appendParameters(pGenome, pMaxValues, pMinValues, pCanChange);
}


void NeuralNet::writeOut(QDataStream & out)
{
    // Weights
    foreach(QVector< QVector<qreal> >* weight, weights)
        out << *weight;

    // Delays
    foreach(QVector< QVector<int> >* delay, delays)
        out << *delay;

    // Layers
    foreach(Layer* layer, layers)
        layer->writeOut(out);
}


void NeuralNet::writeIn(QDataStream & in)
{
    // Weights
    foreach(QVector< QVector<qreal> >* weight, weights)
        in >> *weight;

    // Delays
    foreach(QVector< QVector<int> >* delay, delays)
        in >> *delay;

    // Layers
    foreach(Layer* layer, layers)
        layer->writeIn(in);
}


int NeuralNet::getNumParameters() const
{
    int size = 0;

    foreach(QVector< QVector<qreal> >* weight, weights)
        size += weight->size() * weight->first().size();

    foreach(QVector< QVector<int> >* delay, delays)
        size += delay->size() * delay->first().size();

    foreach(Layer* layer, layers)
        size += layer->getNumParameters();

    return size;
}







///////////////////////////////
// NeuralNet_1
///////////////////////////////
NeuralNet_1::NeuralNet_1(Settings* settings)
    : NeuralNet(settings),
      numRecNeurons(s->nn.numberNeurons),
      numSensNeurons(s->nn.numberSensors),
      curRecFactor(s->nn.curRecFactor),
      curSensFactor(s->nn.curSensFactor)
{
    sensorsLayer = s->createLayer(s, numSensNeurons);
    layers.append(sensorsLayer);

    recLayer = s->createLayer(s, numRecNeurons);
    layers.append(recLayer);

    sensorsLayer->neuronNames += "Left Food sensor";
    sensorsLayer->neuronColors += Qt::darkGreen;
    sensorsLayer->neuronNames += "Right Food sensor";
    sensorsLayer->neuronColors += Qt::darkGreen;
    sensorsLayer->neuronNames += "Middle Food sensor";
    sensorsLayer->neuronColors += Qt::darkGreen;

    sensorsLayer->neuronNames += "Left Poison sensor";
    sensorsLayer->neuronColors += Qt::darkRed;
    sensorsLayer->neuronNames += "Right Poison sensor";
    sensorsLayer->neuronColors += Qt::darkRed;
    sensorsLayer->neuronNames += "Middle Poison sensor";
    sensorsLayer->neuronColors += Qt::darkRed;


    recLayer->neuronNames += "Left Motor Neuron";
    recLayer->neuronColors += Qt::blue;
    recLayer->neuronNames += "Right Motor Neuron";
    recLayer->neuronColors += Qt::magenta;
    for(int i=2; i < recLayer->numNeurons; ++i)
    {
        recLayer->neuronNames += "Hidden Neuron " + QString::number(i-1);
        recLayer->neuronColors += Qt::black;
    }
}


/*virtual*/ void NeuralNet_1::initializeWeights()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Weights between fully recurrent neurons
        QVector<qreal> aNeuronWeightsRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronWeightsRec.append(s->randNumber(minWeight,maxWeight));
        weightsRec.append(aNeuronWeightsRec);

        // Weights between sensor neurons and recurrent ones
        QVector<qreal> aNeuronWeightsSensors;
        for(int k=0; k<numSensNeurons; ++k)
            aNeuronWeightsSensors.append(s->randNumber(minWeight,maxWeight));
        weightsSensors.append(aNeuronWeightsSensors);
    }

    weights.append(&weightsRec);
    weights.append(&weightsSensors);
}


/*virtual*/ void NeuralNet_1::initializeDelays()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Delays between fully recurrent neurons
        QVector<int> aNeuronDelaysRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronDelaysRec.append(s->randInteger(0,maxDelayTime));
        delaysRec.append(aNeuronDelaysRec);

        // Weights between sensor neurons and recurrent ones
        QVector<int> aNeuronDelaysSensors;
        for(int k=0; k<numSensNeurons; ++k)
            aNeuronDelaysSensors.append(s->randInteger(0,maxDelayTime));
        delaysSensors.append(aNeuronDelaysSensors);
    }

    delays.append(&delaysRec);
    delays.append(&delaysSensors);
}


/*virtual*/ void NeuralNet_1::computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity)
{
    // Generating sensor spikes according to a Poisson law
    for(int i = 0; i < numSensNeurons; ++i)
        if(s->randNumber(0,1) < sensorsInputs[i] / maxIntensity)
            sensorsLayer->currents[i] += 1;


    // Compute the current received by the neurons
    for(int k = 0; k<numRecNeurons; ++k)
    {
        for(int i = 0; i < numRecNeurons; ++i)
            recLayer->currents[k] += recLayer->firings[i][delaysRec[k][i]]*weightsRec[k][i] * curRecFactor;

        for(int i = 0; i < numSensNeurons; ++i)
            recLayer->currents[k] += sensorsLayer->firings[i][delaysSensors[k][i]]*weightsSensors[k][i] * curSensFactor;
    }

    // Noise current for the motor neurons
    if(s->randNumber(0,1) < noiseCurrent)
        recLayer->currents[Neuron::leftMotor] += curRecFactor/2; // /2 because in average weights == 1/2
    if(s->randNumber(0,1) < noiseCurrent)
        recLayer->currents[Neuron::rightMotor] += curRecFactor/2;

    // Normalization
    //    for(int k = 0; k<numRecNeurons; ++k)
    //        recLayer->currents[k] /= (qreal)(numNeurons);
    //    for(int k = 0; k<numSensNeurons; ++k)
    //        sensorsLayer->currents[k] /= (qreal)(numNeurons);
}





///////////////////////////////
// NeuralNet_2
///////////////////////////////
NeuralNet_2::NeuralNet_2(Settings* settings)
    : NeuralNet(settings),
      numRecNeurons(s->nn.numberNeurons),
      numSensors(s->nn.numberSensors),
      curRecFactor(s->nn.curRecFactor),
      curSensFactor(s->nn.curSensFactor)
{
    recLayer = s->createLayer(s, numRecNeurons);
    layers.append(recLayer);

    recLayer->neuronNames += "Left Motor Neuron";
    recLayer->neuronColors += Qt::blue;
    recLayer->neuronNames += "Right Motor Neuron";
    recLayer->neuronColors += Qt::magenta;
    for(int i=2; i < recLayer->numNeurons; ++i)
    {
        recLayer->neuronNames += "Hidden Neuron " + QString::number(i-1);
        recLayer->neuronColors += Qt::black;
    }
}


/*virtual*/ void NeuralNet_2::initializeWeights()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Weights between fully recurrent neurons
        QVector<qreal> aNeuronWeightsRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronWeightsRec.append(s->randNumber(minWeight,maxWeight));
        weightsRec.append(aNeuronWeightsRec);

        // Weights between sensor neurons and recurrent ones
        QVector<qreal> aNeuronWeightsSensors;
        for(int k=0; k<numSensors; ++k)
            aNeuronWeightsSensors.append(s->randNumber(minWeight,maxWeight));
        weightsSensors.append(aNeuronWeightsSensors);
    }

    weights.append(&weightsRec);
    weights.append(&weightsSensors);
}


/*virtual*/ void NeuralNet_2::initializeDelays()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Delays between fully recurrent neurons
        QVector<int> aNeuronDelaysRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronDelaysRec.append(s->randInteger(0,maxDelayTime));
        delaysRec.append(aNeuronDelaysRec);
    }

    delays.append(&delaysRec);
}


/*virtual*/ void NeuralNet_2::computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity)
{
    QVector<qreal> sensorsSpikes = sensorsInputs;

    // Generating sensor spikes according to a Poisson law
    for(int i = 0; i < numSensors; ++i)
        if(s->randNumber(0,1) < sensorsInputs[i] / maxIntensity)
            sensorsSpikes[i] = 1;
        else
            sensorsSpikes[i] = 0;


    // Compute the current received by the neurons
    for(int k = 0; k<numRecNeurons; ++k)
    {
        qreal sum = 0;
        for(int i = 0; i < numRecNeurons; ++i)
            sum += recLayer->firings[i][delaysRec[k][i]]*weightsRec[k][i] * curRecFactor;

        for(int i = 0; i < numSensors; ++i)
            sum += sensorsSpikes[i]*weightsSensors[k][i] * curSensFactor;

        recLayer->currents[k] = sum;

        // Normalization
        // recLayer->currents[k] /= (qreal)(numNeurons);
    }

    // Noise current for noise neuron
    noisyCurrent();


}


void NeuralNet_2::noisyCurrent()
{
    if(s->nn.noiseMode == "random")
    {
        if(numNeurons > 2 && s->randNumber(0,1) < noiseCurrent)
            recLayer->currents[Neuron::noise] += curRecFactor;
    }
    else if(s->nn.noiseMode == "initial")
    {
        if(time == 1)
            recLayer->currents[Neuron::noise] += curRecFactor;
    }
}



///////////////////////////////
// NeuralNet_zigzag
///////////////////////////////
NeuralNet_zigzag::NeuralNet_zigzag(Settings* settings)
    : NeuralNet(settings),
      numRecNeurons(s->nn.numberNeurons),
      curRecFactor(s->nn.curRecFactor)
{
    recLayer = s->createLayer(s, numRecNeurons);
    layers.append(recLayer);

    recLayer->neuronNames += "Left Motor Neuron";
    recLayer->neuronColors += Qt::blue;
    recLayer->neuronNames += "Right Motor Neuron";
    recLayer->neuronColors += Qt::magenta;
    for(int i=2; i < recLayer->numNeurons; ++i)
    {
        recLayer->neuronNames += "Hidden Neuron " + QString::number(i-1);
        recLayer->neuronColors += Qt::black;
    }
}


/*virtual*/ void NeuralNet_zigzag::initializeWeights()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Weights between fully recurrent neurons
        QVector<qreal> aNeuronWeightsRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronWeightsRec.append(s->randNumber(minWeight,maxWeight));
        weightsRec.append(aNeuronWeightsRec);
    }

    weights.append(&weightsRec);
}


/*virtual*/ void NeuralNet_zigzag::initializeDelays()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        // Delays between fully recurrent neurons
        QVector<int> aNeuronDelaysRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronDelaysRec.append(s->randInteger(0,maxDelayTime));
        delaysRec.append(aNeuronDelaysRec);
    }

    delays.append(&delaysRec);
}


/*virtual*/ void NeuralNet_zigzag::computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity)
{
    // Compute the current received by the neurons
    for(int k = 0; k<numRecNeurons; ++k)
    {
        qreal sum = 0;
        for(int i = 0; i < numRecNeurons; ++i)
            sum += recLayer->firings[i][delaysRec[k][i]]*weightsRec[k][i] * curRecFactor;

        recLayer->currents[k] += sum;

        // Noise current
        if(s->randNumber(0,1) < noiseCurrent)
            recLayer->currents[k] += curRecFactor;

        // Normalization
        recLayer->currents[k] /= (qreal)(numNeurons);
    }
}



///////////////////////////////
// NeuralNet_CTRNN
///////////////////////////////
NeuralNet_CTRNN::NeuralNet_CTRNN(Settings* settings)
    : NeuralNet(settings),
      numRecNeurons(s->nn.numberNeurons),
      numSensors(s->nn.numberSensors),
      curRecFactor(s->nn.curRecFactor),
      curSensFactor(s->nn.curSensFactor)
{
    recLayer = new Layer_CTRNN(s, numRecNeurons);
    layers.append(recLayer);
    recLayer->neuronNames += "Left Motor Neuron";
    recLayer->neuronColors += Qt::blue;
    recLayer->neuronNames += "Right Motor Neuron";
    recLayer->neuronColors += Qt::magenta;

    for(int i=2; i < recLayer->numNeurons; ++i)
    {
        recLayer->neuronNames += "Hidden Neuron " + QString::number(i-1);
        recLayer->neuronColors += Qt::black;
    }
}


/*virtual*/ void NeuralNet_CTRNN::initializeWeights()
{
    for(int i=0; i<numRecNeurons; ++i)
    {
        QVector<qreal> aNeuronWeightsRec;
        for(int k=0; k<numRecNeurons; ++k)
            aNeuronWeightsRec.append(s->randNumber(minWeight,maxWeight));
        weightsRR.append(aNeuronWeightsRec);

        QVector<qreal> aNeuronWeightsSensors;
        for(int k=0; k<numSensors; ++k)
            aNeuronWeightsSensors.append(s->randNumber(minWeight,maxWeight));
        weightsRS.append(aNeuronWeightsSensors);
    }

    weights.append(&weightsRR);
    weights.append(&weightsRS);
}


/*virtual*/ void NeuralNet_CTRNN::computeCurrents(const QVector<qreal> sensorsInputs, const qreal maxIntensity)
{

    // Compute the current received by the rec neurons
    for(int k = 0; k<numRecNeurons; ++k)
    {
        qreal sum = 0;
        for(int i = 0; i < numRecNeurons; ++i)
            sum += weightsRR[k][i] * s->sigmoid(recLayer->potentials[i] + recLayer->bias[i]);

        recLayer->internalCurrents[k] = sum;


        // Compute the current received by the sensors neurons
        qreal sum2 = 0;
        for(int i = 0; i < numSensors; ++i)
            sum2 += weightsRS[k][i]* sensorsInputs[i];

        recLayer->externalCurrents[k] = sum2;
    }
}












///////////////////////////////
// NeuralNet_LIF
///////////////////////////////
//NeuralNet_LIF::NeuralNet_LIF(Settings* settings)
//    : NeuralNet(settings)
//{
//    potentialResting = s->nn.potResting_lif;
//    potentialThreshold = s->nn.potThreshold_lif;
//    potSpiking = s->nn.potSpiking_lif;
//}


///*virtual*/ void NeuralNet_LIF::initSpecificParameters()
//{
//    // Memory factors of each neuron
//    memoryFactors.append(s->randNumber(-0.5,1));
//}


//// Current is between 0 and 1*curFactor
///*virtual*/ qreal NeuralNet_LIF::nextPotential(int neuron, qreal prevPot, qreal current) const
//{
//    return prevPot
//            + dt * 4 * curFactor * current
//            + dt * (1/5.0) * memoryFactors[neuron] * (potentialResting - prevPot);
//}


///*virtual*/ void NeuralNet_LIF::appendSpecificParameters(QVector<qreal>* pGenome) const
//{
//    for(int i = 0; i < memoryFactors.size(); ++i)
//        pGenome->append(memoryFactors[i]);
//}


///*virtual*/ void NeuralNet_LIF::updateSpecificParameters(const QVector<qreal>* pGenome, int pt)
//{
//    for(int i=0; i<numNeurons; ++i)
//        memoryFactors[i] = pGenome->at(pt++);
//}


///*virtual*/ int NeuralNet_LIF::getNumParameters() const
//{
//    return NeuralNet::getNumParameters() + memoryFactors.size();
//}


//void NeuralNet_LIF::writeOut(QDataStream & out)
//{
//    NeuralNet::writeOut(out);
//    out << memoryFactors;
//}


//void NeuralNet_LIF::writeIn(QDataStream & in)
//{
//    NeuralNet::writeIn(in);
//    in >> memoryFactors;
//}

