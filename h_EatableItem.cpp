#include <QPointF>

#include "Settings.h"
#include "h_Simulation.h"
#include "h_World.h"

#include "h_EatableItem.h"


h_EatableItem::h_EatableItem(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_Item(settings, aSimulation, aRadius),
      isEaten(false)
{
}


h_EatableItem::h_EatableItem(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt)
    : h_Item(settings, aSimulation, aRadius),
      isEaten(false)
{
    x = ppt->x();
    y = ppt->y();
}


/*virtual*/ void h_EatableItem::reset(qreal _x, qreal _y, int _angle)
{
    h_Item::reset(_x, _y, _angle);

    isEaten = false;
}





//////////////////////////////////////////
// h_Food
//////////////////////////////////////////
h_Food::h_Food(Settings *settings, h_Simulation *aSimulation, qreal aRadius)
    : h_EatableItem(settings, aSimulation, aRadius)
{
    // Initialize position
    simulation->resetFood(this);
}


h_Food::h_Food(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt)
    : h_EatableItem(settings, aSimulation, aRadius, ppt)
{
}


void h_Food::reset()
{
    simulation->resetFood(this);
}




//////////////////////////////////////////
// h_Poison
//////////////////////////////////////////
h_Poison::h_Poison(Settings* settings, h_Simulation* aSimulation, qreal aRadius)
    : h_EatableItem(settings, aSimulation, aRadius)
{
    // Initialize position
    simulation->resetPoison(this);
}


h_Poison::h_Poison(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt)
    : h_EatableItem(settings, aSimulation, aRadius, ppt)
{
}


void h_Poison::reset()
{
    simulation->resetPoison(this);
}
