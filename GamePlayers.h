#ifndef GAME_PLAYERS_H
#define GAME_PLAYERS_H
#include "../head/GameState.h"
#include "../head/GameShow.h"
#include "../head/GameCore.h"
#include <stdlib.h>
#include <stdio.h>

// Forward declaration
typedef struct GameState GameState;
typedef enum Color Color;

// Variables externes à importer


// Fonctions du fichier
Color GR12_player_human(GameState* map, Color player);
Color GR12_player_AI_random(GameState* map, Color player);
Color GR12_player_AI_random_plus(GameState* map, Color player);
Color GR12_player_AI_greedy(GameState* map, Color player);
Color GR12_player_AI_hegemonic(GameState* map, Color player);
Color GR12_player_AI_mixed(GameState* map, Color player);

#endif