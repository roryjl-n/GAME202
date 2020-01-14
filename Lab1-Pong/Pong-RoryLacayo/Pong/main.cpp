/*
Name: Rory Lacayo
Course: Object Oriented Programing
Prof: Majid 
Title of Program: Game Programing
Date: Jan/2020
*/

//including this SDL Library into our program.
#include "SDL.h"
#include <cstdlib>

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
SDL_Rect BallTwo;
SDL_Rect MiddleLine;

//
SDL_Event event;

//Mouse coordinates
int mouse_x, mouse_y;

//Store x and y speeds of the ball and array to store two direction
int speed_x, speed_y;
int direction[2] = { -1, 1 };

//Collition detection function between two rectangular objects (Ball & PlayerPaddle/Ball & AIPaddle)
bool check_collision(SDL_Rect A, SDL_Rect B)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	//Calculate the sides of rect B
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;
	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

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
	if (!window) {
		return;
	}

	//
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		return;
	}

	//Setting the start location and the size of the player's paddle.
	PlayerPaddle.x = 20;
	PlayerPaddle.y = 250;
	PlayerPaddle.h = 100;
	PlayerPaddle.w = 20;

	//Setting the start location and the size of the AI's paddle.
	AIPaddle.x = 760;
	AIPaddle.y = 250;
	AIPaddle.h = 100;
	AIPaddle.w = 20;

	//Setting the location and the size of the pong ball.
	Ball.x = 370;
	Ball.y = 290;
	Ball.w = 20;
	Ball.h = 20;

	//Second Ball
	BallTwo.x = 370;
	BallTwo.y = 270;
	BallTwo.w = 20;
	BallTwo.h = 20;

	MiddleLine.x = 395;
	MiddleLine.y = 8;
	MiddleLine.w = 8;
	MiddleLine.h = 580;

	//Initialize speed variables
	speed_x = -1;
	speed_y = -1;
}

/*
Purpose of the Input function:
*/

void Input()
{
	//Queuing events
	while (SDL_PollEvent(&event)) {

		// Track mouse movement 
		if (event.type == SDL_MOUSEMOTION)
			SDL_GetMouseState(&mouse_x, &mouse_y);

		//Clicking 'x' or pressing F4
		if (event.type == SDL_QUIT)
			running = false;

		//Pressing a key
		if(event.type == SDL_KEYDOWN)
			switch (event.key.keysym.sym)
			{

				//Pressing ESC exits from the game
			    case SDLK_ESCAPE:
					running = false;
					break;
			}
	}
}

/*
Purpose of the Update function:
*/

void Update()
{
	PlayerPaddle.y = mouse_y;

	 //ball goes out on sides, left and right
	//reset to centre of screen
	if (Ball.x < 0 || Ball.x > WINDOW_WIDTH)
	{
		Ball.x = WINDOW_WIDTH / 2;
		Ball.y = WINDOW_HEIGHT / 2;
		//this expression produces random numbers -1, -2, 1 and 2
		speed_x = (rand() % 2 + 1) * direction[rand() % 2];
		speed_y = (rand() % 2 + 1) * direction[rand() % 2];
	}

	//check if the ball's position is above(y less than zero) the top wall or 
	// lower(y greater than WINDOW_HEIGHT) than the bottom wall.
	//If so reverse the speed in the y direction
	if (Ball.y < 0 || Ball.y > (WINDOW_HEIGHT - Ball.h))
	{
		speed_y = -speed_y;
	}

	//Alligns center of both the AI's paddle and the ball
	AIPaddle.y = Ball.y - AIPaddle.h/2 + Ball.h/2;
	
	//Continually check if the Ball & PlayerPaddle/Ball & AIPaddle collide
	if (check_collision(Ball, AIPaddle) || check_collision(Ball, PlayerPaddle))
	{
		speed_x = -speed_x;
	}
	
	//Continually update the Ball's x and y position by the amount speed
	Ball.x += speed_x;
	Ball.y += speed_y;

	SDL_Delay(1);
}

/*
Purpose of the DrawScreen function:
*/

void DrawScreen()
{
	//
	SDL_RenderClear(renderer);

	//Draw and color the background of the game
	SDL_Rect background = { 0, 0, 800, 600 };
	SDL_SetRenderDrawColor(renderer, 250, 250, 250, 250);
	SDL_RenderFillRect(renderer, &background);

	//Draw and color the player's paddle, AI's paddle, and the Middle Line
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1);
	SDL_RenderFillRect(renderer, &PlayerPaddle);
	SDL_RenderFillRect(renderer, &AIPaddle);
	SDL_RenderFillRect(renderer, &MiddleLine);

	//Draw and color the ball
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
	SDL_RenderFillRect(renderer, &Ball);
	SDL_RenderFillRect(renderer, &BallTwo);

	//
	SDL_RenderPresent(renderer);
}

/*
Purpose of the Quit function:
*/

void Quit()
{
	//function to end(kill) the program.
	SDL_Quit();
}

/*
Purpose of the Main function:
*/

int main(int argc, char *argv[])
{
	//function to start the program(game).
	LoadGame();

	//keeps the loop(game) running.
	while (running == true)
	{
		Input();       //
		Update();      //
		DrawScreen();  //
	}

	//function to end(kill) the program(game).
	Quit();

	system("PAUSE");
	return 0;
}
