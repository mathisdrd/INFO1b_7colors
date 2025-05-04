#ifndef GAME_CORE_H
#define GAME_CORE_H
#include "../head/GameState.h"
#include "../head/GameShow.h"
#include <stdlib.h>
#include <stdio.h>

// Forward declaration
typedef struct GameState GameState;
typedef enum Color Color;

void GR12_update_map (GameState* map, Color player, Color move);
void GR12_copy_game_state (GameState* state, GameState* copy);
void GR12_playable_colors(GameState* map, Color player, int* playable_colors);
int GR12_reachable_squares(GameState* map, Color player);
int GR12_player_area(GameState* map, Color player);
Color GR12_win_game(GameState* map);
void GR12_classic_game(GameState* map, int size);
void GR12_statistics(GameState* map, int size);
void GR12_full_statistics(GameState* map, int size);
#endif