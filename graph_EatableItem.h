#ifndef GRAPH_EATABLEITEM_H
#define GRAPH_EATABLEITEM_H

#include "graph_Item.h"

class h_EatableItem;



class graph_EatableItem : public graph_Item
{
public:
    graph_EatableItem(Settings* settings, graph_Simulation* aGraph_Sim, h_EatableItem* anItem);

    qreal getWidth() {return width;}

    h_EatableItem* getItem() {return item;}

protected:
    /*virtual*/ QRectF boundingRect() const;
    /*virtual*/ void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    h_EatableItem* item;

    qreal width;
    QColor color;
};



class graph_Food : public graph_EatableItem
{
public:
    graph_Food(Settings* settings, graph_Simulation* aGraph_Sim, h_EatableItem* anItem);

    /*virtual*/ bool isGraphFood() const {return true;}
};



class graph_Poison : public graph_EatableItem
{
public:
    graph_Poison(Settings* settings, graph_Simulation* aGraph_Sim, h_EatableItem* anItem);

    /*virtual*/ bool isGraphPoison() const {return true;}
};

#endif // GRAPH_EATABLEITEM_H
