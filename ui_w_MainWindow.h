/********************************************************************************
** Form generated from reading UI file 'w_MainWindow.ui'
**
** Created: Sat 25. Aug 12:49:08 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_MAINWINDOW_H
#define UI_W_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "w_widget_Boxes.h"

QT_BEGIN_NAMESPACE

class Ui_w_MainWindow
{
public:
    QAction *action_Evolution;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    w_widget_Boxes *w_central_widget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *w_MainWindow)
    {
        if (w_MainWindow->objectName().isEmpty())
            w_MainWindow->setObjectName(QString::fromUtf8("w_MainWindow"));
        w_MainWindow->resize(400, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w_MainWindow->sizePolicy().hasHeightForWidth());
        w_MainWindow->setSizePolicy(sizePolicy);
        w_MainWindow->setDockNestingEnabled(false);
        w_MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        action_Evolution = new QAction(w_MainWindow);
        action_Evolution->setObjectName(QString::fromUtf8("action_Evolution"));
        action_Evolution->setCheckable(true);
        centralwidget = new QWidget(w_MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        w_central_widget = new w_widget_Boxes(centralwidget);
        w_central_widget->setObjectName(QString::fromUtf8("w_central_widget"));
        sizePolicy.setHeightForWidth(w_central_widget->sizePolicy().hasHeightForWidth());
        w_central_widget->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(w_central_widget);

        w_MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(w_MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 400, 25));
        w_MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(w_MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        w_MainWindow->setStatusBar(statusbar);

        retranslateUi(w_MainWindow);

        QMetaObject::connectSlotsByName(w_MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *w_MainWindow)
    {
        w_MainWindow->setWindowTitle(QApplication::translate("w_MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_Evolution->setText(QApplication::translate("w_MainWindow", "&Evolution...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_MainWindow: public Ui_w_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_MAINWINDOW_H
