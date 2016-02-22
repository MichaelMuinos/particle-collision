#ifndef GAME_H
#define	GAME_H

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>

#include "Particle.h"

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600;
const int PARTICLE_COUNT = 25;

// use this in your gravity calculations
const double G = 6.67e-11;

class Game
{
private:
    // screen info
    int width;
    int height;
    
    // for timing frames
    unsigned int start;
    unsigned int last;
    unsigned int current;
    
    // for game status
    bool good;
    bool running;
    
    // SDL managed
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* particleTexture;
    
    // game data
    std::vector<Particle> particles;
    
    // Handle game logic updates here
    void update(double dt);
    
    // Render the game contents to the screen here
    void render();
    
    // Handle any user input events here
    void handleEvent(const SDL_Event& event);
    
    void drawParticle(const Particle& p);
    
    Particle randomParticle() const;
public:
    // This will act as our initialize function
    Game();
    
    // We don't want to be able to copy a game
    Game(const Game& orig) = delete;
    Game& operator=(const Game& right) = delete;
    
    // This will act as our cleanup function
    ~Game();
    
    // Will handle actually running the game (the gameplay loop)
    int operator()();
};

#endif	/* GAME_H */

