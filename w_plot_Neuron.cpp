#include <math.h>
#include <qwt_legend_item.h>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <QVBoxLayout>
#include <QStringList>

#include "graph_Simulation.h"
#include "h_Simulation.h"
#include "h_Robot.h"
#include "NeuralNet.h"

#include "w_plot_Neuron.h"


//////////////////////////////////////////
// w_plot_Neuron
//////////////////////////////////////////

w_plot_Neuron::w_plot_Neuron(QWidget *parent)
    : QWidget(parent),
      numberPoints(0)
{
}


w_plot_Neuron::~w_plot_Neuron()
{
    foreach(QwtPlotCurve* curve, curves)
        delete curve;
    delete plot;
}


/*virtual*/ QSize w_plot_Neuron::sizeHint () const
{
    QSize a(500,100);
    return a;
}


void w_plot_Neuron::init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation)
{
    mainWindow = aMainWindow;
    graph_sim = aGraph_simulation;
    bestNet = graph_sim->getSim()->getRobots()[0]->getNeuralNet();

    setNumPoints();

    layout = new QVBoxLayout(this);

    plot = new QwtPlot(this);

    initData();

    setupPlot();
    setupCurves();

    layout->addWidget(plot);
    plot->replot();
}


void w_plot_Neuron::setupCurves()
{
    foreach(QwtPlotCurve* curve, curves)
    {
        curve->setRenderHint(QwtPlotCurve::RenderAntialiased, true);
        showCurve(curve, false);
    }
}


void w_plot_Neuron::setupPlot()
{
    //plot->setFixedHeight(400);

    plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    plot->setCanvasBackground(QColor(Qt::white));

    alignScales();

    plot->setAutoReplot(false);

    connect(plot, SIGNAL(legendChecked(QwtPlotItem *, bool)),
            this, SLOT(showCurve(QwtPlotItem *, bool)));
}


void w_plot_Neuron::alignScales()
{
    // The code below shows how to align the scales to
    // the canvas frame, but is also a good example demonstrating
    // why the spreaded API needs polishing.

    plot->canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
    plot->canvas()->setLineWidth(1);

    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)plot->axisWidget(i);
        if ( scaleWidget )
            scaleWidget->setMargin(0);

        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)plot->axisScaleDraw(i);
        if ( scaleDraw )
            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
    }

    // Remove this line if you want to refresh the scale in real time
    plot->plotLayout()->setAlignCanvasToScales(true);
}


/*virtual*/ void w_plot_Neuron::initData()
{
    int pt = 0;

    foreach(Layer* layer, bestNet->getLayers())
    {
        QStringList neuronNames = layer->getNeuronNames();
        QList<int> neuronColors = layer->getNeuronColors();

        for(int i = 0; i < layer->getNumNeurons(); ++i)
        {
            initSamples();

            datas.append(new QwtPointSeriesData);
            datas[pt]->setSamples(samples[pt]);

            curves.append(new QwtPlotCurve(neuronNames.at(i)));
            curves.last()->setPen(QPen(Qt::GlobalColor(neuronColors.at(i)), 1));
            curves[pt]->setData(datas[pt]);
            curves[pt]->attach(plot);

            pt++;
        }
    }
}


void w_plot_Neuron::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = plot->legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);

    plot->replot();
}






//////////////////////////////////////////
// w_plot_MFR
//////////////////////////////////////////

w_plot_MFR::w_plot_MFR(QWidget *parent)
    : w_plot_Neuron(parent)
{
}


/*virtual*/ w_plot_MFR::~w_plot_MFR()
{
    delete legend;
}


/*virtual*/ void w_plot_MFR::init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation)
{
    w_plot_Neuron::init(aMainWindow, aGraph_simulation);

    QObject::connect(graph_sim, SIGNAL(sendIteration(int)),
                     this, SLOT(refreshPlot()));
}


/*virtual*/ void w_plot_MFR::initSamples()
{
    QVector<QPointF> buff;
    for(int k=-numberPoints; k < 0; ++k)
        buff.append(QPointF(k,0));
    samples.append(buff);
}


