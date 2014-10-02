/********************************************************************************
** Form generated from reading UI file 'w_widget_Simulation.ui'
**
** Created: Wed 5. Sep 22:07:29 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_W_WIDGET_SIMULATION_H
#define UI_W_WIDGET_SIMULATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "w_plot_Neuron.h"
#include "w_view_World.h"

QT_BEGIN_NAMESPACE

class Ui_w_widget_Simulation
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *w_spinBox_time;
    QSlider *w_slider_ite;
    QFrame *line;
    QPushButton *w_but_pause;
    QFrame *line_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *w_label_iterations;
    QFrame *line_7;
    QVBoxLayout *verticalLayout_4;
    QPushButton *w_but_saveView;
    QPushButton *w_but_saveCurView;
    QFrame *line_3;
    w_view_World *w_World;
    QHBoxLayout *horizontalLayout_4;
    QSpinBox *w_spinBox_path;
    QFrame *line_6;
    QRadioButton *w_radbut_food;
    QRadioButton *w_radbut_pois;
    QPushButton *w_but_clearItems;
    QPushButton *w_but_resetItems;
    QPushButton *w_but_resetRobots;
    QFrame *line_4;
    QPushButton *w_but_load;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    w_plot_MFR *w_plot_mfr;
    QFrame *line_5;
    w_plot_Pot *w_plot_pot;

    void setupUi(QWidget *w_widget_Simulation)
    {
        if (w_widget_Simulation->objectName().isEmpty())
            w_widget_Simulation->setObjectName(QString::fromUtf8("w_widget_Simulation"));
        w_widget_Simulation->resize(896, 378);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w_widget_Simulation->sizePolicy().hasHeightForWidth());
        w_widget_Simulation->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(w_widget_Simulation);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_3->setContentsMargins(-1, 0, -1, -1);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(10, -1, 0, -1);
        groupBox = new QGroupBox(w_widget_Simulation);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        w_spinBox_time = new QSpinBox(groupBox);
        w_spinBox_time->setObjectName(QString::fromUtf8("w_spinBox_time"));
        w_spinBox_time->setAlignment(Qt::AlignCenter);
        w_spinBox_time->setMinimum(1);
        w_spinBox_time->setMaximum(500);

        horizontalLayout_2->addWidget(w_spinBox_time);

        w_slider_ite = new QSlider(groupBox);
        w_slider_ite->setObjectName(QString::fromUtf8("w_slider_ite"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(w_slider_ite->sizePolicy().hasHeightForWidth());
        w_slider_ite->setSizePolicy(sizePolicy2);
        w_slider_ite->setMinimumSize(QSize(200, 0));
        w_slider_ite->setMaximumSize(QSize(300, 20));
        w_slider_ite->setMinimum(1);
        w_slider_ite->setMaximum(500);
        w_slider_ite->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(w_slider_ite);


        horizontalLayout_3->addWidget(groupBox);

        line = new QFrame(w_widget_Simulation);
        line->setObjectName(QString::fromUtf8("line"));
        sizePolicy2.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy2);
        line->setMinimumSize(QSize(0, 30));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line);

        w_but_pause = new QPushButton(w_widget_Simulation);
        w_but_pause->setObjectName(QString::fromUtf8("w_but_pause"));
        w_but_pause->setCheckable(true);

        horizontalLayout_3->addWidget(w_but_pause);

        line_2 = new QFrame(w_widget_Simulation);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        sizePolicy2.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy2);
        line_2->setMinimumSize(QSize(0, 30));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, -1, -1, -1);
        label = new QLabel(w_widget_Simulation);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        w_label_iterations = new QLabel(w_widget_Simulation);
        w_label_iterations->setObjectName(QString::fromUtf8("w_label_iterations"));
        w_label_iterations->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(w_label_iterations);


        horizontalLayout_3->addLayout(verticalLayout);

        line_7 = new QFrame(w_widget_Simulation);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        sizePolicy2.setHeightForWidth(line_7->sizePolicy().hasHeightForWidth());
        line_7->setSizePolicy(sizePolicy2);
        line_7->setMinimumSize(QSize(0, 30));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line_7);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        w_but_saveView = new QPushButton(w_widget_Simulation);
        w_but_saveView->setObjectName(QString::fromUtf8("w_but_saveView"));

        verticalLayout_4->addWidget(w_but_saveView);

        w_but_saveCurView = new QPushButton(w_widget_Simulation);
        w_but_saveCurView->setObjectName(QString::fromUtf8("w_but_saveCurView"));

        verticalLayout_4->addWidget(w_but_saveCurView);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_3->addLayout(horizontalLayout_3);

        line_3 = new QFrame(w_widget_Simulation);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_3);

        w_World = new w_view_World(w_widget_Simulation);
        w_World->setObjectName(QString::fromUtf8("w_World"));
        sizePolicy1.setHeightForWidth(w_World->sizePolicy().hasHeightForWidth());
        w_World->setSizePolicy(sizePolicy1);
        w_World->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
        w_World->setDragMode(QGraphicsView::ScrollHandDrag);
        w_World->setCacheMode(QGraphicsView::CacheBackground);
        w_World->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

        verticalLayout_3->addWidget(w_World);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        w_spinBox_path = new QSpinBox(w_widget_Simulation);
        w_spinBox_path->setObjectName(QString::fromUtf8("w_spinBox_path"));
        w_spinBox_path->setMaximum(20000);
        w_spinBox_path->setValue(10);

        horizontalLayout_4->addWidget(w_spinBox_path);

        line_6 = new QFrame(w_widget_Simulation);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_6);

        w_radbut_food = new QRadioButton(w_widget_Simulation);
        w_radbut_food->setObjectName(QString::fromUtf8("w_radbut_food"));
        sizePolicy2.setHeightForWidth(w_radbut_food->sizePolicy().hasHeightForWidth());
        w_radbut_food->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_radbut_food);

        w_radbut_pois = new QRadioButton(w_widget_Simulation);
        w_radbut_pois->setObjectName(QString::fromUtf8("w_radbut_pois"));
        sizePolicy2.setHeightForWidth(w_radbut_pois->sizePolicy().hasHeightForWidth());
        w_radbut_pois->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_radbut_pois);

        w_but_clearItems = new QPushButton(w_widget_Simulation);
        w_but_clearItems->setObjectName(QString::fromUtf8("w_but_clearItems"));
        sizePolicy2.setHeightForWidth(w_but_clearItems->sizePolicy().hasHeightForWidth());
        w_but_clearItems->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_but_clearItems);

        w_but_resetItems = new QPushButton(w_widget_Simulation);
        w_but_resetItems->setObjectName(QString::fromUtf8("w_but_resetItems"));
        sizePolicy2.setHeightForWidth(w_but_resetItems->sizePolicy().hasHeightForWidth());
        w_but_resetItems->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_but_resetItems);

        w_but_resetRobots = new QPushButton(w_widget_Simulation);
        w_but_resetRobots->setObjectName(QString::fromUtf8("w_but_resetRobots"));
        sizePolicy2.setHeightForWidth(w_but_resetRobots->sizePolicy().hasHeightForWidth());
        w_but_resetRobots->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_but_resetRobots);

        line_4 = new QFrame(w_widget_Simulation);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_4);

        w_but_load = new QPushButton(w_widget_Simulation);
        w_but_load->setObjectName(QString::fromUtf8("w_but_load"));
        sizePolicy2.setHeightForWidth(w_but_load->sizePolicy().hasHeightForWidth());
        w_but_load->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(w_but_load);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(10, -1, -1, -1);
        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        w_plot_mfr = new w_plot_MFR(w_widget_Simulation);
        w_plot_mfr->setObjectName(QString::fromUtf8("w_plot_mfr"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(w_plot_mfr->sizePolicy().hasHeightForWidth());
        w_plot_mfr->setSizePolicy(sizePolicy3);

        verticalLayout_2->addWidget(w_plot_mfr);

        line_5 = new QFrame(w_widget_Simulation);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_5);

        w_plot_pot = new w_plot_Pot(w_widget_Simulation);
        w_plot_pot->setObjectName(QString::fromUtf8("w_plot_pot"));
        sizePolicy3.setHeightForWidth(w_plot_pot->sizePolicy().hasHeightForWidth());
        w_plot_pot->setSizePolicy(sizePolicy3);

        verticalLayout_2->addWidget(w_plot_pot);


        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(w_widget_Simulation);
        QObject::connect(w_spinBox_time, SIGNAL(valueChanged(int)), w_slider_ite, SLOT(setValue(int)));
        QObject::connect(w_slider_ite, SIGNAL(valueChanged(int)), w_spinBox_time, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(w_widget_Simulation);
    } // setupUi

    void retranslateUi(QWidget *w_widget_Simulation)
    {
        w_widget_Simulation->setWindowTitle(QApplication::translate("w_widget_Simulation", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("w_widget_Simulation", "Speed Control", 0, QApplication::UnicodeUTF8));
        w_spinBox_time->setSuffix(QApplication::translate("w_widget_Simulation", " msec", 0, QApplication::UnicodeUTF8));
        w_but_pause->setText(QApplication::translate("w_widget_Simulation", "Pause", 0, QApplication::UnicodeUTF8));
        w_but_pause->setShortcut(QApplication::translate("w_widget_Simulation", "P", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("w_widget_Simulation", "Number of iterations:", 0, QApplication::UnicodeUTF8));
        w_label_iterations->setText(QApplication::translate("w_widget_Simulation", "0", 0, QApplication::UnicodeUTF8));
        w_but_saveView->setText(QApplication::translate("w_widget_Simulation", "Save whole View...", 0, QApplication::UnicodeUTF8));
        w_but_saveCurView->setText(QApplication::translate("w_widget_Simulation", "Save current Zoom...", 0, QApplication::UnicodeUTF8));
        w_spinBox_path->setPrefix(QApplication::translate("w_widget_Simulation", "trail length: ", 0, QApplication::UnicodeUTF8));
        w_radbut_food->setText(QApplication::translate("w_widget_Simulation", "Add food", 0, QApplication::UnicodeUTF8));
        w_radbut_pois->setText(QApplication::translate("w_widget_Simulation", "Add poison", 0, QApplication::UnicodeUTF8));
        w_but_clearItems->setText(QApplication::translate("w_widget_Simulation", "Clear all items", 0, QApplication::UnicodeUTF8));
        w_but_resetItems->setText(QApplication::translate("w_widget_Simulation", "Reset Items", 0, QApplication::UnicodeUTF8));
        w_but_resetRobots->setText(QApplication::translate("w_widget_Simulation", "Reset Robots", 0, QApplication::UnicodeUTF8));
        w_but_load->setText(QApplication::translate("w_widget_Simulation", "Load neural network...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class w_widget_Simulation: public Ui_w_widget_Simulation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_W_WIDGET_SIMULATION_H
