#ifndef POINT_H
#define POINT_H

class Point {

private:

    double x;
    double y;

public:

    Point();
    Point(double x,double y);

    double getX() const;
    double getY() const;
    void setX(double val);
    void setY(double val);

};

#endif
