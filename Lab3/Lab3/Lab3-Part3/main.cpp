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
#define NUM_SPRITES     10
#define MAX_SPEED       1
#define WW2 (WINDOW_WIDTH>>1)
#define WH2 (WINDOW_HEIGHT>>1)

static SDL_Texture *sprite;
static SDL_Rect positions[NUM_SPRITES];
static SDL_Rect velocities[NUM_SPRITES];
static int sprite_w, sprite_h;
SDL_Joystick *joy = NULL;
SDL_Joystick *joy2 = NULL;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
	exit(rc);
}

int
LoadSprite(char *file, SDL_Renderer *renderer)
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
	sprite = SDL_CreateTextureFromSurface(renderer, temp);
	if (!sprite)
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
MoveSprites(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture * sprite)
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
		SDL_RenderCopy(renderer, sprite, NULL, position);
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

	if (LoadSprite("hts.bmp", renderer) < 0)
	{
		quit(2);
	}
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	if (SDL_NumJoysticks() > 0)
	{
		joy = SDL_JoystickOpen(0);
		joy2 = SDL_JoystickOpen(0);

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
			case SDL_JOYBUTTONDOWN:
				printf("Mouse moved by %d/n", event.jbutton.button);
				if (event.jbutton.which == 0 && event.jbutton.button == 0) // Button 1 on 1st Joystick
				{
					positions[0].x = 0;
					positions[0].y = 0;
				}
				else if (event.jbutton.which == 0 && event.jbutton.button == 1)  // Button 2 on 1st Joystick
				{
					done = 1;
				}
				break;
			
				/*
				case SDL_JOYAXISMOTION:
				switch(event.jaxis.which)
				{
				case 0:
				positions[0].x += event.jaxis.value / 6000;
				if (positions[0].x>WINDOW_WIDTH-sprite_w)
				positions[0].x = WINDOW_WIDTH-sprite_w;
				if (positions[0].x< 0)
				positions[0].x = 0;
				break;
				case 1:
				positions[0].y += event.jaxis.value / 6000;
				if (positions[0].y>WINDOW_HEIGHT-sprite_h)
				positions[0].y = WINDOW_HEIGHT-sprite_h;
				if (positions[0].y< 0)
				positions[0].y = 0;
				break;
				}
				break;
				*/
			}

		}
		if (joy)
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

			printf("x= %d,y=%d\n", positions[0].x, positions[0].y);

		}
		if (joy2)
		{

			positions[0].x += SDL_JoystickGetAxis(joy2, 0) / 6000;
			positions[0].y += SDL_JoystickGetAxis(joy2, 1) / 6000;
			if (positions[0].x > WINDOW_WIDTH - sprite_w)
				positions[0].x = WINDOW_WIDTH - sprite_w;
			if (positions[0].x < 0)
				positions[0].x = 0;
			if (positions[0].y > WINDOW_HEIGHT - sprite_h)
				positions[0].y = WINDOW_HEIGHT - sprite_h;
			if (positions[0].y < 0)
				positions[0].y = 0;

		}

		MoveSprites(window, renderer, sprite);
		SDL_Delay(10);
	}

	quit(0);

	return 0;
}

/* vi: set ts=4 sw=4 expandtab: */

