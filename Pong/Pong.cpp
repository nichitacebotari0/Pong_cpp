
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

			deltaFixed -= fixedTimeStep;
			DrawPlayer(Player1Pos, worldState);
			DrawPlayer(Player2Pos, worldState);
		}

		// frame time step
		Player1MoveCd -= deltaFrame;
		Player2MoveCd -= deltaFrame;
		SetConsoleCursorPosition(renderBuffer, zeroCoord);
		Render(worldState);
	}
}

void MovePlayer(Point* player, bool down)
{
	if (!down)
	{
		player->y += 1;
		if (player->y >= 18)
			player->y = 18;
	}
	else { // down
		player->y -= 1;
		if (player->y <= 1)
			player->y = 1;
	}
}

void DrawPlayer(Point position, char worldState[20][20])
{
	worldState[position.y - 2][position.x] = ' '; // maybe look into another way to decide on z-index. probably not needed tho, just draw ball last
	worldState[position.y - 1][position.x] = '|';
	worldState[position.y][position.x] = '|';
	worldState[position.y + 1][position.x] = '|';
	worldState[position.y + 2][position.x] = ' ';
}

void Render(char worldState[20][20])
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			cout << worldState[i][j];
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