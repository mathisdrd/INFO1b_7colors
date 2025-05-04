#include "../head/GameState.h"

GameState state = {.map = NULL, .size = 0};

const Color GR12_colors[7] = {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE};

void create_empty_game_state (GameState* state, int size){
	state -> size = size;
	state -> map = (Color*)malloc(size*size*sizeof(Color));
}

void set_map_value (GameState* state, int x, int y, Color value){
	if (state -> map == NULL || x >= state -> size || y >= state -> size || x < 0 || y < 0){
		printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)\n", state -> map, state -> size, x, y);
	} else{
		state -> map[y * state -> size + x] = value;
	}
}

Color get_map_value (GameState* state, int x, int y){
	// En prenant pour parti qu'une size de 10 permet des cases allant de 0 à 9, il faut mettre x>= et non x> et de même pour y.
	if (state -> map == NULL || x >= state -> size || y >= state -> size || x < 0 || y < 0){
		printf("[ERROR] map not big enough or not initialized %p %i access (%i %i)\n", state -> map, state -> size, x, y);
		return ERROR;
	}
	return state -> map[y * state -> size + x];
}

void fill_map (GameState* map){
	// On parcourt l'entièreté de la map
	for (int i=0; i<(map->size*map->size); i++){
		// On rempli la carte aléatoirement avec les colors.
		// Le random est initialisé dans le main de ce fichier pour l'entiereté du projet
		map->map[i] = GR12_colors[rand() %7];
	}

	// On place les joueurs
	map->map[(map->size-1)*map->size] = PLAYER_1;
	map->map[(map->size-1)] = PLAYER_2;
}

int main (int argc, char** argv){
	int verif; // Pour vérifier que les scanf aboutissent
	int exit; // Pour déterminer la sortie du main, s'il y a eu une erreur ou non.

	// On initialise la graine de l'aléatoire avec le temps actuel.
	srand(time(NULL)); 

	// Titre du jeu
    GR12_ASCII_title();

	// On récupère la taille si elle a été donnée en argument
	int size = -1;
	if (argc > 1){
		size = atoi(argv[1]);
	}

	// Choix du mode de jeu
	printf("Choisissez un mode de jeu : \033[30m(Rentrer 1 ou 2)\033[0m\n");
	printf("[1] : Jeu classique (une partie)\n[2] : Statistiques\n[3] : Statistiques complètes\n\n-> ");
	int choice;
	do {
		verif = scanf("%d", &choice);
		if (0 == verif || !(choice == 1 || choice == 2 || choice == 3)){
			printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (1 ou 2).\n        Veuillez réessayer\n\n-> ");
		}
	} while (0 == verif || !(choice == 1 || choice == 2 || choice == 3));

	switch(choice){
        case 1: GR12_classic_game(&state, size); exit=1; break;
        case 2: GR12_statistics(&state, size); exit=1; break;
        case 3: GR12_full_statistics(&state, size); exit=1; break;
		default: printf("\n\n[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); exit = 0; break;
	}

	// Free de la map allouée avec malloc()
	free(state.map);
	state.map = NULL;

	return exit;
}

