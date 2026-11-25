#include "grafic.h"
#include "raylib.h"
#include <deque>
#include <cmath>

namespace {
    const int cell_size = 44;

    const Color color_player = { 241, 196, 15, 255 };  
    const Color color_floor = { 18, 20, 26, 255 };    
    const Color color_trail = { 9, 10, 14, 255 };    
    const Color color_grid = { 28, 31, 40, 255 };   
    const Color color_liana = { 46, 204, 113, 255 };  
    const Color color_exit = { 26, 188, 156, 255 }; 
    const Color color_griver = { 231, 76, 60, 255 };   
    const Color color_map = { 255, 215, 0, 255 };   
    const Color color_wall = { 72, 78, 92, 255 };    
    const Color color_wall_hi = { 105, 114, 132, 255 }; 
    const Color color_night = { 4, 6, 14, 165 };      

    struct TrailPoint {
        int x;
        int y;
        float alpha;
    };
    std::deque<TrailPoint> trail;
    int prevPlayerX = -1;
    int prevPlayerY = -1;
    int prevLevel = -1;
}
void initGrafic(int windowWidth, int windowHeight) {
    InitWindow(windowWidth, windowHeight, "The Maze Runner: Tomb Edition");
    SetTargetFPS(60);
    trail.clear();
}
void cleanGrafic() {
    trail.clear();
    if (IsWindowReady()) {
        CloseWindow();
    }
}
bool isWindowOpen() {
    return !WindowShouldClose();
}
char getMoveInput() {
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    return 'W';
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  return 'S';
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  return 'A';
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) return 'D';
    return 0;
}
void delayFrame(float seconds) {
    WaitTime(seconds);
}
void drawMap(
    const std::vector<std::string>& map,
    int playerX,
    int playerY,
    bool isPlayerAlive,
    bool isLiana,
    bool isNight,
    const std::vector<Griver>& grivers,
    int level,
    const std::vector<bool>& collectedMaps
) {
    if (level != prevLevel || (prevPlayerX != -1 && (std::abs(playerX - prevPlayerX) > 1 || std::abs(playerY - prevPlayerY) > 1))) {
        trail.clear();
    }
    if (prevPlayerX != -1 && (playerX != prevPlayerX || playerY != prevPlayerY)) {
        trail.push_back({ prevPlayerX, prevPlayerY, 1.0f });
        if (trail.size() > 7) trail.pop_front();
    }
    prevPlayerX = playerX;
    prevPlayerY = playerY;
    prevLevel = level;

    for (auto& pt : trail) pt.alpha -= 0.045f;
    while (!trail.empty() && trail.front().alpha <= 0.0f) trail.pop_front();

    BeginDrawing();
    ClearBackground(color_floor);

    DrawRectangle(0, 0, GetScreenWidth(), 60, { 12, 14, 18, 255 });
    DrawLine(0, 60, GetScreenWidth(), 60, color_wall);

    DrawText(TextFormat("SECTOR %d", level), 25, 18, 24, RAYWHITE);
    DrawText("MAPS:", 185, 20, 20, LIGHTGRAY);

    int visibleSlots = (level > 3) ? 3 : level;
    if (visibleSlots > static_cast<int>(collectedMaps.size())) {
        visibleSlots = static_cast<int>(collectedMaps.size());
    }

    for (int i = 0; i < visibleSlots; ++i) {
        int slotX = 260 + i * 52;
        Rectangle slotRect = { static_cast<float>(slotX), 14.0f, 40.0f, 32.0f };
        if (collectedMaps[i]) {
            DrawRectangleRounded(slotRect, 0.3f, 6, color_map);
            DrawText(TextFormat("%d", i + 1), slotX + 15, 20, 20, BLACK);
        }
        else {
            DrawRectangleRoundedLines(slotRect, 0.3f, 6, DARKGRAY);
            DrawText(TextFormat("%d", i + 1), slotX + 15, 20, 20, DARKGRAY);
        }
    }

    if (isNight) {
        DrawText("NIGHT", GetScreenWidth() - 115, 18, 24, { 130, 150, 255, 255 });
    }
    else {
        DrawText("DAY", GetScreenWidth() - 95, 18, 24, color_map);
    }

    int mapRows = static_cast<int>(map.size());
    int mapCols = mapRows > 0 ? static_cast<int>(map.size()) : 0;
    int offsetX = (GetScreenWidth() - mapCols * cell_size) / 2;
    int offsetY = 75 + (GetScreenHeight() - 75 - mapRows * cell_size) / 2;
    if (offsetY < 75) offsetY = 75;

    for (int y = 0; y < mapRows; ++y) {
        for (int x = 0; x < static_cast<int>(map[y].size()); ++x) {
            int px = offsetX + x * cell_size;
            int py = offsetY + y * cell_size;
            Rectangle cellRect = { static_cast<float>(px), static_cast<float>(py),
                                   static_cast<float>(cell_size), static_cast<float>(cell_size) };

            DrawRectangleLinesEx(cellRect, 1.0f, color_grid);

            char cell = map[y][x];
            if (cell == '#') {
                DrawRectangleRec(cellRect, color_wall);
                DrawRectangle(px + 3, py + 3, cell_size - 6, cell_size - 6, color_wall_hi);
            }
            else if (cell == 'L') {
                DrawRectangleRounded({ (float)px + 4, (float)py + 4, (float)cell_size - 8, (float)cell_size - 8 }, 0.3f, 6, color_liana);
                DrawLineEx({ (float)px + 8, (float)py + 8 }, { (float)px + cell_size - 8, (float)py + cell_size - 8 }, 3.0f, DARKGREEN);
                DrawLineEx({ (float)px + cell_size - 8, (float)py + 8 }, { (float)px + 8, (float)py + cell_size - 8 }, 3.0f, DARKGREEN);
            }
            else if (cell == 'E') {
                DrawRectangleRounded({ (float)px + 4, (float)py + 4, (float)cell_size - 8, (float)cell_size - 8 }, 0.2f, 6, color_exit);
                DrawRectangleLinesEx({ (float)px + 9, (float)py + 9, (float)cell_size - 18, (float)cell_size - 18 }, 2.0f, WHITE);
            }
            else if (cell == 'M') {
                Vector2 center = { px + cell_size / 2.0f, py + cell_size / 2.0f };
                DrawPoly(center, 4, cell_size * 0.34f, 0.0f, color_map);
                DrawPolyLines(center, 4, cell_size * 0.38f, 0.0f, WHITE);
            }
        }
    }

    for (const auto& pt : trail) {
        int tx = offsetX + pt.x * cell_size + 8;
        int ty = offsetY + pt.y * cell_size + 8;
        Color currentTrailColor = color_trail;
        currentTrailColor.a = static_cast<unsigned char>(210 * pt.alpha);
        DrawRectangleRounded({ (float)tx, (float)ty, (float)cell_size - 16, (float)cell_size - 16 }, 0.35f, 6, currentTrailColor);
    }

    if (isNight) {
        DrawRectangle(0, 61, GetScreenWidth(), GetScreenHeight() - 61, color_night);
    }

    if (isPlayerAlive) {
        int plX = offsetX + playerX * cell_size + 6;
        int plY = offsetY + playerY * cell_size + 6;
        Rectangle playerRect = { (float)plX, (float)plY, (float)cell_size - 12, (float)cell_size - 12 };

        if (isLiana) {
            DrawRectangleRounded(playerRect, 0.4f, 8, color_liana);
            DrawRectangleRoundedLines(playerRect, 0.4f, 8, DARKGREEN);
        }
        else {
            DrawRectangleRounded(playerRect, 0.4f, 8, color_player);
        }
    }

    for (const auto& g : grivers) {
        int gx = offsetX + g.x * cell_size + cell_size / 2;
        int gy = offsetY + g.y * cell_size + cell_size / 2;
        DrawCircle(gx, gy, cell_size * 0.38f, color_griver);
        DrawPolyLines({ (float)gx, (float)gy }, 6, cell_size * 0.42f, 30.0f, MAROON);
        DrawCircle(gx - 6, gy - 3, 3.0f, YELLOW);
        DrawCircle(gx + 6, gy - 3, 3.0f, YELLOW);
    }

    EndDrawing();
}

