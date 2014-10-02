#ifndef W_PLOT_MFR_H
#define W_PLOT_MFR_H

class QwtPlotItem;
class QwtPlotCurve;
class QwtLegend;
class QwtPlot;
class QwtPointSeriesData;
class QVBoxLayout;

class w_MainWindow;

class Layer;
class NeuralNet;
class graph_Simulation;

#include <QWidget>


class w_plot_Neuron : public QWidget
{
    Q_OBJECT
public:
    explicit w_plot_Neuron(QWidget *parent = 0);
    virtual ~w_plot_Neuron();

    /*virtual*/ QSize sizeHint () const;

    virtual void init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation); 
    virtual void setNumPoints() {}
    
protected:
    void alignScales();
    virtual void setupPlot();
    void setupCurves();
    virtual void initData();
    virtual void initSamples() {}

signals:
    
public slots:
    virtual void refreshPlot() = 0;
    void showCurve(QwtPlotItem *, bool on);

protected:
    virtual void refreshData(int pt, int neuron, const Layer *pL) {}

protected:
    w_MainWindow* mainWindow;
    graph_Simulation* graph_sim;
    NeuralNet* bestNet;

    QVBoxLayout *layout;
    QwtPlot* plot;
    QVector<QwtPlotCurve*> curves;
    QwtLegend *legend;
    QVector<QwtPointSeriesData*> datas;
    QVector< QVector<QPointF> > samples;

    int numberPoints;
};






class w_plot_MFR : public w_plot_Neuron
{
    Q_OBJECT
public:
    explicit w_plot_MFR(QWidget *parent = 0);
    /*virtual*/ ~w_plot_MFR();
    /*virtual*/ void init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation);
    /*virtual*/ void setNumPoints();


protected:
    /*virtual*/ void setupPlot();
    /*virtual*/ void initSamples();

public slots:
    /*virtual*/ void refreshPlot();

protected:
    /*virtual*/ void refreshData(int pt, int neuron, const Layer *pL);
};


class w_plot_Pot : public w_plot_Neuron
{
    Q_OBJECT
public:
    explicit w_plot_Pot(QWidget *parent = 0);
    /*virtual*/ ~w_plot_Pot();
    /*virtual*/ void init(w_MainWindow* aMainWindow, graph_Simulation* aGraph_simulation);
    /*virtual*/ void setNumPoints();


protected:
    /*virtual*/ void setupPlot();
    /*virtual*/ void initSamples();

public slots:
    /*virtual*/ void refreshPlot();

protected:
    /*virtual*/ void refreshData(int pt, int neuron, const Layer *pL);

private:
};

#endif // W_PLOT_MFR_H
