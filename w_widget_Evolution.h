#ifndef W_WIDGET_EVOLUTION_H
#define W_WIDGET_EVOLUTION_H

class w_MainWindow;

class Evolution;

#include <QWidget>

namespace Ui {
class w_widget_Evolution;
}

class w_widget_Evolution : public QWidget
{
    Q_OBJECT
    
public:
    explicit w_widget_Evolution(QWidget *parent = 0);
    ~w_widget_Evolution();

    void init(w_MainWindow* aMainWindow, Evolution* anEvolution);
    
public slots:
    void updateGenWidgets(int value, int numSim, int numGeneration);
    void launchEvolution();
    void saveBestNetToFile();
    void saveEvolutionToFile();

private:
    void launchEvolution(int numSim);

private:
    Ui::w_widget_Evolution *ui;

    Evolution* evolution;
    w_MainWindow* mainWindow;
};

#endif // W_WIDGET_EVOLUTION_H
