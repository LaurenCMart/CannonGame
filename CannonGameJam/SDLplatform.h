#pragma once
#include "SDL.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

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
void update_and_render(Controls controls, bool init, GameState *game_state);

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
};

// Generates a random number between two values
static int RandomNumberGenerator(int Min, int Max)
{
    int result = (Min + (rand() % (Max - Min) + 1));

    return result;
}

// Constructor function for a Ball
static Ball BallConstructor(int x, int y)
{
    Ball BallResult = {0};
    BallResult.speed.x = RandomNumberGenerator(1, 4);
    BallResult.speed.y = RandomNumberGenerator(1, 4);

    if(BallResult.speed.y <= 1 && BallResult.speed.x >= -1)
    {
        BallResult.speed.x = 2;
    }

    if(BallResult.speed.y <= 1 && BallResult.speed.y >= -1)
    {
        BallResult.speed.y = 2;
    }

    BallResult.pos.x = x;
    BallResult.pos.y = y;

    return BallResult;
}


struct GameState
{

    bool running;

};
