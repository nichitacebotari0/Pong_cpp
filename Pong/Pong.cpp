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

const struct Point Player1Pos(0, 10);
const struct Point Player2Pos(19, 10);
const struct Point BallPos(10, 10);

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
	COORD zeroCoord  = { 0, 0 };
	//hidecursor();

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
		ReadConsoleInput(inputConsole, &inp, 1, &num_of_events);

		switch (inp.EventType)
		{
		case KEY_EVENT:
			switch (inp.Event.KeyEvent.wVirtualKeyCode)
			{
			case 0x57:
				std::cout << "W\n";
				break;

			case 0x41:
				std::cout << "A\n";
				break;

			case 0x53:
				std::cout << "S\n";
				break;

			case 0x44:
				std::cout << "D\n";
				break;

			}
		}



		auto frameStartTime = chrono::steady_clock::now();
		deltaFrame = frameStartTime - timePoint;
		deltaFixed += deltaFrame;
		timePoint = frameStartTime;

		while (deltaFixed > fixedTimeStep)
		{
			// fixed time step
			deltaFixed -= fixedTimeStep;
			DrawPlayer(Player1Pos, worldState);
			DrawPlayer(Player2Pos, worldState);
		}
		// frame time step

		/*SetConsoleCursorPosition(renderBuffer, zeroCoord);
		Render(worldState);*/
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