void drawDeathScreen(const std::string& reason) {
    trail.clear();
    BeginDrawing();
    ClearBackground({ 35, 10, 12, 255 });
    DrawText("YOU DIED", GetScreenWidth() / 2 - 110, 220, 48, color_griver);
    DrawText(reason.c_str(), GetScreenWidth() / 2 - MeasureText(reason.c_str(), 24) / 2, 290, 24, RAYWHITE);
    DrawText("Press R to restart sector", GetScreenWidth() / 2 - 140, 360, 20, LIGHTGRAY);
    EndDrawing();
}

void drawLockedSectorScreen(const std::vector<bool>& collectedMaps) {
    BeginDrawing();
    ClearBackground(color_floor);
    DrawText("SECTOR 4 LOCKED", GetScreenWidth() / 2 - 170, 180, 36, color_griver);
    DrawText("Collect all 3 sector maps first:", GetScreenWidth() / 2 - 175, 240, 22, RAYWHITE);
    for (int i = 0; i < 3; ++i) {
        bool ok = (i < static_cast<int>(collectedMaps.size()) && collectedMaps[i]);
        DrawText(
            TextFormat("Sector %d Map: %s", i + 1, ok ? "FOUND" : "MISSING"),
            GetScreenWidth() / 2 - 120, 290 + i * 35, 22, ok ? color_liana : color_griver
        );
    }
    EndDrawing();
}

void drawWinScreen() {
    trail.clear();
    BeginDrawing();
    ClearBackground({ 12, 35, 28, 255 });
    DrawText("YOU ESCAPED THE MAZE!", GetScreenWidth() / 2 - 230, 260, 36, color_map);
    EndDrawing();
}