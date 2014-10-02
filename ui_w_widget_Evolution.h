/********************************************************************************
** Form generated from reading UI file 'w_widget_Evolution.ui'
**
** Created: Sat 25. Aug 12:49:08 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_WIDGET_EVOLUTION_H
#define UI_W_WIDGET_EVOLUTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "w_plot_Evolution.h"

QT_BEGIN_NAMESPACE

class Ui_w_widget_Evolution
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *w_but_Evolution;
    QSpinBox *w_spinBox_Evolution;
    QProgressBar *w_progress_Evolution;
    QFrame *line;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *w_label_ItEvo;
    QFrame *line_2;
    w_plot_Evolution *w_plot_Ev;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *w_but_saveGraph;
    QPushButton *w_but_saveEv;
    QPushButton *w_but_saveBestNet;

    void setupUi(QWidget *w_widget_Evolution)
    {
        if (w_widget_Evolution->objectName().isEmpty())
            w_widget_Evolution->setObjectName(QString::fromUtf8("w_widget_Evolution"));
        w_widget_Evolution->resize(564, 232);
        verticalLayout_2 = new QVBoxLayout(w_widget_Evolution);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(0, -1, 0, -1);
        w_but_Evolution = new QPushButton(w_widget_Evolution);
        w_but_Evolution->setObjectName(QString::fromUtf8("w_but_Evolution"));

        horizontalLayout_2->addWidget(w_but_Evolution);

        w_spinBox_Evolution = new QSpinBox(w_widget_Evolution);
        w_spinBox_Evolution->setObjectName(QString::fromUtf8("w_spinBox_Evolution"));
        w_spinBox_Evolution->setAlignment(Qt::AlignCenter);
        w_spinBox_Evolution->setMaximum(50001);
        w_spinBox_Evolution->setValue(20);

        horizontalLayout_2->addWidget(w_spinBox_Evolution);

        w_progress_Evolution = new QProgressBar(w_widget_Evolution);
        w_progress_Evolution->setObjectName(QString::fromUtf8("w_progress_Evolution"));
        w_progress_Evolution->setValue(0);

        horizontalLayout_2->addWidget(w_progress_Evolution);

        line = new QFrame(w_widget_Evolution);
        line->setObjectName(QString::fromUtf8("line"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy);
        line->setMinimumSize(QSize(0, 30));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(10, -1, -1, -1);
        label = new QLabel(w_widget_Evolution);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        w_label_ItEvo = new QLabel(w_widget_Evolution);
        w_label_ItEvo->setObjectName(QString::fromUtf8("w_label_ItEvo"));
        w_label_ItEvo->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(w_label_ItEvo);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        line_2 = new QFrame(w_widget_Evolution);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_2);

        w_plot_Ev = new w_plot_Evolution(w_widget_Evolution);
        w_plot_Ev->setObjectName(QString::fromUtf8("w_plot_Ev"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(w_plot_Ev->sizePolicy().hasHeightForWidth());
        w_plot_Ev->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(w_plot_Ev);

        line_3 = new QFrame(w_widget_Evolution);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        w_but_saveGraph = new QPushButton(w_widget_Evolution);
        w_but_saveGraph->setObjectName(QString::fromUtf8("w_but_saveGraph"));

        horizontalLayout->addWidget(w_but_saveGraph);

        w_but_saveEv = new QPushButton(w_widget_Evolution);
        w_but_saveEv->setObjectName(QString::fromUtf8("w_but_saveEv"));

        horizontalLayout->addWidget(w_but_saveEv);

        w_but_saveBestNet = new QPushButton(w_widget_Evolution);
        w_but_saveBestNet->setObjectName(QString::fromUtf8("w_but_saveBestNet"));

        horizontalLayout->addWidget(w_but_saveBestNet);


        verticalLayout_2->addLayout(horizontalLayout);

        QWidget::setTabOrder(w_but_Evolution, w_spinBox_Evolution);
        QWidget::setTabOrder(w_spinBox_Evolution, w_but_saveGraph);
        QWidget::setTabOrder(w_but_saveGraph, w_but_saveEv);
        QWidget::setTabOrder(w_but_saveEv, w_but_saveBestNet);

        retranslateUi(w_widget_Evolution);

        QMetaObject::connectSlotsByName(w_widget_Evolution);
    } // setupUi

    void retranslateUi(QWidget *w_widget_Evolution)
    {
        w_widget_Evolution->setWindowTitle(QApplication::translate("w_widget_Evolution", "Form", 0, QApplication::UnicodeUTF8));
        w_but_Evolution->setText(QApplication::translate("w_widget_Evolution", "Evolve", 0, QApplication::UnicodeUTF8));
        w_spinBox_Evolution->setSuffix(QApplication::translate("w_widget_Evolution", " iterations", 0, QApplication::UnicodeUTF8));
        w_spinBox_Evolution->setPrefix(QString());
        label->setText(QApplication::translate("w_widget_Evolution", "Total number of generations", 0, QApplication::UnicodeUTF8));
        w_label_ItEvo->setText(QApplication::translate("w_widget_Evolution", "0", 0, QApplication::UnicodeUTF8));
        w_but_saveGraph->setText(QApplication::translate("w_widget_Evolution", "Save Graph...", 0, QApplication::UnicodeUTF8));
        w_but_saveEv->setText(QApplication::translate("w_widget_Evolution", "Save Evolution...", 0, QApplication::UnicodeUTF8));
        w_but_saveBestNet->setText(QApplication::translate("w_widget_Evolution", "Save best evolved neural network...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_widget_Evolution: public Ui_w_widget_Evolution {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_WIDGET_EVOLUTION_H
