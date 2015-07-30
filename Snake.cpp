#pragma once
#include "common.h"
#include "Snake.h"

Snake::Snake(int minX, int maxX, int minY, int maxY, HANDLE hConsole)
{
	COORD initPos = {(maxX-minX)/2, (maxY-minY)/2};
	this->snakeCoords.push_back(initPos);
	COORD tail = {initPos.X-1, initPos.Y};
	this->snakeCoords.push_back(tail);
	this->snakeBody.push_back('o');
	this->snakeBody.push_back('o');
	this->minX = minX;
	this->maxX = maxX;
	this->minY = minY;
	this->maxY = maxY;
	this->hConsole = hConsole;
	this->direction = RIGHT; // snake moves right initially
	this->oldCoord.X = -1;
	this->oldCoord.Y = -1;
}

Snake::~Snake()
{
}

void Snake::print()
{
	for(int i = 0; i < this->snakeBody.size(); i++)
	{
		SetConsoleCursorPosition(hConsole, this->snakeCoords[i]);
		putchar(this->snakeBody[i]);
	}
	if(this->oldCoord.X != -1 && this->oldCoord.Y != -1)
	{
		SetConsoleCursorPosition(hConsole, oldCoord);
		putchar(0);
	}
}

int Snake::move(int direction, COORD applePos)
{
	/* Snake will grow when the apple has reached its tail. */
	bool growSnake = this->snakeBody[this->snakeBody.size()-1] == 'O';

	this->oldCoord.X = this->snakeCoords[this->snakeCoords.size()-1].X;
	this->oldCoord.Y = this->snakeCoords[this->snakeCoords.size()-1].Y;

	/* Copy all coordinates towards the tail. Coordinates of the head will
	be modified according to direction of snake */
	for(int i = this->snakeCoords.size()-1; i > 0; i--)
	{
		this->snakeCoords[i] = this->snakeCoords[i-1];
		this->snakeBody[i] = this->snakeBody[i-1];
	}
	
	/* Check to see if apple is on tail. If so, grow snake. */
	if(growSnake)
	{
		this->snakeBody.push_back('o');
		COORD newTail = {this->oldCoord.X, this->oldCoord.Y};
		this->snakeCoords.push_back(newTail);
		this->oldCoord.X = -1;
		this->oldCoord.Y = -1;
	}
	
	/* Coordinates of the head will be modified according to direction of snake */
	if(direction == LEFT || direction == RIGHT)
	{
		if(this->direction == UP || this->direction == DOWN)
			this->direction = direction;
	}
	else if(direction == UP || direction == DOWN)
	{
		if(this->direction == LEFT || this ->direction == RIGHT)
			this->direction = direction;
	}
	// else no change in direction

	switch(this->direction)
	{
	case UP:
		this->snakeCoords[0].Y--;
		break;
	case DOWN:
		this->snakeCoords[0].Y++;
		break;
	case LEFT:
		this->snakeCoords[0].X--;
		break;
	case RIGHT:
		this->snakeCoords[0].X++;
		break;
	default:
		break;
	}

	/* Game over? */
	// check if head crashes on to wall
	if(this->snakeCoords[0].X == this->minX-1 || this->snakeCoords[0].X == this->maxX+1
		|| this->snakeCoords[0].Y == this->minY-1 || this->snakeCoords[0].Y == this->maxY+1)
		return GAME_OVER;
	// check also to see if head clashes with any of its body
	for(int i = 1; i < this->snakeCoords.size(); i++)
	{
		if(this->snakeCoords[0].X == this->snakeCoords[i].X && this->snakeCoords[0].Y == this->snakeCoords[i].Y)
			return GAME_OVER;
	}

	/* Apple eaten? */
	if(this->snakeCoords[0].X == applePos.X && this->snakeCoords[0].Y == applePos.Y) // ate the apple
	{
		this->snakeBody[0] = 'O';
		return APPLE_EATEN;
	}
	else
		this->snakeBody[0] = 'o';

	return NOTHING;
}

vector<COORD> Snake::doNotTouch()
{
	return this->snakeCoords;
}