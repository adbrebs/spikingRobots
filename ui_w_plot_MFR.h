/********************************************************************************
** Form generated from reading UI file 'w_plot_MFR.ui'
**
** Created: Sat 25. Aug 12:49:08 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_PLOT_MFR_H
#define UI_W_PLOT_MFR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_w_plot_MFR
{
public:

    void setupUi(QWidget *w_plot_MFR)
    {
        if (w_plot_MFR->objectName().isEmpty())
            w_plot_MFR->setObjectName(QString::fromUtf8("w_plot_MFR"));
        w_plot_MFR->resize(615, 379);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w_plot_MFR->sizePolicy().hasHeightForWidth());
        w_plot_MFR->setSizePolicy(sizePolicy);

        retranslateUi(w_plot_MFR);

        QMetaObject::connectSlotsByName(w_plot_MFR);
    } // setupUi

    void retranslateUi(QWidget *w_plot_MFR)
    {
        w_plot_MFR->setWindowTitle(QApplication::translate("w_plot_MFR", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_plot_MFR: public Ui_w_plot_MFR {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_PLOT_MFR_H
