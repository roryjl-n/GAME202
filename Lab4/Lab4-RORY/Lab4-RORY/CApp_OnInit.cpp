//==============================================================================
#include "CApp.h"
#include <stdlib.h>
#include <time.h>
//==============================================================================
bool CApp::OnInit()
{

	window = SDL_CreateWindow("Basic SDL 2 Sprites", 100, 100,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK)<0)
    {
        return false;
    }
    window = SDL_CreateWindow("Basic SDL 2.0 Sprites",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
	if (!window)
	{
		return false;
	}

    renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		return false;
	}

	if (LoadSprite("alien.bmp", sprite1) < 0)
	{
		return false;
	}

	positions[0].x = 0;
	positions[0].y = WINDOW_HEIGHT / 2 - sprite_h / 2;
	positions[0].w = sprite_w;
	positions[0].h = sprite_h;
	velocities[0].x = 0;
	velocities[0].y = 0;

	if (LoadSprite("cannon.bmp", sprite2) < 0)
	{
		return false;
	}

	positions[1].x = WINDOW_WIDTH - sprite_w;
	positions[1].y = WINDOW_HEIGHT / 2 - sprite_h / 2;
	positions[1].w = sprite_w;
	positions[1].h = sprite_h;
	velocities[1].x = 0;
	velocities[1].y = 0;

	if (LoadSprite("fireball.bmp", sprite3) < 0)
	{
		return false;
	}

	if (LoadSprite("cannonball.bmp", sprite4) < 0)
	{
		return false;
	}


    return true;
}

//==============================================================================
