
#include "Settings.h"
#include "h_Simulation.h"
#include "h_Item.h"
#include "h_World.h"

#include "graph_Item.h"

graph_Item::graph_Item(Settings *settings, graph_Simulation *aGraph_Sim)
    : s(settings),
      graph_Sim(aGraph_Sim)
{
}


void graph_Item::refreshPosition(h_Item *item)
{
    transferPositionFrom(item);
}


void graph_Item::transferPositionFrom(const h_Item *item)
{
    int wWidth = item->getSimulation()->getWorld()->getWidth();
    int wHeigh = item->getSimulation()->getWorld()->getHeigh();

    // Conversion (the basic vectors and angle norms are different)
    setRotation(-item->getAngle());
    setX(item->getX() - wWidth/2);
    setY(wHeigh/2 - item->getY());
}
