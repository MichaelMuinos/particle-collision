#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>

#include "Game.h"

Game::Game() : start(0), last(0), current(0), good(true), running(false),
        width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT),
        particles(std::vector<Particle>())
{
    // Seed the random number generator
    srand(time(0));

    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        good = false;
        return;
    }

    // initialize SDL window
    window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        good = false;
        return;
    }

    // initialize SDL renderer
    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        good = false;
        return;
    }

    // initialize particle texture
    SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
    if (bmp == NULL)
    {
        good = false;
        return;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (particleTexture == NULL)
    {
        good = false;
        return;
    }

    // initialize our particles
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        particles.push_back(randomParticle());
    }
}

Game::~Game()
{
    if (!good)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    if (particleTexture != NULL)
    {
        SDL_DestroyTexture(particleTexture);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int Game::operator()()
{
    if (!good)
    {
        return -1;
    }
    running = true;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    start = SDL_GetTicks();
    last = start;
    while (running) // every iteration is 1 frame
    {
        current = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        update((current - last) / 1000.0);
        render();
        last = current;
    }
    return 0;
}

bool collides(Particle p1, Particle p2) {
    double xDiff = p1.getPos().getX() - p2.getPos().getX();
    double yDiff = p1.getPos().getY() - p2.getPos().getY();

    //lol
    double totallyRad = p1.getRadius() + p2.getRadius();
    double squareRad = totallyRad * totallyRad;

    double squareDist = (xDiff * xDiff) + (yDiff * yDiff);

    if (squareDist <= squareRad) {
        return true;
    }

    return false;
}

bool checkedPair(std::vector<std::pair<Particle,Particle>> pairVector,Particle p, Particle p2) {
    if(pairVector.size() == 0) {
        pairVector.push_back(std::make_pair(p,p2));
        return false;
    }
    for(int i = 0; i < pairVector.size(); ++i) {
        std::pair<Particle,Particle> temp = pairVector[i];
        if((p == temp.first && p2 == temp.second) || (p == temp.second && p2 == temp.first)) {
            return true;
        }
    }
    pairVector.push_back(std::make_pair(p,p2));
    return false;
}

double findAngleOfContact(Particle p, Particle p2) {
    return atan2(p2.getPos().getY() - p.getPos().getY(), p2.getPos().getX() - p.getPos().getX()) * (180/3.1415);
}

/*void calculateCollisionss(Particle &p1, Particle &p2, double dt) {

    //find magnitudes
    double p1Magnitude = sqrt((p1.getVelocity().getX() * p1.getVelocity().getX())
                                       + (p1.getVelocity().getY() * p1.getVelocity().getY()));
    double p2Magnitude = sqrt((p2.getVelocity().getX() * p2.getVelocity().getX())
                                       + (p2.getVelocity().getY() * p2.getVelocity().getY()));

    //calculate angle of contact
    double angleContact = findAngleOfContact(p1, p2);

    //calculate p1 and p2 angles
    double p1AngleMovement = atan(p1.getPos().getY()/p1.getPos().getX()) * (180/3.1415); //Might try converting 180/pi
    double p2AngleMovement = atan(p2.getPos().getY()/p2.getPos().getX()) * (180/3.1415); //potentially use atan2?

    //calculate new velocities
    double p1NewXVelocity = ((((p1Magnitude * cos(p1AngleMovement - angleContact) * (p1.getMass() - p2.getMass()))
                               + (2 * p2.getMass() * p2Magnitude * cos(p2AngleMovement - angleContact))) / (p1.getMass() + p2.getMass()))
                                * (cos(angleContact) + (p1Magnitude * sin(p1AngleMovement - angleContact) * cos(angleContact + (3.14/2)))));

    double p1NewYVelocity = ((((p1Magnitude * cos(p1AngleMovement - angleContact) * (p1.getMass() - p2.getMass())) 
                                + (2 * p2.getMass() * p2Magnitude * cos(p2AngleMovement - angleContact))) / (p1.getMass() + p2.getMass())) 
                                * (sin(angleContact) + (p1Magnitude * sin(p1AngleMovement - angleContact) * sin(angleContact + (3.14/2)))));

    double p2NewXVelocity = ((((p2Magnitude * cos(p2AngleMovement - angleContact) * (p2.getMass() - p1.getMass()))
                               + (2 * p1.getMass() * p1Magnitude * cos(p1AngleMovement - angleContact))) / (p2.getMass() + p1.getMass()))
                                * (cos(angleContact) + (p2Magnitude * sin(p2AngleMovement - angleContact) * cos(angleContact + (3.14/2)))));

    double p2NewYVelocity = ((((p2Magnitude * cos(p2AngleMovement - angleContact) * (p2.getMass() - p1.getMass()))
                               + (2 * p1.getMass() * p1Magnitude * cos(p1AngleMovement - angleContact))) / (p2.getMass() + p1.getMass()))
                                * (sin(angleContact) + (p2Magnitude * sin(p2AngleMovement - angleContact) * sin(angleContact + (3.14/2)))));

    //Set new velocities
    p1.modVelX(p1NewXVelocity * dt); 
    p1.modVelY(p1NewYVelocity * dt);
    p2.modVelX(p2NewXVelocity * dt);
    p2.modVelY(p2NewYVelocity * );
}*/

void calculateCollision(Particle &p1, Particle &p2)
{
    double m2 = p1.getMass();
    double m1 = p2.getMass();
    double x2 = p1.getPos().getX();
    double x1 = p2.getPos().getX();
    double y2 = p1.getPos().getY();
    double y1 = p2.getPos().getY();
    double vx2 = p1.getVelocity().getX();
    double vx1 = p2.getVelocity().getX();
    double vy2 = p1.getVelocity().getY();
    double vy1 = p2.getVelocity().getY();
    
    double newMass = m2 / m1;
    double newX = x2 - x1;
    double newY = y2 - y1;
    double newVelX = vx2 - vx1;
    double newVelY = vy2 - vy1;

    if ((newVelX * newX + newVelY * newY) >= 0)
    {
        return;
    }

    double vxCm = (m1 * vx1 + m2 * vx2) / (m1 + m2);
    double vyCm = (m1*vy1 + m2*vy2) / (m1 + m2);

    double a = newY / newX;
    double dvx2 = -2 * (newVelX + a * newVelY) / ((1 + a * a) * (1 + newMass));
    vx2 += dvx2;
    vy2 = vy2 + a * dvx2;
    vx1 = vx1 - newMass * dvx2;
    vy1 = vy1 - a * newMass * dvx2;

    p2.modVelX(vx1);
    p2.modVelY(vy1);
    p1.modVelX(vx2);
    p1.modVelY(vy2);
}

void Game::update(double dt)
{
    std::cout << dt << " sec since last frame.\n";

    // Replace with your game logic!
    for(int i = 0; i < PARTICLE_COUNT; ++i) {
    	for(int j = 0; j < PARTICLE_COUNT; ++j) {
    		//Effects of gravity between particles
    		if(i != j) {
        		//Distance between two radiuses
        		//get x and y compnents of acceleration for accel variable
    	   	 	double xDiff = particles[j].getPos().getX() - particles[i].getPos().getX();
        		double yDiff = particles[j].getPos().getY() - particles[i].getPos().getY();

       			double squareDist = (xDiff * xDiff) + (yDiff * yDiff);    			

    		    double acceleration = G * (particles[j].getMass()/(squareDist));
    		  	double angle = atan2(yDiff, xDiff);
    		   	double accelX = acceleration * cos(angle);
    		   	double accelY = acceleration * sin(angle);
    		    double valueX = particles[i].getVelocity().getX() + (accelX * dt);
    		    double valueY = particles[i].getVelocity().getY() + (accelY * dt);
        	    particles[i].modVelX(valueX);
         	   	particles[i].modVelY(valueY);
			}		
  	 	 }
    }
    std::vector<std::pair<Particle,Particle>> pairVector;
    for(int i = 0; i < PARTICLE_COUNT; ++i) {
        for(int j = 0; j < PARTICLE_COUNT; ++j) {
            //Collision Detection
            bool wat = checkedPair(pairVector, particles[i], particles[j]);
            if(!wat && j != i && collides(particles[i],particles[j])) {
                calculateCollision(particles[i], particles[j]);
            }
        }
    }

    for(int i = 0; i < PARTICLE_COUNT; ++i) {
        //Boundaries
        if(particles[i].getPos().getX() <= 0) {
            double negateX = particles[i].getVelocity().getX() * -1;
            particles[i].modPosX(0);
            particles[i].modVelX(negateX);
        }

        if(particles[i].getPos().getX() >= 800) {
            double negateX = particles[i].getVelocity().getX() * -1;
            particles[i].modPosX(800);
            particles[i].modVelX(negateX);
        }

        if(particles[i].getPos().getY() <= 0) {
            double negateY = particles[i].getVelocity().getY() * -1;
            particles[i].modPosY(0);
            particles[i].modVelY(negateY);
        }

        if(particles[i].getPos().getY() >= 600) {
            double negateY = particles[i].getVelocity().getY() * -1;
            particles[i].modPosY(600);
            particles[i].modVelY(negateY);
        }   
    }

    double tempX;
    double tempY;
    double oldXPos;
    double oldYPos;
    for(int k = 0; k < PARTICLE_COUNT; ++k) {
        tempX = particles[k].getVelocity().getX();
        tempY = particles[k].getVelocity().getY();
        oldXPos = particles[k].getPos().getX();
        oldYPos = particles[k].getPos().getY();
        particles[k].modPosX(oldXPos + (tempX * dt));
        particles[k].modPosY(oldYPos + (tempY * dt));
        std::cout << "Run: " << k << std::endl;
        std::cout << tempX << std::endl;
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);

    // rendering here would place objects beneath the particles

    for (const Particle& p : particles)
    {
        drawParticle(p);
    }

    // rendering here would place objects on top of the particles

    SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    // Add your own event handling here if desired
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::drawParticle(const Particle& p)
{
    SDL_Rect dst;
    double shift = p.getRadius();
    dst.x = (int) (p.getPos().getX() - shift);
    dst.y = (int) (p.getPos().getY() - shift);
    dst.w = shift * 2;
    dst.h = shift * 2;
    SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Particle Game::randomParticle() const
{
    Point pos(rand() % width, rand() % height);

    // using some percentage of the mass of Jupiter
    double mass = ((double) rand() / RAND_MAX) * 100 + 1.0;

    return Particle(pos, mass);
}
