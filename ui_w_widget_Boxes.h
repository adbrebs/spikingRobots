/********************************************************************************
** Form generated from reading UI file 'w_widget_Boxes.ui'
**
** Created: Sat 25. Aug 12:49:08 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_WIDGET_BOXES_H
#define UI_W_WIDGET_BOXES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "w_widget_Evolution.h"
#include "w_widget_Simulation.h"

QT_BEGIN_NAMESPACE

class Ui_w_widget_Boxes
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *w_tabWidget;
    w_widget_Evolution *w_widget_Evo;
    w_widget_Simulation *w_widget_Sim;

    void setupUi(QWidget *w_widget_Boxes)
    {
        if (w_widget_Boxes->objectName().isEmpty())
            w_widget_Boxes->setObjectName(QString::fromUtf8("w_widget_Boxes"));
        w_widget_Boxes->resize(400, 295);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w_widget_Boxes->sizePolicy().hasHeightForWidth());
        w_widget_Boxes->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(w_widget_Boxes);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        w_tabWidget = new QTabWidget(w_widget_Boxes);
        w_tabWidget->setObjectName(QString::fromUtf8("w_tabWidget"));
        sizePolicy.setHeightForWidth(w_tabWidget->sizePolicy().hasHeightForWidth());
        w_tabWidget->setSizePolicy(sizePolicy);
        w_tabWidget->setTabPosition(QTabWidget::West);
        w_widget_Evo = new w_widget_Evolution();
        w_widget_Evo->setObjectName(QString::fromUtf8("w_widget_Evo"));
        w_tabWidget->addTab(w_widget_Evo, QString());
        w_widget_Sim = new w_widget_Simulation();
        w_widget_Sim->setObjectName(QString::fromUtf8("w_widget_Sim"));
        w_tabWidget->addTab(w_widget_Sim, QString());

        verticalLayout->addWidget(w_tabWidget);


        retranslateUi(w_widget_Boxes);

        w_tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(w_widget_Boxes);
    } // setupUi

    void retranslateUi(QWidget *w_widget_Boxes)
    {
        w_widget_Boxes->setWindowTitle(QApplication::translate("w_widget_Boxes", "Form", 0, QApplication::UnicodeUTF8));
        w_tabWidget->setTabText(w_tabWidget->indexOf(w_widget_Evo), QApplication::translate("w_widget_Boxes", "&Evolution control", 0, QApplication::UnicodeUTF8));
        w_tabWidget->setTabText(w_tabWidget->indexOf(w_widget_Sim), QApplication::translate("w_widget_Boxes", "&Simulation control", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_widget_Boxes: public Ui_w_widget_Boxes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_WIDGET_BOXES_H
