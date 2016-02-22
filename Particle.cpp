#include "Particle.h"

Particle::Particle(Point p) : position(p),velocity(Point(25, 25)),mass(1.9e27), radius(5) {}

Particle::Particle(Point p,double m) : position(p),mass(m),velocity(Point(25, 25)), radius(5.0) {}

Particle::Particle(Point p,Point v,double m,double r) : position(p),
                        velocity(v),mass(m),radius(r) {}

Point Particle::getPos() const {
    return position;
}

Point Particle::getVelocity() const {
    return velocity;
}

void Particle::modPosX(double val) {
	position.setX(val);
}

void Particle::modPosY(double val) {
	position.setY(val);
}

void Particle::modVelX(double val) {
	velocity.setX(val);
}

void Particle::modVelY(double val) {
	velocity.setY(val);
}

double Particle::getMass() const {
    return mass;
}

double Particle::getRadius() const {
    return radius;
}

bool Particle::operator==(const Particle &p) const {
	double xPos = p.getPos().getX(); 
	double yPos = p.getPos().getY();
	double xVel = p.getVelocity().getX();
	double yVel = p.getVelocity().getY();
	double pMass = p.getMass();
	double pRad = p.getRadius();

	if(this->position.getX() == xPos && this->position.getY() == yPos && 
		this->velocity.getX() == xVel && this->velocity.getY() == yVel &&
		mass == pMass && radius == pRad) {
		return true;
	}
	return false;
} 