#ifndef H_ITEM_H
#define H_ITEM_H

typedef double qreal;

class Settings;
class h_Simulation;

#include <QObject>

class h_Item : public QObject
{
    Q_OBJECT

public:
    h_Item(Settings *settings, h_Simulation *aSimulation, qreal aRadius);

    h_Simulation* getSimulation() const {return simulation;}

    qreal getX() const {return x;}
    qreal getY() const {return y;}
    int getAngle() const {return angle;}

    void setX(qreal _x) {x =_x;}
    void setY(qreal _y) {y =_y;}
    void setAngle(int _angle) {angle =_angle;}

    virtual bool isRobot() const {return false;}
    virtual bool isFood() const {return false;}
    virtual bool isPoison() const {return false;}

    virtual void initPos(qreal _x, qreal _y, int _angle);
    virtual void reset(qreal _x, qreal _y, int _angle);

signals:
    void positionChanged(h_Item *item);

protected:
    qreal dist(const h_Item *item1, const h_Item *item2) const;

protected:
    Settings* s;

    h_Simulation* simulation;

    qreal radius; // For collision

    qreal x;
    qreal y;
    int angle;
};

#endif // H_ITEM_H
