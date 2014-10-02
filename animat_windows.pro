#INCLUDEPATH += /homes/ahd11/local/qwt/include
#LIBS += -L/homes/ahd11/local/qwt/lib -lqwt






#QWT_ROOT = /homes/ahd11/local/qwt/



#QMAKE_RPATHDIR *= $${QWT_ROOT}/lib

#contains(QWT_CONFIG, QwtFramework) {

#    LIBS      += -F$${QWT_ROOT}/lib
#}
#else {

#    LIBS      += -L$${QWT_ROOT}/lib
#}

#IPATH       = $${INCLUDEPATH}
#qtAddLibrary(qwt)
#INCLUDEPATH = $${IPATH}

#contains(QWT_CONFIG, QwtSvg) {

#    QT += svg
#}
#else {

#    DEFINES += QWT_NO_SVG
#}

CONFIG += qwt

HEADERS += \
    w_widget_Simulation.h \
    w_widget_Evolution.h \
    w_widget_Boxes.h \
    w_plot_Neuron.h \
    w_plot_Evolution.h \
    w_MainWindow.h \
    Settings.h \
    NeuralNet.h \
    Individual.h \
    h_World.h \
    h_Simulation.h \
    h_Robot.h \
    h_Item.h \
    h_EatableItem.h \
    graph_Simulation.h \
    graph_Scene.h \
    graph_Robot.h \
    graph_Item.h \
    graph_EatableItem.h \
    Evolution.h \
    w_view_World.h \
    Condor.h \
    h_Sensor.h

SOURCES += \
    w_widget_Simulation.cpp \
    w_widget_Evolution.cpp \
    w_widget_Boxes.cpp \
    w_plot_Neuron.cpp \
    w_plot_Evolution.cpp \
    w_MainWindow.cpp \
    Settings.cpp \
    NeuralNet.cpp \
    main.cpp \
    Individual.cpp \
    h_World.cpp \
    h_Simulation.cpp \
    h_Robot.cpp \
    h_Item.cpp \
    h_EatableItem.cpp \
    graph_Simulation.cpp \
    graph_Scene.cpp \
    graph_Robot.cpp \
    graph_Item.cpp \
    graph_EatableItem.cpp \
    Evolution.cpp \
    w_view_World.cpp \
    Condor.cpp \
    h_Sensor.cpp

FORMS += \
    w_widget_Simulation.ui \
    w_widget_Evolution.ui \
    w_widget_Boxes.ui \
    w_plot_MFR.ui \
    w_MainWindow.ui
