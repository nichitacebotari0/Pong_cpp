
#include <iostream>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace std::chrono;

struct Point {
	Point(int xVal, int yVal)
	{
		y = xVal;
		x = yVal;
	}

	int y;
	int x;
};

struct Vector2 {
	float x;
	float y;
};


void InitWorldState(char worldState[20][20]);
void Render(char worldState[20][20]);
void DrawPlayer(Point position, char worldState[20][20]);

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
struct Point Player1Pos = Point(0, 10);
nanoseconds Player1MoveCd = duration_cast<nanoseconds>(milliseconds(0));
struct Point Player2Pos = Point(19, 10);
nanoseconds Player2MoveCd = duration_cast<nanoseconds>(milliseconds(0));


struct Point BallPos = Point(10, 10);
struct Point BallVector = Point{ -1, 0 };
const nanoseconds BallMovementCooldown = duration_cast<nanoseconds>(milliseconds(100));
nanoseconds BallMoveCd = duration_cast<nanoseconds>(milliseconds(0));

int main()
{
	// init 
	auto renderBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD zeroCoord = { 0, 0 };
	hidecursor();

	char worldState[20][20];
	InitWorldState(worldState);
	HANDLE inputConsole = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD inp;
	DWORD num_of_events;

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
			DrawPlayer(Player1Pos, worldState);
			DrawPlayer(Player2Pos, worldState);

			worldState[BallPos.x][BallPos.y] = ' ';
			if (BallMoveCd <= BallMoveCd.zero())
			{
				BallPos.y += BallVector.y;
				BallPos.x += BallVector.x;
				BallMoveCd = BallMovementCooldown;
			}
			if (BallPos.y <= 0 || BallPos.x >= 19)
			{
				BallPos.y = 10;
				BallPos.x = 10;
				BallVector.y = -1;
				BallVector.x = 0;
			}
			worldState[BallPos.x][BallPos.y] = '@';


			deltaFixed -= fixedTimeStep;
		}

		// frame time step
		Player1MoveCd -= deltaFrame;
		Player2MoveCd -= deltaFrame;
		BallMoveCd -= deltaFrame;
		SetConsoleCursorPosition(renderBuffer, zeroCoord);
		Render(worldState);
	}
}

void MovePlayer(Point* player, bool down)
{
	if (!down)
	{
		player->x += 1;
		if (player->x >= 18)
			player->x = 18;
	}
	else { // down
		player->x -= 1;
		if (player->x <= 1)
			player->x = 1;
	}
}

void DrawPlayer(Point position, char worldState[20][20])
{
	worldState[position.x - 2][position.y] = ' '; // maybe look into another way to decide on z-index. probably not needed tho, just draw ball last
	worldState[position.x - 1][position.y] = '|';
	worldState[position.x][position.y] = '|';
	worldState[position.x + 1][position.y] = '|';
	worldState[position.x + 2][position.y] = ' ';
}

void Render(char worldState[20][20])
{
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			cout << worldState[y][x];
		}
		cout << "\n";
	}
}

void InitWorldState(char worldState[20][20])
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			worldState[i][j] = ' ';
			if (i % 19 == 0)
				worldState[i][j] = '-';
		}
	}
}