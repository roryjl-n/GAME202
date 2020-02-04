//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnEvent(SDL_Event* Event) 
{
		switch (Event->type)
		{
		case SDL_QUIT:
		case SDL_KEYDOWN:
			Running = false;
			break;
		case SDL_JOYBUTTONDOWN:
			if (Event->jbutton.which == 0 && Event->jbutton.button == 0) // Button 0 on Joystick will make image[2] spawn wherever image[0]
			{
				positions[2].x = positions[0].x + 60;
				positions[2].y = positions[0].y + sprite_h;
				positions[2].w = sprite_w;
				positions[2].h = sprite_h;
				velocities[2].x = 0;
				velocities[2].y = 0;
			}
			else if (Event->jbutton.which == 0 && Event->jbutton.button == 1)  // Button 1 on Joystick
			{
				Running = false;
			}
			break;
		case SDL_MOUSEMOTION: //mouse control for image [1] and locked in y position
			positions[1].y = Event->motion.y;
			if (positions[1].y + sprite_h > WINDOW_HEIGHT)
			{
				positions[1].y = WINDOW_HEIGHT - sprite_h;
			}
			break;
		case SDL_MOUSEBUTTONDOWN: //left click on mouse will make image[3] spawn wherever image[1] is
			if (SDL_BUTTON_LEFT)
			{
				positions[3].x = positions[1].x - 25;
				positions[3].y = positions[1].y;
				positions[3].w = sprite_w;
				positions[3].h = sprite_h;
				velocities[3].x = 5;
				velocities[3].y = 0;
			}
			else if (SDL_BUTTON_RIGHT)
			{
				Running = false;
			}
		}
}


//==============================================================================
