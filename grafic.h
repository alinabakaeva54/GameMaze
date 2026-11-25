#pragma once
#include <vector>
#include <string>
struct Griver {
	int x;
	int y;
};
void initGrafic();
void drawMap(
	const std::vector<std::string>& map,
	int playerX,
	int playerY,
	bool isliana,
	const std::vector<Griver>& grivers,
	int health,
	int score
);
void cleanGrafic();