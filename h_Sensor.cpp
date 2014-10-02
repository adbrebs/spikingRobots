#include "math.h"

#include "h_EatableItem.h"

#include "h_Sensor.h"

///////////////////////
// h_Sensor
///////////////////////
h_Sensor::h_Sensor()
{
}



///////////////////////
// h_EatableItemsSensor
///////////////////////
h_EItemsSensor_1::h_EItemsSensor_1(QList<h_EatableItem*>* pSomeItems)
    : h_Sensor(),
      pItems(pSomeItems)
{
}

/*virtual*/ void h_EItemsSensor_1::detect() const
{

}
