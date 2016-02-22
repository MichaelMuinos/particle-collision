#include "Point.h"

Point::Point() : x(0.0),y(0.0) {}
Point::Point(double x,double y) : x(x),y(y) {}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

void Point::setX(double val) {
    this->x = val;
}

void Point::setY(double val) {
    this->y = val;
}
