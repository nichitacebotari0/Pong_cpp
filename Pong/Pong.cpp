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

struct Point Player1Pos = Point(0, 10);
struct Point Player2Pos = Point(19, 10);
struct Point BallPos = Point(10, 10);

#include <iostream>
#include <chrono>
#include <windows.h>


void hidecursor()
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(output, &info);
}

using namespace std;
using namespace std::chrono;

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
		ReadConsoleInput(inputConsole, &inp, 1, &num_of_events); // TODO: switch to non-blocking way of reading input

		while (deltaFixed > fixedTimeStep)
		{
			switch (inp.EventType)
			{
			case KEY_EVENT:
				switch (inp.Event.KeyEvent.wVirtualKeyCode)
				{
				case 0x57:
					Player1Pos.y += 1;
					Player1Pos.y %= 18;
					break;

				case 0x53:
					Player1Pos.y -= 1;
					if (Player1Pos.y < 1)
						Player1Pos.y = 1;
					break;
				}
			}


			// fixed time step
			deltaFixed -= fixedTimeStep;
			DrawPlayer(Player1Pos, worldState);
			DrawPlayer(Player2Pos, worldState);
		}
		// frame time step

		SetConsoleCursorPosition(renderBuffer, zeroCoord);
		Render(worldState);
	}
}

/// <summary>
/// Doesn't actually draw.
/// </summary>
void DrawPlayer(Point position, char worldState[20][20])
{
	worldState[position.y - 1][position.x] = '|';
	worldState[position.y][position.x] = '|';
	worldState[position.y + 1][position.x] = '|';
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
