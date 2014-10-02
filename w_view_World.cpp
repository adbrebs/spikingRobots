#include <math.h>
#include <QMouseEvent>

#include "graph_Simulation.h"
#include "graph_EatableItem.h"

#include "w_view_World.h"


w_view_World::w_view_World(QWidget *parent)
    : QGraphicsView(parent),
      addMode(ADD_NO)
{
}


void w_view_World::init(graph_Simulation* aGraph_simulation)
{
    graph_sim = aGraph_simulation;
}


void w_view_World::mousePressEvent(QMouseEvent * e)
{
    if(!e->modifiers().testFlag(Qt::ControlModifier))
        return QGraphicsView::mousePressEvent(e);

    QList<graph_EatableItem*> graphFoods = graph_sim->getgraph_Foods();
    QList<graph_EatableItem*> graphPoisons = graph_sim->getgraph_Poisons();

    QPointF pt = mapToScene(e->pos());

    // Remove items touched by the mouse.
    bool foodWasRemoved = removesItem(&pt, &graphFoods);
    bool poisonWasRemoved = removesItem(&pt, &graphPoisons);


    if(!foodWasRemoved && !poisonWasRemoved)
    {
        if(addMode == ADD_FOOD)
            graph_sim->addFood(pt);
        else if(addMode == ADD_POIS)
            graph_sim->addPoison(pt);
    }
}


// Remove items touched by the mouse (ppt). If some items are removed, return true
bool w_view_World::removesItem(QPointF* ppt, QList<graph_EatableItem*>* pGraphItems)
{
    bool var = false;

    foreach(graph_EatableItem* graphItem, *pGraphItems)
    {
        qreal distance = sqrt( pow(ppt->x() - graphItem->x(),2) + pow(ppt->y() - graphItem->y(),2) );

        if(distance < graphItem->getWidth())
        {
            graph_sim->removeItem(graphItem);
            var = true;
        }
    }

    return var;
}


void w_view_World::clearAllItems()
{
    graph_sim->clearAllItems();
}


void w_view_World::reInitializeAllItems()
{
    graph_sim->reInitializeAllItems();
}


void w_view_World::reIniRobotsPos()
{
    graph_sim->reIniRobotsPos();
}


void w_view_World::wheelEvent ( QWheelEvent * e)
{
    if (e->modifiers().testFlag(Qt::ControlModifier)){ // zoom only when CTRL key pressed
        int numSteps = e->delta() / 15 / 8;

        if (numSteps == 0) {
            e->ignore();
            return;
        }
        qreal sc = pow(1.25, numSteps); // I use scale factor 1.25
        this->zoom(sc, mapToScene(e->pos()));
        e->accept();
    }
    else
    {
        QGraphicsView::wheelEvent(e);
    }
}

void w_view_World::zoom(qreal factor, QPointF centerPoint)
{
    scale(factor, factor);
    centerOn(centerPoint);
}
