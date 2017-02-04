/* To Do:
1. Create window, add background.
2. Create a rectangle for the ground.
3. Create Cannon
4. Make cannon move
5. Make cannon fire projectile
6. Add some wall obsticles
7. Add a target for the cannon ball
8. When target is destroyed, target disappears. Game Won message appears.
9. Add 3 balls for player. When all 3 cannon balls have been used, and target hasn't been hit, game over message appears.
10. After either game over or game won messages appear, ask user if they want to play again.
11. Add sounds and background music.
*/

#include "SDL.h"
#include "SDLplatform.h"
#include "assert.h"

float toRadians(float x)
{
    float res = x * 0.0174533f; // 1 degree = 0.0174533 radians.

    return res;
}

void resetBallPos(Ball *ball, Vector2D pos)
{
    ball->pos = pos;
    ball->speed = 0.0f;
    ball->velocityY = 0.0f;
    ball->canShoot = true;
}

void initGame(GameState *game_state, SDL_Renderer *pRenderer, SDL_Surface *pScreenSurface)
{

    // Ground Properties
    game_state->ground.pos.x = 0;
    game_state->ground.pos.y = 460;
    game_state->ground.size.x = 640;
    game_state->ground.size.y = 30;

    // Cannon Base Properties
    game_state->cannonBase.pos.x = 20;
    game_state->cannonBase.pos.y = 420;
    game_state->cannonBase.size.x = 30;
    game_state->cannonBase.size.y = 40;

    // Cannon Shaft Properties
    game_state->cannonShaft.pos.x = 30;
    game_state->cannonShaft.pos.y = 420;
    game_state->cannonShaft.size.x = 60;
    game_state->cannonShaft.size.y = 20;
    game_state->cannonShaftAngle = 360.0f;

    game_state->pCannonShaft = LoadTexture("CannonShaft.bmp", pRenderer, pScreenSurface);
    // Asserting if it was successfully created
    assert(game_state->pCannonShaft);

    // Wall Properties
    game_state->wall.pos.x = 310;
    game_state->wall.pos.y = 240;
    game_state->wall.size.x = 20;
    game_state->wall.size.y = 220;


    // Ball Properties
    game_state->aBall.pos.x = 30;
    game_state->aBall.pos.y = 420;
    game_state->aBall.size.x = 24;
    game_state->aBall.size.y = 24;
    game_state->aBall.canShoot = true;


    game_state->pBall = LoadTexture("BreakoutBall.png", pRenderer, pScreenSurface);
    // Asserting if it was successfully created
    assert(game_state->pBall);

}

void update_and_render(Controls controls, bool init, GameState *game_state, SDL_Renderer *pRenderer, SDL_Surface *pScreenSurface)
{
    if(init)
    {
        initGame(game_state, pRenderer, pScreenSurface);
    }

    // Collision of Ball and Wall
    Vector2D ballSize = V2Constructor(game_state->aBall.size.x, game_state->aBall.size.y);
    Vector2D wallSize = V2Constructor(game_state->wall.size.x, game_state->wall.size.y);

    if(TestOverlap(game_state->aBall.pos, ballSize, game_state->wall.pos, wallSize))
    {
        resetBallPos(&game_state->aBall, game_state->cannonShaft.pos);
    }

    // Reseting Ball position when it flies off the sides.
    if(game_state->aBall.pos.x > 640 || game_state->aBall.pos.x < 0)
    {
        resetBallPos(&game_state->aBall, game_state->cannonShaft.pos);
    }

    if(game_state->aBall.pos.y > 480 || game_state->aBall.pos.y < 0)
    {
        resetBallPos(&game_state->aBall, game_state->cannonShaft.pos);
    }

    // Moving Cannon Shaft
    if(controls.up)
    {
        game_state->cannonShaftAngle -= 10;
    }
    else if(controls.down)
    {
        game_state->cannonShaftAngle += 10;
    }

    // Adding Cannon Shaft Movement Restrictions
    if(game_state->cannonShaftAngle < 270)
    {
        game_state->cannonShaftAngle = 270;
    }
    else if(game_state->cannonShaftAngle > 360)
    {
        game_state->cannonShaftAngle = 360;
    }

    // Ball Movement
    if(controls.space && game_state->aBall.canShoot)
    {
        game_state->aBall.canShoot = false;
        game_state->aBall.speed = 5.0f;
        game_state->aBall.angle = game_state->cannonShaftAngle;

    }

    if(game_state->aBall.speed > 0)
    {
        float deltaX = cosf(toRadians(game_state->aBall.angle)) * game_state->aBall.speed;
        float deltaY = sinf(toRadians(game_state->aBall.angle))  * game_state->aBall.speed;

        game_state->aBall.pos.x += deltaX;
        game_state->aBall.pos.y += deltaY;
        
        // Ball Gravity
        float dt = 0.08f;
        float acceleration = 9.8f;
        game_state->aBall.velocityY += acceleration * dt;
        game_state->aBall.pos.y += game_state->aBall.velocityY * dt;
    }




    // Change BG colour
    SDL_SetRenderDrawColor(pRenderer, 0x66, 0x00, 0x66, 0xFF);

    // Clear Screen
    SDL_RenderClear(pRenderer);

    // Drawing Ground
    SDL_Rect GroundRect = RectFromPositions(game_state->ground.pos, game_state->ground.size);
    SDL_SetRenderDrawColor(pRenderer, 0xF9, 0xB8, 0x1F, 0x00);
    SDL_RenderFillRect(pRenderer, &GroundRect);

    
    // Drawing Ball
    SDL_Rect cannonBallRect = RectFromPositions(game_state->aBall.pos, game_state->aBall.size);
    //SDL_SetRenderDrawColor(pRenderer, 0xFF, 0x00, 0x00, 0x00);
    //SDL_BlitSurface(game_state->pBall, NULL, pScreenSurface, &cannonBallRect);
    SDL_RenderCopy(pRenderer, game_state->pBall, NULL, &cannonBallRect);

    // Drawing Cannon Shaft
    SDL_Rect cannonShaftRect = RectFromPositions(game_state->cannonShaft.pos, game_state->cannonShaft.size);
    SDL_SetRenderDrawColor(pRenderer, 0xFF, 0x00, 0x00, 0x00);
    SDL_Point centerPt = {5, 10};
    SDL_RenderCopyEx(pRenderer, game_state->pCannonShaft, NULL, &cannonShaftRect, game_state->cannonShaftAngle, &centerPt, SDL_FLIP_NONE);

    // Drawing Cannon Base
    SDL_Rect cannonBaseRect = RectFromPositions(game_state->cannonBase.pos, game_state->cannonBase.size);
    SDL_SetRenderDrawColor(pRenderer, 0xF9, 0x00, 0x1F, 0x00);
    SDL_RenderFillRect(pRenderer, &cannonBaseRect);

    // Drawing Wall
    SDL_Rect wallRect = RectFromPositions(game_state->wall.pos, game_state->wall.size);
    SDL_SetRenderDrawColor(pRenderer, 0xFF, 0x08, 0xFF, 0x00);
    SDL_RenderFillRect(pRenderer, &wallRect);




}



