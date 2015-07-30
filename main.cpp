#pragma once
#include "common.h"
#include "Snake.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

COORD printApple(int minX, int maxX, int minY, int maxY, vector<COORD> cannotTouch)
{
	bool clash;
	int randX, randY;
	do
	{
		clash = FALSE;
		randX = rand()%(maxX-minX+1) + minX;
		randY = rand()%(maxY-minY+1) + minY;
		for(int i = 0; i < cannotTouch.size(); i++)
		{
			if(randX == cannotTouch[i].X && randY == cannotTouch[i].Y)
			{
				clash = TRUE;
				break;
			}
		}
	} while(clash);
	COORD applePos = {randX, randY};
	SetConsoleCursorPosition(hConsole, applePos);
	putchar('@');
	return applePos;
}

int main()
{
	/* Set screen size */
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole,&info);
	short maxWinSizeX = info.dwMaximumWindowSize.X;
	short maxWinSizeY = info.dwMaximumWindowSize.Y;
	short winSizeX = 25;
	short winSizeY = 25;
	SMALL_RECT windowSize = {0, 0, winSizeX-1, winSizeY-1};
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
	COORD dwSize = {winSizeX, winSizeY};
	SetConsoleScreenBufferSize(hConsole, dwSize);

	/* Draw border around console window */
	// Top & bottom border
	for(int i = 0; i < winSizeX; i++)
	{
		COORD posTop = {i, 0};
		SetConsoleCursorPosition(hConsole, posTop);
		putchar('#');
		COORD posBottom = {i, winSizeY-2};
		SetConsoleCursorPosition(hConsole, posBottom);
		putchar('#');
	}
	// Left & right border
	for(int i = 1; i < winSizeY-2; i++)
	{
		COORD posLeft = {0, i};
		SetConsoleCursorPosition(hConsole, posLeft);
		putchar('#');
		COORD posRight = {winSizeX-1, i};
		SetConsoleCursorPosition(hConsole, posRight);
		putchar('#');
	}

	/* Usable coords for snake and apple to occupy is 1 to winSizeX-2 in horizontal direction
	and 1 to winSizeY-3 in vertical direction. */
	Snake mySnake = Snake(1, winSizeX-2, 1, winSizeY-3, hConsole);
	bool appleEaten = FALSE;
	COORD applePos = printApple(1, winSizeX-2, 1, winSizeY-3, mySnake.doNotTouch());
	mySnake.print();

	srand(time(NULL));
	do
	{
		/* Detect keyboard hit from user */
		int nextDir = -1;
		if(_kbhit())
		{
			char keyStroke;
			keyStroke = _getch();
			if(keyStroke == -32)
			{
				keyStroke = _getch();
				switch(keyStroke)
				{
				case 72:
					nextDir = UP;
					break;
				case 75:
					nextDir = LEFT;
					break;
				case 77:
					nextDir = RIGHT;
					break;
				case 80:
					nextDir = DOWN;
					break;
				default:
					break;
				}
			}
		}

		/* Move snake */
		int nextMove = mySnake.move(nextDir, applePos);
		if(nextMove == APPLE_EATEN)
			appleEaten = TRUE;
		else if(nextMove == GAME_OVER)
			break;

		/* Randomly generate position of apple */
		if(appleEaten)
		{
			applePos = printApple(1, winSizeX-2, 1, winSizeY-3, mySnake.doNotTouch());
			appleEaten = FALSE;
		}

		/* Generate position of snake */
		mySnake.print();

		Sleep(200);
	} while(TRUE);

	COORD pos = {2, 2};
	SetConsoleCursorPosition(hConsole, pos);

	system("PAUSE");
	return 0;
}