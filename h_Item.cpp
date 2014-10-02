#include <math.h>

#include "Settings.h"
#include "h_Simulation.h"

#include "h_Item.h"


h_Item::h_Item(Settings* settings, h_Simulation *aSimulation, qreal aRadius)
    : s(settings),
      simulation(aSimulation),
      radius(aRadius),
      x(0), y(0), angle(0)
{
}


qreal h_Item::dist(const h_Item* item1, const h_Item* item2) const
{
    qreal diffX = pow(item1->getX()-item2->getX(), 2);
    qreal diffY = pow(item1->getY()-item2->getY(), 2);

    return sqrt(diffX + diffY);
}


/*virtual*/ void h_Item::reset(qreal _x, qreal _y, int _angle)
{
    x = _x;
    y = _y;
    angle = _angle;

    emit positionChanged(this);
}


/*virtual*/ void h_Item::initPos(qreal _x, qreal _y, int _angle)
{
    reset(_x, _y, _angle);
}
