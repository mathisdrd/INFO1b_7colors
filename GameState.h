#ifndef GAME_STATE_H
#define GAME_STATE_H
#include "../head/GameShow.h"
#include "../head/GameCore.h"
#include "../head/GamePlayers.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef enum Color{
	ERROR = -1,
	EMPTY,
	PLAYER_1,
	PLAYER_2,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	WHITE
}Color; 

typedef struct GameState{
	Color* map;
	int size;
} GameState;


void create_empty_game_state (GameState* state, int size);
void set_map_value (GameState* state, int x, int y, Color value);
Color get_map_value (GameState* state, int x, int y);
void fill_map (GameState* state);
int main (int argc, char** argv);
#endif