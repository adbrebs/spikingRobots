#ifndef W_VIEW_WORLD_H
#define W_VIEW_WORLD_H

#define ADD_NO      0
#define ADD_FOOD    1
#define ADD_POIS    2

#include <QGraphicsView>

class QMouseEvent;
class graph_Simulation;
class graph_EatableItem;

class w_view_World : public QGraphicsView
{
    Q_OBJECT
public:
    explicit w_view_World(QWidget *parent = 0);

    void init(graph_Simulation* aGraph_simulation);
    void setFoodAddMode() {addMode = ADD_FOOD;}
    void setPoisAddMode() {addMode = ADD_POIS;}

    void clearAllItems();
    void reInitializeAllItems();
    void reIniRobotsPos();

    void zoom(qreal factor, QPointF centerPoint);

private:
    bool removesItem(QPointF* ppt, QList<graph_EatableItem *> *pGraphItems);

signals:

public slots:
    void mousePressEvent(QMouseEvent* e);
    void wheelEvent( QWheelEvent * event );
    // void mouseReleaseEvent(QMouseEvent * e);
    // void mouseDoubleClickEvent(QMouseEvent * e);
    // void mouseMoveEvent(QMouseEvent * e);

private:
    graph_Simulation* graph_sim;
    int addMode;
};

#endif // W_VIEW_WORLD_H

