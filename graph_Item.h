#ifndef GRAPH_ITEM_H
#define GRAPH_ITEM_H

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class Settings;
class graph_Simulation;
class h_Item;

#include <QGraphicsObject>

class graph_Item : public QGraphicsObject
{
    Q_OBJECT

public:
    graph_Item(Settings* settings, graph_Simulation* aGraph_Sim);

    virtual bool isGraphRobot() const {return false;}
    virtual bool isGraphFood() const {return false;}
    virtual bool isGraphPoison() const {return false;}

public slots:
    virtual void refreshPosition(h_Item *item);

protected:
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget) = 0;

    void transferPositionFrom(const h_Item* item);

protected:
    Settings* s;

    graph_Simulation* graph_Sim;
};

#endif // GRAPH_ITEM_H
