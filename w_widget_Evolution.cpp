#include <QMessageBox>
#include <QFileDialog>

#include "Evolution.h"
#include "w_MainWindow.h"


#include "w_widget_Evolution.h"
#include "ui_w_widget_Evolution.h"

w_widget_Evolution::w_widget_Evolution(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_widget_Evolution)
{
    ui->setupUi(this);
}

w_widget_Evolution::~w_widget_Evolution()
{
    delete ui;
}


void w_widget_Evolution::init(w_MainWindow* aMainWindow, Evolution* anEvolution)
{
    mainWindow = aMainWindow;
    evolution = anEvolution;

    ui->w_plot_Ev->init(mainWindow, evolution);

    QObject::connect(evolution, SIGNAL(nextGeneration(int, int, int)),
                     this, SLOT(updateGenWidgets(int, int, int)));
    QObject::connect(ui->w_but_Evolution, SIGNAL(clicked()),
                     this, SLOT(launchEvolution()));
    QObject::connect(ui->w_but_saveBestNet, SIGNAL(clicked()),
                     this, SLOT(saveBestNetToFile()));
    QObject::connect(ui->w_but_saveEv, SIGNAL(clicked()),
                     this, SLOT(saveEvolutionToFile()));
    QObject::connect(ui->w_but_saveGraph, SIGNAL(clicked()),
                     ui->w_plot_Ev, SLOT(exportPlot()));
}


/*slots*/ void w_widget_Evolution::updateGenWidgets(int value, int numSim, int numGeneration)
{
    ui->w_label_ItEvo->setText(QString::number(numGeneration));
    ui->w_progress_Evolution->setValue((100*(value+1))/numSim);
}


/*slots*/ void w_widget_Evolution::launchEvolution()
{
    mainWindow->launchEvolution(ui->w_spinBox_Evolution->value());
}


/*slots*/ void w_widget_Evolution::saveBestNetToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save the best evolved neural network"), "",
                                                    tr("Networks (*.net);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
        evolution->SaveBestNetworkOnFile(fileName);
}


/*slots*/ void w_widget_Evolution::saveEvolutionToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save evolution"), "",
                                                    tr("evolution files (*.ev);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
        evolution->SaveEvolutionOnFile(fileName);
}
