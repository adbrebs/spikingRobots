#include <QApplication>
#include <QCoreApplication>
#include <QTime>
#include <QDesktopWidget>
#include <QSettings>
#include <QUuid>
#include <QHash>

#include "Settings.h"
#include "Evolution.h"
#include "h_Simulation.h"
#include "Condor.h"

#include "w_MainWindow.h"


void condorMode(Settings* s);


int main(int argc, char **argv)
{
    QSettings qSettings("./results/config.ini", QSettings::IniFormat);
    Settings* s = new Settings(&qSettings);

    switch(s->appMode)
    {
    case 0:
    {
        ///////////////////////////////////////////////////////
        // Graphical Mode
        ///////////////////////////////////////////////////////
        QApplication app(argc, argv);

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        w_MainWindow fenetre(s);

        // Move it to the center of the screen
//        QRect screenGeometry = QApplication::desktop()->screenGeometry();
//        int x = (screenGeometry.width()-fenetre.width()) / 6;
//        int y = (screenGeometry.height()-fenetre.height()) / 6;
//        fenetre.move(x, y);
//        fenetre.show();
        fenetre.showMaximized();

        app.exec();

        break;
    }
    case 1:
    {
        ///////////////////////////////////////////////////////
        // Condor Mode
        ///////////////////////////////////////////////////////
        QCoreApplication app(argc, argv);

        // To give unique filenames to Condor files
        QUuid randd = QUuid::createUuid();
        int a = qHash(randd.toString());

        if(argc > 1)
            qsrand(a + atoi(argv[1])*1000 + 1);
        else
            qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        Condor cond(s);
        cond.launchTest();

        break;
    }
    }

    // Update .ini file
    s->writeSettingsToFile(&qSettings);
    delete s;

    return 0;
}
