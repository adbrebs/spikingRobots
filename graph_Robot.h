#ifndef GRAPH_ROBOT_H
#define GRAPH_ROBOT_H

class Settings;
class h_Robot;

#include <QList>
#include <QGraphicsItem>

#include "graph_Item.h"

class graph_Robot : public graph_Item
{
public:
    graph_Robot(Settings* settings, graph_Simulation* aGraph_Sim, h_Robot* aRobot);

    /*virtual*/ bool isGraphRobot() const {return true;}

    h_Robot* getRobot() {return robot;}

    void setNumberLastPositions(int newNumber) {numberLastPositions = newNumber;}

protected:
    /*virtual*/ QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *a,
                           QWidget *b);

public slots:
    /*virtual*/ void refreshPosition(h_Item *item);

protected:
    h_Robot* robot;

    qreal height;
    QColor color;

    int numberLastPositions;
    QList<QGraphicsItem*> lastPositions;
};





class graph_Robot_Explo : public graph_Robot
{
public:
    graph_Robot_Explo(Settings* settings, graph_Simulation* aGraph_Sim, h_Robot* aRobot);

private:
    /*virtual*/ void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget);
};

#endif
