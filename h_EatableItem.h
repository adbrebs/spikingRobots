#ifndef H_EATABLEITEM_H
#define H_EATABLEITEM_H

class QPointF;

#include "h_Item.h"

class h_EatableItem : public h_Item
{
public:
    h_EatableItem(Settings* settings, h_Simulation* aSimulation, qreal aRadius);
    h_EatableItem(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt);

    void setIsEaten(bool var) {isEaten = var;}
    bool getIsEaten() {return isEaten;}

    /*virtual*/ void reset(qreal _x, qreal _y, int _angle);

protected:
    bool isEaten;
};


class h_Food : public h_EatableItem
{
public:
    h_Food(Settings* settings, h_Simulation* aSimulation, qreal aRadius);
    h_Food(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt);

    void reset();

    /*virtual*/ bool isFood() const {return true;}
};


class h_Poison : public h_EatableItem
{
public:
    h_Poison(Settings* settings, h_Simulation* aSimulation, qreal aRadius);
    h_Poison(Settings* settings, h_Simulation* aSimulation, qreal aRadius, QPointF* ppt);

    void reset();

    /*virtual*/ bool isPoison() const {return true;}
};




#endif // H_EATABLEITEM_H
