#include <QGraphicsLineItem>
#include <QPainter>
#include <QImage>
#include <QSvgGenerator>

#include "Settings.h"

#include "h_World.h"
#include "graph_Scene.h"

graph_Scene::graph_Scene(Settings *settings, h_World *aWorld)
    : s(settings),
      world(aWorld)
{
    int widthW = world->getWidth();
    int heighW = world->getHeigh();
    setSceneRect(-widthW/2, -heighW/2, widthW, heighW);
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

void graph_Scene::drawBackground( QPainter * painter, const QRectF & rect )
{
    painter->save();

    int widthW = world->getWidth();
    int heighW = world->getHeigh();

    QPen pen(Qt::black, 5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->drawRect(-widthW/2, -heighW/2, widthW, heighW);

    // Sensor scope length
    QPen pen2(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter->setPen(pen2);
    painter->drawLine(-widthW/2+5, heighW/2+10, -widthW/2+5+s->rob.sensorScope, heighW/2+10);
    painter->drawText(-widthW/2+5, heighW/2+25, "Sensor scope");

    painter->restore();
}


void graph_Scene::saveOnFile(QString filename)
{
    QImage image(5000,5000,QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter);
    image.save(filename);
}


void graph_Scene::saveSVGOnFile(QString filename)
{
    QSvgGenerator svgGen;

    svgGen.setFileName( filename );
    svgGen.setSize(QSize(200, 200));
    svgGen.setViewBox(QRect(0, 0, 200, 200));
    svgGen.setTitle(tr("SVG Generator Example Drawing"));
    svgGen.setDescription(tr("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt."));

    QPainter painter( &svgGen );
    render( &painter );
}
