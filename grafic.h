#pragma once
#include <vector>
#include <string>
struct Griver {
	int x;
	int y;
};
void initGrafic(int windowWidth=900,int windowHeight =700);
void cleanGrafic();
bool isWindowOpen();
char getMoveInput();
void delayFrame(float second = 0.04f);
void drawMap(
	const std::vector<std::string>& map,
	int playerX,
	int playerY,
	bool isPlayerAlive,
	bool isNight,
	bool isliana,
	const std::vector<Griver>& grivers,
	int level,
	const std::vector<bool>& collectedMaps
);
void drawDeathScreen(const std::string& reason);
void drawLockedSector(const std::vector<bool>& collectedMaps);
void drawWinScreen();