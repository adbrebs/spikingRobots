#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H

class Settings;
class h_World;

#include <QGraphicsScene>

class graph_Scene : public QGraphicsScene
{
public:
    graph_Scene(Settings* settings, h_World *aWorld);

    /*virtual*/ void drawBackground( QPainter * painter, const QRectF & rect );

    void saveOnFile(QString filename);
    void saveSVGOnFile(QString filename);

private:
    Settings* s;

    h_World* world;
};

#endif // GRAPH_SCENE_H
