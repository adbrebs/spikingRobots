#include <QFileDialog>

#include "graph_Simulation.h"
#include "graph_Scene.h"
#include "h_Simulation.h"

#include "w_widget_Simulation.h"
#include "ui_w_widget_Simulation.h"

w_widget_Simulation::w_widget_Simulation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::w_widget_Simulation)
{
    ui->setupUi(this);
}

w_widget_Simulation::~w_widget_Simulation()
{
    delete ui;
}


void w_widget_Simulation::init(w_MainWindow* aMainWindow, h_Simulation* aH_simulation, graph_Simulation* aGraph_simulation)
{
    mainWindow = aMainWindow;
    h_simulation = aH_simulation;
    graph_sim = aGraph_simulation;

    // Init the two plots
    ui->w_plot_mfr->init(mainWindow, graph_sim);
    ui->w_plot_mfr->setFixedHeight(450);
    ui->w_plot_pot->init(mainWindow, graph_sim);
    ui->w_plot_pot->setFixedHeight(450);

    // Init the view (the world)
    ui->w_World->init(graph_sim);

    // Connect the scene to the view
    setScene();

    qreal wid = 800 + 2;
    qreal hei = 800 + 2;
    ui->w_World->setFixedSize(wid, hei);
    ui->w_slider_ite->setValue(graph_sim->getTimeOutInterval());
    ui->w_spinBox_time->setValue(graph_sim->getTimeOutInterval());

    QObject::connect(graph_sim, SIGNAL(sendIteration(int)),
                     this, SLOT(updateIterationValue(int)));
    QObject::connect(ui->w_slider_ite, SIGNAL(valueChanged(int)),
                     this, SLOT(updateTimeOut(int)));
    QObject::connect(ui->w_but_pause, SIGNAL(clicked(bool)),
                     this, SLOT(pauseButton(bool)));
    QObject::connect(ui->w_but_load, SIGNAL(clicked()),
                     this, SLOT(loadBestNetFromFile()));
    QObject::connect(ui->w_radbut_food, SIGNAL(toggled(bool)),
                     this, SLOT(addFood(bool)));
    QObject::connect(ui->w_radbut_pois, SIGNAL(toggled(bool)),
                     this, SLOT(addPoison(bool)));
    QObject::connect(ui->w_but_clearItems, SIGNAL(clicked()),
                     this, SLOT(clearAllItems()));
    QObject::connect(ui->w_but_resetItems, SIGNAL(clicked()),
                     this, SLOT(reInitAllItems()));
    QObject::connect(ui->w_but_resetRobots, SIGNAL(clicked()),
                     this, SLOT(reIniRobotsPos()));
    QObject::connect(ui->w_spinBox_path, SIGNAL(valueChanged(int)),
                     this, SLOT(changePathLength(int)));
    QObject::connect(ui->w_but_saveView, SIGNAL(clicked()),
                     this, SLOT(saveWholeViewToFile()));
    QObject::connect(ui->w_but_saveCurView, SIGNAL(clicked()),
                     this, SLOT(saveCurViewToFile()));
}


/*slot*/ void w_widget_Simulation::updateIterationValue(int iteration)
{
    ui->w_label_iterations->setText(QString::number(iteration));
}


/*slot*/ void w_widget_Simulation::updateTimeOut(int time)
{
    graph_sim->setTimeOutInterval(time);
}


/*slot*/ void w_widget_Simulation::pauseButton(bool var)
{
    if(var)
    {
        pause();
        ui->w_but_pause->setText("Resume");
    }
    else
    {
        resume();
        ui->w_but_pause->setText("Pause");
    }

}


/*slot*/ void w_widget_Simulation::pause()
{
    graph_sim->pauseSim();
}


/*slot*/ void w_widget_Simulation::resume()
{
    graph_sim->resumeSim();
}


void w_widget_Simulation::setScene()
{
    ui->w_World->setScene(graph_sim->getScene());
}


/*slots*/ void w_widget_Simulation::loadBestNetFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load Network File"), "",
                                                    tr("Network file (*.net);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
        h_simulation->updateRobotFromFile(fileName);
}


void w_widget_Simulation::addFood(bool checked)
{
    if(checked)
        ui->w_World->setFoodAddMode();

    ui->w_radbut_pois->setChecked(false);
}


void w_widget_Simulation::addPoison(bool checked)
{
    if(checked)
        ui->w_World->setPoisAddMode();

    ui->w_radbut_food->setChecked(false);
}


void w_widget_Simulation::clearAllItems()
{
    ui->w_World->clearAllItems();
}


void w_widget_Simulation::reInitAllItems()
{
    ui->w_World->reInitializeAllItems();
}


void w_widget_Simulation::reIniRobotsPos()
{
    ui->w_World->reIniRobotsPos();
}


void w_widget_Simulation::changePathLength(int newLength)
{
    graph_sim->changeRobotsPathLengths(newLength);
}


/*slots*/ void w_widget_Simulation::saveWholeViewToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Whole View"), "",
                                                    tr("images (*.svg);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
    {
        graph_sim->getScene()->saveSVGOnFile(fileName);
        //graph_sim->getScene()->saveOnFile(fileName);
    }
}


/*slots*/ void w_widget_Simulation::saveCurViewToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Current View"), "",
                                                    tr("images (*.png);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else
    {
        QPixmap pixMap = QPixmap::grabWidget(ui->w_World);
        pixMap.save(fileName);
    }
}
