#ifndef GAME_SHOW_H
#define GAME_SHOW_H
#include "../head/GameState.h"
#include "../head/GameCore.h"
#include "../head/GamePlayers.h"
#include <stdlib.h>
#include <stdio.h>

// Forward declaration
typedef struct GameState GameState;
typedef enum Color Color;

// Fonctions du fichier
void GR12_show_map(GameState* map);
void GR12_fancy_show_map(GameState* map);
void GR12_ASCII_title();
void GR12_begin_game(GameState* map, int player1, int player2);
void GR12_end_game(Color winner);

#endif