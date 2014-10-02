#include <math.h>
#include <qwt_legend_item.h>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_grid.h>
#include <QVBoxLayout>
#include <QFileDialog>

#include "Evolution.h"

#include "w_plot_Evolution.h"

w_plot_Evolution::w_plot_Evolution(QWidget *parent)
    : QWidget(parent),
      maxYAxis(1),
      minYAxis(0)
{
}


w_plot_Evolution::~w_plot_Evolution()
{
    foreach(QwtPlotCurve* curve, curves)
        delete curve;
    delete zoomer;
    delete grid;
    delete plot;
}


void w_plot_Evolution::init(w_MainWindow* aMainWindow, Evolution* anEvolution)
{
    mainWindow = aMainWindow;
    evolution = anEvolution;
    connect(evolution, SIGNAL(newFitness(int, const QVector< QVector<QPointF> >*)),
            this, SLOT(refreshPlot(int, const QVector< QVector<QPointF> >*)));

    layout = new QVBoxLayout(this);

    plot = new QwtPlot(this);
    layout->addWidget(plot);
    setupPlot();

    curves.append(new QwtPlotCurve("Mean individual fitness"));

    for(int i=0; i < curves.size(); ++i)
    {
        datas.append(new QwtPointSeriesData);
        curves[i]->setData(datas[i]);
        curves[i]->attach(plot);
    }
    setupCurves();

    zoomer = new QwtPlotZoomer(plot->canvas());

    grid = new QwtPlotGrid();
    grid->enableX(false); // On affiche seulement les lignes horizontales
    grid->setPen(QPen(Qt::lightGray, 0, Qt::DotLine));
    grid->attach(plot);

    plot->replot();
}


void w_plot_Evolution::setupCurves()
{
    foreach(QwtPlotCurve* curve, curves)
    {
        curve->setRenderHint(QwtPlotCurve::RenderAntialiased, true);
        showCurve(curve, true);
    }

    curves[0]->setPen(QPen(Qt::black, 1));
}


void w_plot_Evolution::setupPlot() {

    plot->setCanvasBackground(QColor(Qt::white));

    alignScales();

    plot->setAutoReplot(false);

    // In order to disable curves when clicking on the legend
    connect(plot, SIGNAL(legendChecked(QwtPlotItem *, bool)),
            this, SLOT(showCurve(QwtPlotItem *, bool)));

    plot->setTitle("Evolution of the mean fitness of an individual");
    // legend
    legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    plot->insertLegend(legend, QwtPlot::BottomLegend);

    // axis
    plot->setAxisTitle(QwtPlot::xBottom, "Iterations of the genetic algorithm");
    plot->setAxisScale(QwtPlot::xBottom, 0, 10);

    plot->setAxisTitle(QwtPlot::yLeft, "Values");
    plot->setAxisScale(QwtPlot::yLeft, minYAxis, maxYAxis);
}


void w_plot_Evolution::alignScales()
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


void w_plot_Evolution::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = plot->legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);

    plot->replot();
}


void w_plot_Evolution::refreshPlot(int numGeneration, const QVector< QVector<QPointF> >* pHistory)
{
    qreal fitness = pHistory->at(0).last().y();

    if(fitness > maxYAxis)
        maxYAxis = fitness + 1;

    if(fitness < minYAxis)
        minYAxis = fitness;

    plot->setAxisScale(QwtPlot::xBottom, 0, numGeneration + 1);
    plot->setAxisScale(QwtPlot::yLeft, minYAxis, maxYAxis);

    for(int i = 0; i < curves.size(); ++i)
    {
        datas[i]->setSamples(pHistory->at(i));
        curves[i]->setData(datas[i]);
    }

    // Set new axis to be the original ones
    zoomer->setZoomBase();

    plot->replot();
}


/*plot*/ void w_plot_Evolution::exportPlot()
{
    QStringList filter;
    filter += "PDF Documents (*.pdf)";
    filter += "SVG Documents (*.svg)";

    QString fileName = QFileDialog::getSaveFileName(
                parentWidget(), "Export File Name", "",
                filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);

    if ( !fileName.isEmpty() )
    {
        QwtPlotRenderer renderer;

        // flags to make the document look like the widget
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames, true);

        renderer.renderDocument(plot, fileName, QSizeF(300, 200), 85);
    }
}
