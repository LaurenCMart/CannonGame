#include "SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
//#include <sdl_mixer.h>
#include <string.h>
#include <string>
#include <Windows.h>
#include "SDLPlatform.h"

using namespace::std;

// ---------------------------------------------------------- UTILITY STUFF ----------------------------------------------------------

// Function for loading images
SDL_Surface *LoadImage(char *path, SDL_Surface *ScreenSurface)
{
    // The final optimized image
    SDL_Surface *optimizedSurface = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = SDL_LoadBMP(path);
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        __debugbreak();
    }
    else
    {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, ScreenSurface->format, NULL);
        if(optimizedSurface == NULL)
        {
            printf("Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());
            __debugbreak();
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

// Loading a Texture:
SDL_Texture *LoadTexture(char *pFileName, SDL_Renderer *pRenderer, SDL_Surface *pScreenSurface)
{
    // pFileName: Pointer to the Filename of the Bitmap you're loading in
    // pRenderer: Is a Pointer to the Renderer that renders the window
    // pScreenSurface: Pointer to the part inside the window border

    // Creates an SDL_Texture object called Result which points to NULL
    SDL_Texture *Result = NULL;

    // SurfaceResult is an SDL Surface Object that points to the result of the LoadImage function
    // LoadImage function loads a bitmap
    SDL_Surface *SurfaceResult = LoadImage(pFileName, pScreenSurface);

    // Sets a bitmap background transparent
    SDL_SetColorKey(SurfaceResult, SDL_TRUE, SDL_MapRGB(SurfaceResult->format, 0xFF, 0xFF, 0xFF));

    // Passes the image data to the GPU and gets a handle to it
    Result = SDL_CreateTextureFromSurface(pRenderer, SurfaceResult);

    // Get rid of memory for loaded surface
    SDL_FreeSurface(SurfaceResult);
    return Result;
}

// Creating a messagebox
bool displayWinMessageBox(const char *message, const char *title)
{
    int returnedMessageBox = MessageBoxA(NULL, message, title, MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL);

    if(returnedMessageBox == IDYES)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Creates a rectangle from a position and a size:
SDL_Rect RectFromPositions(Vector2D Position, Vector2D Size)
{
    SDL_Rect Result;
    Result.x = Position.x;
    Result.y = Position.y;
    Result.w = Size.x;
    Result.h = Size.y;

    // Returns a Rectangle
    return Result;
}

// Tests whether two objects overlap
bool TestOverlap(Vector2D Position1, Vector2D Size1, Vector2D Position2, Vector2D Size2)
{
    if(Position1.x <= Position2.x + Size2.x)
        if(Position2.x <= Position1.x + Size1.x)
            if(Position1.y <= Position2.y + Size2.y)
                if(Position2.y <= Position1.y + Size1.y)
                    return true;

    return false;
}

int main(int argc, char **argv)
{
    // Window stuff
    // initialising SDL's video component.
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        // if SDL cannot be initialised...
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Creates a window
        SDL_Window *Window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

        if(!Window)
        {
            // if a window cannot be created....
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Otherwise, do all this...

            // Creates a renderer
            SDL_Renderer *pRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

            // Gets screen surface
            SDL_Surface *pScreenSurface = SDL_GetWindowSurface(Window);

            // Game Loop
            GameState *game_state = new GameState();
            Controls controls = {};
            bool first_time = true;

            if(!game_state)
            {
                fprintf(stderr, "Failed to allocate gamestate");
            }
            else
            {
                game_state->running = true;

                while(game_state->running)
                {
                    int StartTime = SDL_GetTicks();
                    int TargetMS = 33;

                    if(controls.left)
                    {
                        controls.prev_left = true;
                    }
                    else
                    {
                        controls.prev_left = false;
                    }
                    if(controls.right)
                    {
                        controls.prev_right = true;
                    }
                    else
                    {
                        controls.prev_right = false;
                    }
                    if(controls.up)
                    {
                        controls.prev_up = true;
                    }
                    else
                    {
                        controls.prev_up = false;
                    }
                    if(controls.down)
                    {
                        controls.prev_down = true;
                    }
                    else
                    {
                        controls.prev_down = false;
                    }

                    //controls.left = false; controls.right = false; controls.up = false; controls.down = false;

                    SDL_Event e = {0};
                    while(SDL_PollEvent(&e) != 0)
                    {
                        switch(e.type)
                        {
                            case SDL_QUIT:
                            {
                                game_state->running = false;

                            } break;

                            case SDL_KEYDOWN:
                            {
                                switch(e.key.keysym.sym)
                                {
                                    case 'a': case SDLK_LEFT:
                                    {
                                        controls.left = true;
                                    } break;
                                    case 'd': case SDLK_RIGHT:
                                    {
                                        controls.right = true;
                                    } break;
                                    case 'w': case SDLK_UP:
                                    {
                                        controls.up = true;
                                    } break;
                                    case 's': case SDLK_DOWN:
                                    {
                                        controls.down = true;
                                    } break;
                                }
                            } break;

                            case SDL_KEYUP:
                            {
                                switch(e.key.keysym.sym)
                                {
                                    case 'a': case SDLK_LEFT:
                                    {
                                        controls.left = false;
                                    } break;
                                    case 'd': case SDLK_RIGHT:
                                    {
                                        controls.right = false;
                                    } break;
                                    case 'w': case SDLK_UP:
                                    {
                                        controls.up = false;
                                    } break;
                                    case 's': case SDLK_DOWN:
                                    {
                                        controls.down = false;
                                    } break;
                                }
                            } break;
                        }
                    }
                    // Rendering
                    update_and_render(controls, first_time, game_state);
                    SDL_RenderPresent(pRenderer);
                    first_time = false;

                    // Fixing frame rate
                    int EndTime = SDL_GetTicks();
                    int FrameDelta = EndTime - StartTime;
                    int TimeToSleep = TargetMS - FrameDelta;

                    if(TimeToSleep > 0)
                    {
                        SDL_Delay(TimeToSleep);
                    }

                }
            }
        }
        return 0;
    }
}