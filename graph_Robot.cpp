#include <math.h>
#include <QPainter>

#include "Settings.h"
#include "h_Robot.h"

#include "graph_Simulation.h"
#include "graph_Robot.h"

const int zoom = 1;

graph_Robot::graph_Robot(Settings* settings, graph_Simulation *aGraph_Sim, h_Robot* aRobot)
    : graph_Item(settings, aGraph_Sim),
      robot(aRobot),
      height(20*zoom),
      color(qrand() % 256, qrand() % 256, qrand() % 256),
      numberLastPositions(10)
{
    transferPositionFrom(robot);

    QObject::connect(robot, SIGNAL(positionChanged(h_Item*)),
                     this, SLOT(refreshPosition(h_Item*)));
}



// Shape for redrawing area
QRectF graph_Robot::boundingRect() const
{
    qreal change = height/4; // area is larger when the robot rotates
    return QRectF(-height/2 - change, -height/2 - change, height + 3*change, height + 3*change);
}


void graph_Robot::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(color);
    painter->drawRect(-height/2, -height/2, height/2, height);
    painter->setBrush(Qt::white);
    painter->drawRect(0, -height/2, height/2, height);


    // Sensors zone : indicate if the sensor has spot something by darken
    qreal lSAngle1Rad = s->radian(robot->gLSAngle1());
    qreal lSAngle2Rad = s->radian(robot->gLSAngle2());
    qreal xl1 = height / (2*tan(lSAngle1Rad));
    qreal yl2 = -tan(lSAngle2Rad) * height/2;
    static const QPoint leftCamera[4] = {
        QPoint(0,0),
        QPoint(xl1, -height/2),
        QPoint(height/2, -height/2),
        QPoint(height/2, yl2),
    };

    qreal rSAngle1Rad = s->radian(robot->gRSAngle1());
    qreal rSAngle2Rad = s->radian(robot->gRSAngle2());
    qreal xr2 = -height / (2*tan(rSAngle2Rad));
    qreal yr1 = -tan(rSAngle1Rad) * height/2;
    static const QPoint rightCamera[4] = {
        QPoint(0,0),
        QPoint(xr2, height/2),
        QPoint(height/2, height/2),
        QPoint(height/2, yr1),
    };

    qreal mSAngle1Rad = s->radian(robot->gMSAngle1());
    qreal mSAngle2Rad = s->radian(robot->gMSAngle2());
    qreal ym1 = -tan(mSAngle1Rad) * height/2;
    qreal ym2 = -tan(mSAngle2Rad) * height/2;
    static const QPoint middleCamera[3] = {
        QPoint(0,0),
        QPoint(height/2, ym1),
        QPoint(height/2, ym2),
    };

    QVector<qreal> sensors = robot->getSensors();
    if(sensors.isEmpty())
        return;
    int maxSensorIntensity = robot->gMaxSensorIntensity();

    qreal leftIntensity = sensors[0]*255/maxSensorIntensity;
    if(leftIntensity>255)
        leftIntensity = 255;
    painter->setBrush(QColor(100, 200, 100, leftIntensity));
    painter->drawPolygon(leftCamera, 4);

    qreal rightIntensity = sensors[1]*255/maxSensorIntensity;
    if(rightIntensity>255)
        rightIntensity = 255;
    painter->setBrush(QColor(100, 200, 100, rightIntensity));
    painter->drawPolygon(rightCamera, 4);

    qreal middleIntensity = sensors[2]*255/maxSensorIntensity;
    if(middleIntensity>255)
        middleIntensity = 255;
    painter->setBrush(QColor(100, 200, 100, middleIntensity));
    painter->drawPolygon(middleCamera, 3);
}


/*virtual*/ void graph_Robot::refreshPosition(h_Item *item)
{
    graph_Item::refreshPosition(item);

    qreal s = 4*zoom;
    // Save the position // TODO maybe a memory leak
    QGraphicsEllipseItem* rec = new QGraphicsEllipseItem(x()-s/2, y()-s/2, s/2, s/2);
    rec->setZValue(-10);
    rec->setBrush(QColor(0, 0, 0));
    lastPositions.append(rec);
    graph_Sim->addGraphicsItem(lastPositions.last());
    while(lastPositions.size() > numberLastPositions)
    {
        graph_Sim->removeGraphicsItem(lastPositions.first());
        lastPositions.removeFirst();
    }
}






//////////////////////////////////////////
// graph_Robot_Explo
//////////////////////////////////////////
graph_Robot_Explo::graph_Robot_Explo(Settings* settings, graph_Simulation* aGraph_Sim, h_Robot* aRobot)
    : graph_Robot(settings, aGraph_Sim, aRobot)
{
}


void graph_Robot_Explo::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Body
    painter->setBrush(color);
    painter->drawRect(-height/2, -height/2, height/2, height);
    painter->setBrush(Qt::white);
    painter->drawRect(0, -height/2, height/2, height);


    // Sensors zone : indicate if the sensor has spot something by darken
    qreal mSAngle1Rad = s->radian(robot->gMSAngle1());
    qreal mSAngle2Rad = s->radian(robot->gMSAngle2());
    qreal ym1 = -tan(mSAngle1Rad) * height/2;
    qreal ym2 = -tan(mSAngle2Rad) * height/2;
    static const QPoint middleCamera[3] = {
        QPoint(0,0),
        QPoint(height/2, ym1),
        QPoint(height/2, ym2),
    };

    QVector<qreal> sensors = robot->getSensors();
    if(sensors.isEmpty())
        return;
    int maxSensorIntensity = robot->gMaxSensorIntensity();

    qreal middleIntensity = sensors[0]*255/maxSensorIntensity;
    if(middleIntensity>255)
        middleIntensity = 255;
    painter->setBrush(QColor(100, 200, 100, middleIntensity));
    painter->drawPolygon(middleCamera, 3);
}
