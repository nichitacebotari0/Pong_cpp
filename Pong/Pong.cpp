
#include <iostream>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace std::chrono;

struct Point {
	Point(int xVal, int yVal)
	{
		x = xVal;
		y = yVal;
	}

	int x;
	int y;
};

struct Vector2 {
	float x;
	float y;
};

const USHORT xLen = 60;
const USHORT yLen = 20;

void InitWorldState(char worldState[yLen][xLen]);
void Render(char worldState[yLen][xLen]);
void DrawPlayer(Point position, char worldState[yLen][xLen]);

void MovePlayer(Point* player, bool up);

void hidecursor()
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(output, &info);
}

const nanoseconds PlayerMovementCooldown = duration_cast<nanoseconds>(milliseconds(100));
struct Point Player1Pos = Point(0, yLen / 2);
nanoseconds Player1MoveCd = duration_cast<nanoseconds>(milliseconds(0));
struct Point Player2Pos = Point(xLen - 1, yLen / 2);
nanoseconds Player2MoveCd = duration_cast<nanoseconds>(milliseconds(0));

struct Point BallPos = Point(xLen / 2, yLen / 2);
struct Point BallVector = Point(-1, 0);
const nanoseconds BallMovementCooldown = duration_cast<nanoseconds>(milliseconds(100));
nanoseconds BallMoveCd = duration_cast<nanoseconds>(milliseconds(0));

int main()
{
	// init 
	auto renderBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD zeroCoord = { 0, 0 };
	hidecursor();

	char worldState[yLen][xLen];
	InitWorldState(worldState);
	HANDLE inputConsole = GetStdHandle(STD_INPUT_HANDLE);

	// loop
	auto fixedTimeStep = microseconds(4ms);
	auto timePoint = steady_clock::now();
	auto deltaFrame = timePoint - timePoint;
	auto deltaFixed = timePoint - timePoint;
	while (true)
	{
		auto frameStartTime = chrono::steady_clock::now();
		deltaFrame = frameStartTime - timePoint;
		deltaFixed += deltaFrame;
		timePoint = frameStartTime;

		worldState[BallPos.y][BallPos.x] = ' ';
		while (deltaFixed > fixedTimeStep)
		{
			// fixed time step
			if (GetAsyncKeyState('W') & 0x8000 && Player1MoveCd <= Player1MoveCd.zero())
			{
				MovePlayer(&Player1Pos, true);
				Player1MoveCd = PlayerMovementCooldown;
			}
			if (GetAsyncKeyState('S') & 0x8000 && Player1MoveCd <= Player1MoveCd.zero())
			{
				MovePlayer(&Player1Pos, false);
				Player1MoveCd = PlayerMovementCooldown;
			}

			if (GetAsyncKeyState(VK_UP) & 0x8000 && Player2MoveCd <= Player2MoveCd.zero())
			{
				MovePlayer(&Player2Pos, true);
				Player2MoveCd = PlayerMovementCooldown;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000 && Player2MoveCd <= Player2MoveCd.zero())
			{
				MovePlayer(&Player2Pos, false);
				Player2MoveCd = PlayerMovementCooldown;
			}

			if (BallMoveCd <= BallMoveCd.zero())
			{
				BallPos.y += BallVector.y;
				BallPos.x += BallVector.x;
				if (BallPos.x == 0)
				{
					bool ballHitPlayer1 = BallPos.y <= Player1Pos.y + 1 && BallPos.y >= Player1Pos.y - 1;
					if (ballHitPlayer1)
					{
						BallVector.x = -BallVector.x;

						BallPos.x += BallVector.x;
						BallPos.y += BallVector.y;
					}
				}
				if (BallPos.x == xLen - 1)
				{
					bool ballHitPlayer2 = BallPos.y <= Player2Pos.y + 1 && BallPos.y >= Player2Pos.y - 1;
					if (ballHitPlayer2)
					{
						BallVector.x = -BallVector.x;

						BallPos.x += BallVector.x;
						BallPos.y += BallVector.y;
					}
				}

				BallMoveCd = BallMovementCooldown;
			}

			if (BallPos.x < 0 || BallPos.x > xLen - 1)
			{
				BallPos.y = yLen/2;
				BallPos.x = xLen / 2;
				BallVector.x = -1;
				BallVector.y = 0;
			}

			deltaFixed -= fixedTimeStep;
		}
		DrawPlayer(Player1Pos, worldState);
		DrawPlayer(Player2Pos, worldState);
		worldState[BallPos.y][BallPos.x] = '@';

		// frame time step
		Player1MoveCd -= deltaFrame;
		Player2MoveCd -= deltaFrame;
		BallMoveCd -= deltaFrame;
		SetConsoleCursorPosition(renderBuffer, zeroCoord);

		Render(worldState);
	}
}

void MovePlayer(Point* player, bool up)
{
	if (up)
	{
		player->y += 1;
		if (player->y >= yLen - 2) // account for player width of 1
			player->y = yLen - 2;
	}
	else {
		player->y -= 1;
		if (player->y <= 1) // account for player width of 1
			player->y = 1;
	}
}

void DrawPlayer(Point position, char worldState[yLen][xLen])
{
	worldState[position.y - 2][position.x] = ' '; // maybe look into another way to decide on z-index. probably not needed tho, just draw ball last
	worldState[position.y - 1][position.x] = '|';
	worldState[position.y][position.x] = '|';
	worldState[position.y + 1][position.x] = '|';
	worldState[position.y + 2][position.x] = ' ';
}

void Render(char worldState[yLen][xLen])
{
	for (int y = yLen - 1; y >= 0; y--)
	{
		for (int x = 0; x < xLen; x++)
		{
			cout << worldState[y][x];
		}
		cout << "\n";
	}
}

void InitWorldState(char worldState[yLen][xLen])
{
	for (int y = 0; y < yLen; y++)
	{
		for (int x = 0; x < xLen; x++)
		{
			worldState[y][x] = ' ';
			if (y % (yLen - 1) == 0)
				worldState[y][x] = '-';
		}
	}
}