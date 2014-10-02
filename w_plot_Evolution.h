#ifndef W_PLOT_EVOLUTION_H
#define W_PLOT_EVOLUTION_H

class QwtPlotItem;
class QwtPlotCurve;
class QwtLegend;
class QwtPlot;
class QwtPointSeriesData;
class QwtPlotZoomer;
class QwtPlotGrid;
class QVBoxLayout;

class w_MainWindow;
class Evolution;


#include <QWidget>

class w_plot_Evolution : public QWidget
{
    Q_OBJECT
public:
    explicit w_plot_Evolution(QWidget *parent = 0);
    ~w_plot_Evolution();

    void init(w_MainWindow* aMainWindow, Evolution* anEvolution);

private:
    void alignScales();
    void setupPlot();
    void setupCurves();

signals:

public slots:
    void refreshPlot(int numGeneration, const QVector<QVector<QPointF> > *pHistory);
    void showCurve(QwtPlotItem *, bool on);
    void exportPlot();

private:
    int maxYAxis;
    int minYAxis;

    w_MainWindow* mainWindow;
    Evolution* evolution;

    QVBoxLayout *layout;
    QwtPlot* plot;
    QVector< QwtPlotCurve* > curves;
    QwtLegend *legend;
    QVector<QwtPointSeriesData*> datas;
    QVector< QVector<QPointF> > samples;
    QwtPlotZoomer* zoomer;
    QwtPlotGrid* grid;
};

#endif // W_PLOT_EVOLUTION_H
