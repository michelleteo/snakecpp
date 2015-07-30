#pragma once
#include "common.h"

class Snake
{
private:
	vector<char> snakeBody;
	vector<COORD> snakeCoords;
	HANDLE hConsole;
	int direction;
	COORD oldCoord;
	int minX, maxX, minY, maxY;
public:
	Snake(int minX, int maxX, int minY, int maxY, HANDLE hConsole);
	~Snake();
	void print();
	int move(int direction, COORD applePos);
	vector<COORD> doNotTouch();
};