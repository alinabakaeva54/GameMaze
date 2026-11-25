#include "grafic.h"
#include "raylib.h" // Только для клавиши R (рестарт в тесте)

int main() {
    initGrafic(900, 700);

    // Мини-карта для теста всех элементов:
    // # - стена, L - лиана, M - карта, E - выход
    const std::vector<std::string> startMap = {
        "##############",
        "#            #",
        "# ####  ## M #",
        "# #        # #",
        "# #  ####  # #",
        "#    #  L  #E#",
        "##############"
    };

    std::vector<std::string> map = startMap;
    int playerX = 1, playerY = 1;
    bool isPlayerAlive = true;
    bool isLiana = false;
    bool isNight = false;
    int steps = 0; // Счётчик шагов для смены дня/ночи и появления Гривера

    std::vector<Griver> grivers;
    std::vector<bool> collectedMaps = { false, false, false };
    std::string status = "PLAY"; // "PLAY", "DEAD", "WIN"
    std::string deathReason = "";

    while (isWindowOpen()) {
        // --- ЭКРАН ПОРАЖЕНИЯ ---
        if (status == "DEAD") {
            drawDeathScreen(deathReason);
            if (IsKeyPressed(KEY_R)) {
                map = startMap;
                playerX = 1; playerY = 1;
                isPlayerAlive = true; isLiana = false; isNight = false;
                steps = 0; grivers.clear();
                collectedMaps = { false, false, false };
                status = "PLAY";
            }
            continue;
        }

        // --- ЭКРАН ПОБЕДЫ ---
        if (status == "WIN") {
            drawWinScreen();
            if (IsKeyPressed(KEY_R)) {
                map = startMap;
                playerX = 1; playerY = 1;
                isPlayerAlive = true; isLiana = false; isNight = false;
                steps = 0; grivers.clear();
                collectedMaps = { false, false, false };
                status = "PLAY";
            }
            continue;
        }

        // --- УПРАВЛЕНИЕ И СКОЛЬЖЕНИЕ ---
        char move = getMoveInput();
        int dx = 0, dy = 0;
        if (move == 'W') dy = -1;
        if (move == 'S') dy = 1;
        if (move == 'A') dx = -1;
        if (move == 'D') dx = 1;

        if (dx != 0 || dy != 0) {
            while (map[playerY + dy][playerX + dx] != '#') {
                playerX += dx;
                playerY += dy;
                steps++;

                // Через 5 шагов наступает НОЧЬ и на старте появляется ГРИВЕР
                if (steps == 5) {
                    isNight = true;
                    grivers.push_back({ 1, 1 });
                }

                // 1. Врезались в лиану -> зеленеем и умираем
                if (map[playerY][playerX] == 'L') {
                    isLiana = true;
                    drawMap(map, playerX, playerY, true, true, isNight, grivers, 2, collectedMaps);
                    delayFrame(0.6f);
                    deathReason = "Caught in toxic lianas!";
                    status = "DEAD";
                    break;
                }

                // 2. Собрали карту сектора
                if (map[playerY][playerX] == 'M') {
                    map[playerY][playerX] = ' ';
                    collectedMaps[0] = true; // Загорится 1-я карта в шапке
                    collectedMaps[1] = true; // И 2-я (т.к. для теста стоит level = 2)
                }

                // 3. Дошли до выхода
                if (map[playerY][playerX] == 'E') {
                    drawMap(map, playerX, playerY, true, false, isNight, grivers, 2, collectedMaps);
                    delayFrame(0.3f);
                    status = "WIN";
                    break;
                }

                // Отрисовка каждого шага скольжения со шлейфом
                drawMap(map, playerX, playerY, true, false, isNight, grivers, 2, collectedMaps);
                delayFrame(0.04f);
            }

            // Простой шаг Гривера к игроку после рывка (для проверки съедения)
            if (status == "PLAY" && !grivers.empty()) {
                if (grivers[0].x < playerX) grivers[0].x++;
                else if (grivers[0].x > playerX) grivers[0].x--;
                else if (grivers[0].y < playerY) grivers[0].y++;
                else if (grivers[0].y > playerY) grivers[0].y--;

                if (grivers[0].x == playerX && grivers[0].y == playerY) {
                    isPlayerAlive = false; // Игрок исчезает
                    drawMap(map, playerX, playerY, false, false, isNight, grivers, 2, collectedMaps);
                    delayFrame(0.7f);
                    deathReason = "Eaten by a Griver!";
                    status = "DEAD";
                }
            }
        }

        if (status == "PLAY") {
            drawMap(map, playerX, playerY, isPlayerAlive, isLiana, isNight, grivers, 2, collectedMaps);
        }
    }

    cleanGrafic();
    return 0;
}