/*virtual*/ void w_plot_MFR::setNumPoints()
{
    numberPoints = 40;
}

/*virtual*/ void w_plot_MFR::setupPlot()
{
    w_plot_Neuron::setupPlot();

    plot->setTitle("Mean Firing rates");

    // legend
    legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    plot->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    plot->setAxisTitle(QwtPlot::xBottom, "Last iterations");
    plot->setAxisScale(QwtPlot::xBottom, -numberPoints, -1);

    plot->setAxisTitle(QwtPlot::yLeft, "Values");
    if(bestNet->getMotorLayer()->isCTRNN())
        plot->setAxisScale(QwtPlot::yLeft, -21, 21);
    else
        plot->setAxisScale(QwtPlot::yLeft, -0.1, 1.1);
}


/*virtual*/ void w_plot_MFR::refreshPlot()
{
    int pt = 0;

    foreach(Layer* layer, bestNet->getLayers())
        for(int i = 0; i < layer->getNumNeurons(); ++i)
            refreshData(pt++, i, layer);

    plot->replot();
}


/*virtual*/ void w_plot_MFR::refreshData(int pt, int neuron, const Layer* pL)
{
    for(int k=0; k < numberPoints-1; ++k)
        samples[pt][k].setY(samples[pt][k+1].y());
    samples[pt][numberPoints-1].setY(pL->getPlotDisplay1(neuron));

    datas[pt]->setSamples(samples[pt]);
    curves[pt]->setData(datas[pt]);
}




//////////////////////////////////////////
// w_plot_Pot
//////////////////////////////////////////

w_plot_Pot::w_plot_Pot(QWidget *parent)
    : w_plot_Neuron(parent)
{
}


/*virtual*/ w_plot_Pot::~w_plot_Pot()
{
    delete legend;
}


void w_plot_Pot::init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation)
{
    w_plot_Neuron::init(aMainWindow, aGraph_simulation);

    QObject::connect(graph_sim, SIGNAL(sendIteration(int)),
                     this, SLOT(refreshPlot()));
}


/*virtual*/ void w_plot_Pot::initSamples()
{
    QVector<QPointF> buff;
    for(int k=0; k < numberPoints; ++k)
        buff.append(QPointF(k,0));
    samples.append(buff);
}


/*virtual*/ void w_plot_Pot::setNumPoints()
{
    numberPoints = bestNet->getStepTime();
}


/*virtual*/ void w_plot_Pot::setupPlot()
{
    plot->setTitle("Potentials");
    w_plot_Neuron::setupPlot();

    // legend
    legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    plot->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    plot->setAxisTitle(QwtPlot::xBottom, "Last iterations");
    plot->setAxisScale(QwtPlot::xBottom, 0, bestNet->getStepTime());


    Layer* aLayer = bestNet->getLayers().first();
    plot->setAxisTitle(QwtPlot::yLeft, "Values");
    if(aLayer->isSpiking())
    {
        Layer_Spiking* aSpikingLayer = dynamic_cast<Layer_Spiking*>(aLayer);
        plot->setAxisScale(QwtPlot::yLeft, aSpikingLayer->getPotResting()-10, aSpikingLayer->getPotSpiking()+10);
    }
    else
        plot->setAxisScale(QwtPlot::yLeft, -100, +100);
}



/*virtual*/ void w_plot_Pot::refreshPlot()
{
    int pt = 0;

    foreach(Layer* layer, bestNet->getLayers())
        for(int i = 0; i < layer->getNumNeurons(); ++i)
            refreshData(pt++, i, layer);

    plot->replot();
}


/*virtual*/ void w_plot_Pot::refreshData(int pt, int neuron, const Layer* pL)
{
    if(!pL->isSpiking())
        return;

    for(int k=0; k < numberPoints; ++k)
        samples[pt][k].setY(pL->getPlotDisplay2(neuron)->at(bestNet->getStepTime()-1-k));

    datas[pt]->setSamples(samples[pt]);
    curves[pt]->setData(datas[pt]);
}


