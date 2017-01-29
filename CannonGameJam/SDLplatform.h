#pragma once
#include "SDL.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string>

struct Controls
{
    bool left;
    bool prev_left;

    bool right;
    bool prev_right;

    bool up;
    bool prev_up;

    bool down;
    bool prev_down;
};

struct GameState;
void update_and_render(Controls controls, bool init, GameState *game_state, SDL_Renderer *pRenderer, SDL_Surface *pScreenSurface);

struct Vector2D
{
    int x, y;
};

static Vector2D V2Constructor(int x, int y)
{
    Vector2D result = {x, y};

    return(result);
}

// Put your stuff here!

struct Block
{
    Vector2D pos;
    bool isEnabled;
};

struct Ball
{
    Vector2D pos;
    Vector2D speed;
    Vector2D size;
};

struct Rect
{
    Vector2D pos;
    Vector2D size;
};

// Generates a random number between two values
int RandomNumberGenerator(int Min, int Max);

// Constructor function for a Ball
Ball BallConstructor(int x, int y);

// Creates a rectangle from a position and a size:
SDL_Rect RectFromPositions(Vector2D Position, Vector2D Size);

// Loading a Texture:
SDL_Texture *LoadTexture(char *pFileName, SDL_Renderer *pRenderer, SDL_Surface *pScreenSurface);

// Tests whether two objects overlap
bool TestOverlap(Vector2D Position1, Vector2D Size1, Vector2D Position2, Vector2D Size2);

// Loading SDL PNG Image
SDL_Surface* loadSurface(std::string path, SDL_Surface *pScreenSurface);

struct GameState
{
    Rect ground;
    Rect cannonBase;
    Rect cannonShaft;
    SDL_Texture *pCannonShaft;
    Ball aBall;
    SDL_Texture *pBall;



    float angle = 0.0f;

    bool running;
};
