#include "grafic.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
	enum Color {
		color_player=6,
		color=7,
		color_floor=8,
		color_liana=10,
		color_exit=11,
		color_griver=12,
		color_map=14,
		color_wall=15
	};
	void setColor(Color color) {
#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAtribute(hConsole, color);
#else
		switch (color) {
		case color_player: std::cout << "\033[33m" <<break;
		case color_floor: std::cout << "\033[90m" << break;
		case color_liana: std::cout << "\033[92m" << break;
		case color_exit: std::cout << "\033[96m" << break;
		case color_griver: std::cout << "\033[91m" << break;
		case color_map: std::cout << "\033[93m" << break;
		case color_wall: std::cout << "\033[97m" << break;
		default: std::cout << "\033[0m" << break;
		}
#endif
	}

	void resetCursor() {
#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos = { 0,0 };
		SetConsoleCursorPosition(hConsole, pos);
#else
		std::cout << "\033[H";
#endif
	}
}
void initGrafic() {
#ifdef _WIN32
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}