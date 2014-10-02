#ifndef H_SENSOR_H
#define H_SENSOR_H

#include <QList>

class h_EatableItem;


class h_Sensor
{
public:
    h_Sensor();

    virtual void detect() const = 0;
};



class h_EItemsSensor_1 : public h_Sensor
{
public:
    h_EItemsSensor_1(QList<h_EatableItem*>* pSomeItems);

    /*virtual*/ void detect() const;

private:
    QList<h_EatableItem*>* pItems;
};


#endif // H_SENSOR_H
