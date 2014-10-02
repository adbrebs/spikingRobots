#include <QPainter>

#include "Settings.h"
#include "graph_EatableItem.h"

#include "h_EatableItem.h"

const int zoom = 1;


//////////////////////////////////////////
// graph_EatableItem
//////////////////////////////////////////
graph_EatableItem::graph_EatableItem(Settings* settings, graph_Simulation *aGraph_Sim, h_EatableItem* anItem)
    : graph_Item(settings, aGraph_Sim),
      item(anItem),
      width(5*zoom),
      color(200, 100, 100)
{
    transferPositionFrom(item);

    QObject::connect(item, SIGNAL(positionChanged(h_Item*)),
                     this, SLOT(refreshPosition(h_Item*)));
}

QRectF graph_EatableItem::boundingRect() const
{
    return QRectF(-width, -width, width*2, width*2);
}

void graph_EatableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(color);
    painter->drawEllipse(-width, -width, width*2, width*2);
}


//////////////////////////////////////////
// graph_Food
//////////////////////////////////////////
graph_Food::graph_Food(Settings* settings, graph_Simulation *aGraph_Sim, h_EatableItem* anItem)
    : graph_EatableItem(settings, aGraph_Sim, anItem)
{
    color.setRgb(100, 200, 100);
}



//////////////////////////////////////////
// graph_Poison
//////////////////////////////////////////
graph_Poison::graph_Poison(Settings* settings, graph_Simulation *aGraph_Sim, h_EatableItem* anItem)
    : graph_EatableItem(settings, aGraph_Sim, anItem)
{
    color.setRgb(200, 100, 100);
}

