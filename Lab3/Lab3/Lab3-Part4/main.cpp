#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL.h"

#ifdef __NDS__
#define WINDOW_WIDTH    256
#define WINDOW_HEIGHT   (2*192)
#else
#define WINDOW_WIDTH    512
#define WINDOW_HEIGHT   384
#endif
#define NUM_SPRITES     4
#define MAX_SPEED       1
#define WW2 (WINDOW_WIDTH>>1)
#define WH2 (WINDOW_HEIGHT>>1)

static SDL_Texture *sprite;
static SDL_Texture *sprite1; //ALIEN
static SDL_Texture *sprite2; //CANNON
static SDL_Texture *sprite3; //FIREBALL
static SDL_Texture *sprite4; //CANNONBALL
static SDL_Rect positions[NUM_SPRITES];
static SDL_Rect velocities[NUM_SPRITES];
static int sprite_w, sprite_h;
SDL_Joystick *joy = NULL;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
	exit(rc);
}

int
LoadSprite(char *file, SDL_Renderer *renderer, SDL_Texture* &someSprite)
{
	SDL_Surface *temp;

	/* Load the sprite image */
	temp = SDL_LoadBMP(file);
	if (temp == NULL)
	{
		fprintf(stderr, "Couldn't load %s: %s", file, SDL_GetError());
		return (-1);
	}
	sprite_w = temp->w;
	sprite_h = temp->h;

	/* Set transparent pixel as the pixel at (0,0) */
	if (temp->format->palette)
	{
		SDL_SetColorKey(temp, SDL_TRUE, *(Uint8 *)temp->pixels);
	}
	else
	{
		switch (temp->format->BitsPerPixel)
		{
		case 15:
			SDL_SetColorKey(temp, SDL_TRUE,
				(*(Uint16 *)temp->pixels) & 0x00007FFF);
			break;
		case 16:
			SDL_SetColorKey(temp, SDL_TRUE, *(Uint16 *)temp->pixels);
			break;
		case 24:
			SDL_SetColorKey(temp, SDL_TRUE,
				(*(Uint32 *)temp->pixels) & 0x00FFFFFF);
			break;
		case 32:
			SDL_SetColorKey(temp, SDL_TRUE, *(Uint32 *)temp->pixels);
			break;
		}
	}

	/* Create textures from the image */
	someSprite = SDL_CreateTextureFromSurface(renderer, temp);
	if (!someSprite)
	{
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(temp);
		return (-1);
	}
	SDL_FreeSurface(temp);

	/* We're ready to roll. :) */
	return (0);
}

void
MoveSprites(SDL_Window * window, SDL_Renderer * renderer)
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
			position->x += velocity->x;
			if ((position->x < 0) || (position->x >= (window_w - sprite_w)))
			{
				velocity->x = -velocity->x;
				position->x += velocity->x;
			}
			position->y += velocity->y;
			if ((position->y < 0) || (position->y >= (window_h - sprite_h)))
			{
				velocity->y = -velocity->y;
				position->y += velocity->y;
			}
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
			SDL_RenderCopy(renderer, sprite3, NULL, position);
		}
		else if (i == 3)
		{
			SDL_RenderCopy(renderer, sprite4, NULL, position);
		}
	}

	/* Update the screen! */
	SDL_RenderPresent(renderer);
}

int
main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int i, done;
	SDL_Event event;


	window = SDL_CreateWindow("Basic SDL 2 Sprites", 100, 100,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (!window) {
		quit(2);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		quit(2);
	}

	if (LoadSprite("alien.bmp", renderer, sprite1) < 0)
	{
		quit(2);
	}
	if (LoadSprite("cannon.bmp", renderer, sprite2) < 0)
	{
		quit(2);
	}
	if (LoadSprite("fireball.bmp", renderer, sprite3) < 0)
	{
		quit(2);
	}
	if (LoadSprite("cannonball.bmp", renderer, sprite4) < 0)
	{
		quit(2);
	}
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	if (SDL_NumJoysticks() > 0)
	{
		joy = SDL_JoystickOpen(0);

	}

	/* Initialize the sprite positions */
	srand(time(NULL));
	for (i = 0; i < NUM_SPRITES; ++i)
	{
		positions[i].x = rand() % (WINDOW_WIDTH - sprite_w);
		positions[i].y = rand() % (WINDOW_HEIGHT - sprite_h);
		positions[i].w = sprite_w;
		positions[i].h = sprite_h;
		velocities[i].x = 0;
		velocities[i].y = 0;
		while (!velocities[i].x && !velocities[i].y)
		{
			velocities[i].x = (rand() % (MAX_SPEED * 2 + 1)) - MAX_SPEED;
			velocities[i].y = (rand() % (MAX_SPEED * 2 + 1)) - MAX_SPEED;
		}
	}

	/* Main render loop */
	done = 0;
	while (!done)
	{
		/* Check for events */
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			case SDL_KEYDOWN:
				done = 1;
				break;
			case SDL_JOYDEVICEREMOVED:
				printf("BYE-BYE JOYSTICK");
				printf("Joy number %d\n", event.jdevice.which);
			case SDL_JOYBUTTONDOWN:
				printf("Joy number %d\n", event.jbutton.which);//prints the joystick we are using to press the buttons
				printf("Button %d\n", event.jbutton.button); //prints the joystick button pressed
				if (event.jbutton.which == 0 && event.jbutton.button == 0) // Button 0 on 1st Joystick
				{
					positions[0].x = 0;
					positions[0].y = 0;
				}
				else if (event.jbutton.which == 0 && event.jbutton.button == 1)  // Button 1 on 1st Joystick
				{
					done = 1;
				}
				break;

			case SDL_JOYAXISMOTION: //Prints the x and y coordinates
				if (event.jaxis.axis == 0)
				{
					printf("x = %d\n", event.jaxis.value);
				}
				else if (event.jaxis.axis == 1)
				{
					printf("y = %d\n", event.jaxis.value);
				}



				break;

			}

		}
		if (joy) //P1 Joystick control for image [0]
		{

			positions[0].x += SDL_JoystickGetAxis(joy, 0) / 6000;
			positions[0].y += SDL_JoystickGetAxis(joy, 1) / 6000;
			if (positions[0].x > WINDOW_WIDTH - sprite_w)
				positions[0].x = WINDOW_WIDTH - sprite_w;
			if (positions[0].x < 0)
				positions[0].x = 0;
			if (positions[0].y > WINDOW_HEIGHT - sprite_h)
				positions[0].y = WINDOW_HEIGHT - sprite_h;
			if (positions[0].y < 0)
				positions[0].y = 0;



		}
	
		MoveSprites(window, renderer);
		SDL_Delay(10);
	}

	quit(0);

	return 0;
}

/* vi: set ts=4 sw=4 expandtab: */

