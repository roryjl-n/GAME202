/*
Name:
Course:
Prof:
Title of Program:
Date:
*/

//including this SDL Library into our program.
#include "SDL.h"

//defining the size of our game window.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//
SDL_Renderer *renderer;

//setting the program as true so it runs automatically 
bool running = true;

//
SDL_Rect PlayerPaddle;
SDL_Rect AIPaddle;
SDL_Rect Ball;

void LoadGame()
{
	//
	SDL_Window *window;

	//
	window = SDL_CreateWindow("Basic SDL Sprites",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (!renderer) {
		return;
	}

	//Setting the location and the size of the player's paddle.
	PlayerPaddle.x = 20;
	PlayerPaddle.y = 250;
	PlayerPaddle.h = 100;
	PlayerPaddle.w = 20;

	//Setting the location and the size of the AI's paddle.
	AIPaddle.x = 760;
	AIPaddle.y = 250;
	AIPaddle.h = 100;
	AIPaddle.w = 20;

	//Setting the location and the size of the pong ball.
	Ball.x = 370;
	Ball.y = 290;
	Ball.w = 20;
	Ball.h = 20;

}

/*
Purpose of the Input function:
*/

void Input()
{

}

/*
Purpose of the Update function:
*/

void Update()
{

}

/*
Purpose of the DrawScreen function:
*/

void DrawScreen()
{
	//
	SDL_RenderClear(renderer);

	//
	SDL_Rect background = { 0, 0, 800, 600 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &background);

	//
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &PlayerPaddle);
	SDL_RenderFillRect(renderer, &AIPaddle);

	//
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &Ball);

	//
	SDL_RenderPresent(renderer);
}

/*
Purpose of the Quit function:
*/

void Quit()
{
	//
	SDL_Quit();
}

/*
Purpose of the Main function:
*/

int main(int argc, char *argv[])
{
	//
	LoadGame();

	//
	while (running == true)
	{
		Input();       //
		Update();      //
		DrawScreen();  //
	}

	//
	Quit();

	system("PAUSE");
	return 0;
}
