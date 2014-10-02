#ifndef H_WORLD_H
#define H_WORLD_H

typedef double qreal;

class h_Simulation;

class h_World
{
public:
    h_World(h_Simulation* aSimulation, qreal aWidth, qreal aHeigh);

    qreal getWidth() const {return width;}
    qreal getHeigh() const {return heigh;}

private:
    h_Simulation* simulation;

    qreal width;
    qreal heigh;
};

#endif // H_WORLD_H
