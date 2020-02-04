//==============================================================================
#include "CApp.h"


void CApp::MoveSprites()
{
	int i;
	int window_w = WINDOW_WIDTH;
	int window_h = WINDOW_HEIGHT;
	SDL_Rect *position, *velocity;

	/* Draw a gray background */
	SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderClear(renderer);

	/* Move the sprite, bounce at the wall, and draw */
	for (i = 0; i < NUM_SPRITES; ++i)
	{

		position = &positions[i];
		velocity = &velocities[i];
		if (i != 0)
		{
			//if(  )
		}
		/* Blit the sprite onto the screen */
		if (i == 0)
		{
			SDL_RenderCopy(renderer, sprite1, NULL, position);
		}
		else if (i == 1)
		{
			SDL_RenderCopy(renderer, sprite2, NULL, position);
		}
		else if (i == 2)
		{
			positions[2].x += 5;
			SDL_RenderCopy(renderer, sprite3, NULL, position);
		}
		else if (i == 3)
		{
			positions[3].x -= 5;
			SDL_RenderCopy(renderer, sprite4, NULL, position);
		}

	}

	SDL_RenderPresent(renderer);
}


//==============================================================================
void CApp::OnLoop()
{

	if (joy) //Joystick control for image [0] and locked in y position
	{
		positions[0].y += SDL_JoystickGetAxis(joy, 1) / 6000;

		if (positions[0].y > WINDOW_HEIGHT - sprite_h)
			positions[0].y = WINDOW_HEIGHT - sprite_h;
		if (positions[0].y < 0)
			positions[0].y = 0;
	}


    MoveSprites();



}

//==============================================================================
