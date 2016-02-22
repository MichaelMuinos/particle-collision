#ifndef PARTICLE_H
#define PARTICLE_H

#include "Point.h"

class Particle {

private:

    Point position;
    Point velocity;
    double mass;
    double radius;

public:

    Particle(Point p);
    Particle(Point p,double m);
    Particle(Point p,Point v,double m,double r);
    Point getPos() const;
    Point getVelocity() const;
    void modVelX(double val);
    void modVelY(double val);
    void modPosX(double val);
    void modPosY(double val);
    double getMass() const;
    double getRadius() const;

    bool operator==(const Particle &p) const;

};

#endif
