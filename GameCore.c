#include "../head/GameCore.h"

const char* GR12_strcolors[7] = {"Rouge", "Vert", "Bleu", "Jaune", "Magenta", "Cyan", "Orange"};
extern Color (*GR12_players[6])(GameState*, Color);

char* (GR12_strplayers[6]) = {
    "Human", 
    "AI Random",
    "AI Random+",
    "AI Greedy",
    "AI Hegemonic",
    "AI Mixed",
};

void GR12_update_map (GameState* map, Color player, Color move){
    int updates = 0;
    for (int i=0; i<(map->size); i++){
		for (int j=0; j<(map->size); j++){
            if (get_map_value(map, i, j) == player){
                // On check les adjacents avec une vérification paresseuse pour ne pas dépasser la map.
                if (i > 0           && get_map_value(map, i-1, j) == move) {set_map_value(map, i-1, j, player); updates++;}
                if (i < map->size-1 && get_map_value(map, i+1, j) == move) {set_map_value(map, i+1, j, player); updates++;}
                if (j > 0           && get_map_value(map, i, j-1) == move) {set_map_value(map, i, j-1, player); updates++;}
                if (j < map->size-1 && get_map_value(map, i, j+1) == move) {set_map_value(map, i, j+1, player); updates++;}
            }
        }
	}
    
    // Si on a fait des modifications, il faut reparcourir la map.
    if(updates){GR12_update_map(map, player, move);}
}

void GR12_copy_game_state (GameState* state, GameState* copy){
    // On créé le GameState copy avec la taille de state
    create_empty_game_state (copy, state->size);

    // On parcourt state pour prendre chaque valeur et la mettre dans copy
    for (int i=0; i<(state->size); i++){
		for (int j=0; j<(state->size); j++){
            set_map_value(copy, i, j, get_map_value(state, i, j));
        }
	}
}

void GR12_playable_colors (GameState* map, Color player, int* playable_colors){
    // On initialise le tableau à modifier
    for (int i=0; i<7; i++){playable_colors[i] = 0;}
    
    for (int i=0; i<(map->size); i++){
		for (int j=0; j<(map->size); j++){
            if (get_map_value(map, i, j) == player){
                // On check les adjacents avec une vérification paresseuse pour ne pas dépasser la map.
                if (i > 0           && (get_map_value(map, i-1, j) != PLAYER_1 || get_map_value(map, i-1, j) != PLAYER_2)) {playable_colors[get_map_value(map, i-1, j) - 3] = 1;}
                if (i < map->size-1 && (get_map_value(map, i+1, j) != PLAYER_1 || get_map_value(map, i+1, j) != PLAYER_2)) {playable_colors[get_map_value(map, i+1, j) - 3] = 1;}
                if (j > 0           && (get_map_value(map, i, j-1) != PLAYER_1 || get_map_value(map, i, j-1) != PLAYER_2)) {playable_colors[get_map_value(map, i, j-1) - 3] = 1;}
                if (j < map->size-1 && (get_map_value(map, i, j+1) != PLAYER_1 || get_map_value(map, i, j+1) != PLAYER_2)) {playable_colors[get_map_value(map, i, j+1) - 3] = 1;}
            }
        }
	}
}

int GR12_reachable_squares(GameState* map, Color player){
    int count = 0; // On compte le nombre de cases atteignables
    // On créé une liste qui va garder en mémoire les cases atteignables, 0 : pas atteignable, 1 atteignable
    int* reachable_squares = (int*)malloc(map->size*map->size*sizeof(int));
    for (int i=0; i<map->size*map->size; i++){reachable_squares[i] = 0;}
    

    for (int i=0; i<(map->size); i++){
		for (int j=0; j<(map->size); j++){
            if (get_map_value(map, i, j) == player){
                // On check les adjacents avec une évaluation paresseuse pour ne pas dépasser la map.
                if (i > 0           && (get_map_value(map, i-1, j) != PLAYER_1 && get_map_value(map, i-1, j) != PLAYER_2) && 0 == reachable_squares[(i-1) + map->size*j]) {reachable_squares[(i-1) + map->size*j] = 1; count++;}
                if (i < map->size-1 && (get_map_value(map, i+1, j) != PLAYER_1 && get_map_value(map, i+1, j) != PLAYER_2) && 0 == reachable_squares[(i+1) + map->size*j]) {reachable_squares[(i+1) + map->size*j] = 1; count++;}
                if (j > 0           && (get_map_value(map, i, j-1) != PLAYER_1 && get_map_value(map, i, j-1) != PLAYER_2) && 0 == reachable_squares[i + map->size*(j-1)]) {reachable_squares[i + map->size*(j-1)] = 1; count++;}
                if (j < map->size-1 && (get_map_value(map, i, j+1) != PLAYER_1 && get_map_value(map, i, j+1) != PLAYER_2) && 0 == reachable_squares[i + map->size*(j+1)]) {reachable_squares[i + map->size*(j+1)] = 1; count++;}
            }
        }
	}

    // On free le tableau alloué avec malloc()
    free(reachable_squares);
	reachable_squares = NULL;

    return count;
}

int GR12_player_area (GameState* map, Color player){
    int count = 0;

    // On parcourt toute la map pour savoir l'étendue du territoires du joueur.
    for (int i=0; i<(map->size); i++){
		for (int j=0; j<(map->size); j++){
            if (get_map_value(map, i, j) == player){count++;}
        }
	}
    
    return count;
}

Color GR12_win_game (GameState* map){
    // Détermination de l'état de la partie : EMPTY : Partie non finie, PLAYER_X : Joueur X a gagné, RED ou autre couleur : égalité

    // On récupère la partie du territoire conquis par chaque utilisateur
    int count_P1 = GR12_player_area(map, PLAYER_1);
    int count_P2 = GR12_player_area(map, PLAYER_2);

    // Si l'un des joueurs à +50% de la map, victoire
    if (2*count_P1 > (map->size*map->size)){return PLAYER_1;}
    if (2*count_P2 > (map->size*map->size)){return PLAYER_2;}

    // A partir d'ici les joueurs ont 50% ou moins de la map.
    // Si les deux joueurs ont 50% de la map, égalité
    if (count_P1 == count_P2 && 2*count_P1 == (map->size*map->size)){return EMPTY;}

    // A partir d'ici les deux joueurs ont moins de 50% du terrain
    // On récupère le nombre de couleurs jouables pour savoir si le joueur peut encore jouer
    int playable_P1[7]; GR12_playable_colors(map, PLAYER_1, playable_P1);
    int playable_P2[7]; GR12_playable_colors(map, PLAYER_2, playable_P2);
    
    int nb_colors_P1 = 0;
    int nb_colors_P2 = 0;

    // On compte le nombre de couleurs jouables
    for (int i=0; i<7; i++){
        nb_colors_P1 += playable_P1[i];
        nb_colors_P2 += playable_P2[i];
    }

    // Si un joueur est en dessous de 50% et ne peut plus jouer de coup, il a forcément perdu
    if (0 == nb_colors_P2){return PLAYER_1;}
    if (0 == nb_colors_P1){return PLAYER_2;}

    // A partir d'ici les deux joueurs ont moins de 50% de la map et peuvent jouer des coups.
    // La partie n'est pas finie (Personne n'a la majorité et des coups peuvent encore être joué)
    return RED;
}

void GR12_classic_game(GameState* map, int size){
    int verif; // Pour vérifier que les scanf aboutissent

    // ## Choix des joueurs #####################
    // On définir les indices des choix de joueurs
    int player1, player2;

    // On demande à l'utilisateur de choisir les types de joueurs
    printf("\n\033[47m  \033[0m Choix du premier joueur (premier à jouer) :\n"); // Couleur PLAYER_1
    printf("   [1] : Humain, [2] : IA aléatoire, [3] : IA aléatoire+, [4] : IA gloutonne, [5] : IA hégémonique, [6] : IA mixte\n\n-> ");
    do {
		verif = scanf("%d", &player1);
		if (0 == verif || player1 < 1 || player1 > 6){
			printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (1 <= x <= 6).\n        Veuillez réessayer :\n\n-> ");
		}
	} while (0 == verif || player1 < 1 || player1 > 6);
    
    player1 -= 1; // L'indicage des listes commence à 0

    printf("\n\033[48;5;0m  \033[0m Choix du second joueur :\n"); // Couleur PLAYER_2
    printf("   [1] : Humain, [2] : IA aléatoire, [3] : IA aléatoire+, [4] : IA gloutonne, [5] : IA hégémonique, [6] : IA mixte\n\n-> ");
    do {
		verif = scanf("%d", &player2);
		if (0 == verif || player2 < 1 || player2 > 6){
			printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (1 <= x <= 6).\n        Veuillez réessayer :\n\n-> ");
		}
	} while (0 == verif || player2 < 1 || player2 > 6);
    
    player2 -= 1;
    // ## Fin choix des joueurs #################

    // ## Choix de la taille de la grille #######
    if (size == -1){
        printf("\nChoix de la taille de la grille : \033[30m(Côté de la grille entre 2 et 300)\033[0m\n\n-> ");
        verif = scanf("%d", &size);
    }

    while (0 == verif || size < 2 || size > 300){
		printf("\n[\033[31mERROR\033[0m] La taille choisit doit être un entier compris entre 2 et 300.\n        Veuillez entrer une taille valable :\n\n-> ");
        verif = scanf("%d", &size);
		//if (0 == verif || size < 2 || size > 300){
			
		//}
	}
    // ## Fin choix de la taille de la grille ### 

    
    // ## Corps du jeu ##########################
    // On génère la carte.
    create_empty_game_state(map, size);
    // On remplit la carte.
    fill_map(map);

    GR12_begin_game(map, player1, player2); // Affichage

    int turn_to = 0;
    Color winner = RED;
    Color color; // Pour la couleur jouée par les joueurs
    while (!(winner == EMPTY || winner == PLAYER_1 || winner == PLAYER_2)){
        switch(turn_to){
            case 0:
                printf("\n\033[47m  \033[0m Au joueur 1 de jouer :\n\n");
                // Le joueur choisit un couleur à jouer
                color = GR12_players[player1](map, PLAYER_1);

                // On affiche le coup joué :
                printf("\033[47m  \033[0m Le joueur 1 a joué %s\n\n", GR12_strcolors[color - 3]);
                // On update la map selon le coup choisit
                GR12_update_map(map, PLAYER_1, color);
                break; // Pour quitter le switch
            case 1: 
                printf("\n\033[48;5;0m  \033[0m Au joueur 2 de jouer :\n\n");
                color = GR12_players[player2](map, PLAYER_2);
                printf("\033[48;5;0m  \033[0m Le joueur 2 a joué %s\n\n", GR12_strcolors[color - 3]);
                GR12_update_map(map, PLAYER_2, color);
                break;
        }
        // On met à jour le potentiel gagnant
        winner = GR12_win_game(map);

        GR12_fancy_show_map(map); // Si on remplace GR12_fancy_show_map par GR12_show_map, il faut également modifier le bloc fancy dans le joueur humain et l'appel de fonction en dernière ligne dans la fonction GR12_begin_game
        printf("\n");
        // On passe au tour du prochain joueur
        turn_to ^= 1; // On pourrait aussi écrire : turn_to = (turn_to + 1) % 2;
    }

    GR12_end_game(winner); 
    // ## Fin corps du jeu ######################
}

void GR12_statistics(GameState* map, int size){
    int verif; // Pour vérifier que les scanf aboutissent

    // ## Choix des joueurs #####################
    // On définir les indices des choix de joueurs
    int player1, player2;

    // On demande à l'utilisateur de choisir les types de joueurs
    printf("\n\033[47m  \033[0m Choix du premier joueur (premier à jouer) :\n"); // Couleur PLAYER_1
    printf("   [1] : Humain, [2] : IA aléatoire, [3] : IA aléatoire+, [4] : IA gloutonne, [5] : IA hégémonique, [6] : IA mixte\n\n-> ");
    do {
		verif = scanf("%d", &player1);
		if (0 == verif || player1 < 1 || player1 > 6){
			printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (1 <= x <= 6).\n        Veuillez réessayer :\n\n-> ");
		}
	} while (0 == verif || player1 < 1 || player1 > 6);
    
    player1 -= 1; // L'indicage des listes commence à 0

    printf("\n\033[48;5;0m  \033[0m Choix du second joueur :\n"); // Couleur PLAYER_2
    printf("   [1] : Humain, [2] : IA aléatoire, [3] : IA aléatoire+, [4] : IA gloutonne, [5] : IA hégémonique, [6] : IA mixte\n\n-> ");
    do {
		verif = scanf("%d", &player2);
		if (0 == verif || player2 < 1 || player2 > 6){
			printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (1 <= x <= 6).\n        Veuillez réessayer :\n\n-> ");
		}
	} while (0 == verif || player2 < 1 || player2 > 6);
    
    player2 -= 1;
    // ## Fin choix des joueurs #################

    // ## Choix de la taille de la grille #######
    if (size == -1){
        printf("\nChoix de la taille de la grille : \033[30m(Côté de la grille entre 2 et 300)\033[0m\n\n-> ");
        verif = scanf("%d", &size);
    }

    while (0 == verif || size < 2 || size > 300){
		verif = scanf("%d", &size);
		if (0 == verif || size < 2 || size > 300){
			printf("\n[\033[31mERROR\033[0m] La taille choisit doit être un entier compris entre 2 et 300.\n        Veuillez entrer une taille valable :\n\n-> ");
		}
	}
    // ## Fin choix de la taille de la grille ###

    // ## Choix du nombre de parties ############
    printf("\nChoix du nombre de parties : \033[30m(Entre 1 et 2000)\033[0m\n\n-> ");
    int rounds;
    do {
		verif = scanf("%d", &rounds);
		if (0 == verif || rounds < 1 || rounds > 2000){
			printf("\n[\033[31mERROR\033[0m] Le nombre de parties doit être un entier compris entre 1 et 2000.\n        Veuillez entrer un nombre valable :\n\n-> ");
		}
	} while (0 == verif || rounds < 1 || rounds > 2000);
    printf("\n");
    // ## Fin choix du nombre de parties ########

    // ## Corps #################################
    // On génère la carte.
    create_empty_game_state(map, size);

    // Pour les statistiques
    int wins_player1 = 0;
    int wins_player2 = 0;
    int draws = 0;

    // On réalise les parties
    int turn_to = 0; // Pour savoir à quel joueur est-ce le tour.
    Color color; // Pour la couleur jouée par les joueurs
    Color winner = RED; // Pour désigner le gagnant

    for (int i=0; i<rounds; i++){
        // On (ré)initialise la carte et le gagnant
        fill_map(map);
        winner = RED;

        // On fait un tour
        while (!(winner == EMPTY || winner == PLAYER_1 || winner == PLAYER_2)){
            switch(turn_to){
                case 0:
                    // Le joueur choisit un couleur à jouer
                    color = GR12_players[player1](map, PLAYER_1);
                    GR12_update_map(map, PLAYER_1, color);
                    break; // Pour quitter le switch
                case 1:
                    color = GR12_players[player2](map, PLAYER_2);
                    GR12_update_map(map, PLAYER_2, color);
                    break;
                default : printf("[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); break;
            }
            // On met à jour le potentiel gagnant
            winner = GR12_win_game(map);

            // On passe au tour du prochain joueur
            turn_to ^= 1; // On pourrait aussi écrire : turn_to = (turn_to + 1) % 2;
        }
        if (PLAYER_1 == winner){wins_player1++;}
        if (PLAYER_2 == winner){wins_player2++;}
        if (EMPTY == winner){draws++;}
    }
    // ## Fin corps #############################

    // ## Statistiques ##########################
    float winrate_player1 = 100.0 * wins_player1 / rounds;
    float winrate_player2 = 100.0 * wins_player2 / rounds;
    if (winrate_player1 > winrate_player2){
        printf("Pour %d parties, le joueur 1 a le plus gagné avec un winrate de %f%% et %d parties gagnées (%d égalités), %d\n", rounds, winrate_player1, wins_player1, draws, wins_player2);
    } else {
        printf("Pour %d parties, le joueur 2 a le plus gagné avec un winrate de %f%% et %d parties gagnées (%d égalités), %d\n", rounds, winrate_player2, wins_player2, draws, wins_player1);
    }
    // ## Fin statistiques ######################
}

void GR12_full_statistics(GameState* map, int size){
    int verif=1; // Pour vérifier que les scanf aboutissent

    int player1, player2;

    // ## Choix de la taille de la grille #######
    if (size == -1){
        printf("\nChoix de la taille de la grille : \033[30m(Côté de la grille entre 2 et 300)\033[0m\n\n-> ");
        verif = scanf("%d", &size);
    }

    while (0 == verif || size < 2 || size > 300){
		verif = scanf("%d", &size);
		if (0 == verif || size < 2 || size > 300){
			printf("\n[\033[31mERROR\033[0m] La taille choisit doit être un entier compris entre 2 et 300.\n        Veuillez entrer une taille valable :\n\n-> ");
		}
	}
    // ## Fin choix de la taille de la grille ###

    // ## Choix du nombre de parties ############
    printf("\nChoix du nombre de parties : \033[30m(Entre 1 et 2000)\033[0m\n\n-> ");
    int rounds;
    do {
		verif = scanf("%d", &rounds);
		if (0 == verif || rounds < 1 || rounds > 2000){
			printf("\n[\033[31mERROR\033[0m] Le nombre de parties doit être un entier compris entre 1 et 2000.\n        Veuillez entrer un nombre valable :\n\n-> ");
		}
	} while (0 == verif || rounds < 1 || rounds > 2000);
    printf("\n");
    // ## Fin choix du nombre de parties ########

    // ## Corps #################################
    // On génère la carte.
    create_empty_game_state(map, size);

    // Pour les statistiques
    int wins_player1 = 0;
    int wins_player2 = 0;
    int draws = 0;

    // On réalise les parties
    int turn_to = 0; // Pour savoir à quel joueur est-ce le tour.
    Color color; // Pour la couleur jouée par les joueurs
    Color winner = RED; // Pour désigner le gagnant

    for (int j=1; j<6; j++){
        for (int k=1; k<6; k++){
            if (k==j){continue;}
            player1 = j;
            player2 = k;
            wins_player1 = 0;
            wins_player2 = 0;
            draws = 0;

            for (int i=0; i<rounds; i++){
                // On (ré)initialise la carte et le gagnant
                fill_map(map);
                winner = RED;
        
                // On fait un tour
                while (!(winner == EMPTY || winner == PLAYER_1 || winner == PLAYER_2)){
                    switch(turn_to){
                        case 0:
                            // Le joueur choisit un couleur à jouer
                            color = GR12_players[player1](map, PLAYER_1);
                            GR12_update_map(map, PLAYER_1, color);
                            break; // Pour quitter le switch
                        case 1:
                            color = GR12_players[player2](map, PLAYER_2);
                            GR12_update_map(map, PLAYER_2, color);
                            break;
                        default : printf("[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); break;
                    }
                    // On met à jour le potentiel gagnant
                    winner = GR12_win_game(map);
        
                    // On passe au tour du prochain joueur
                    turn_to ^= 1; // On pourrait aussi écrire : turn_to = (turn_to + 1) % 2;
                }
                if (PLAYER_1 == winner){wins_player1++;}
                if (PLAYER_2 == winner){wins_player2++;}
                if (EMPTY == winner){draws++;}
            }
            // ## Fin corps #############################
        
            // ## Statistiques ##########################
            float winrate_player1 = 100.0 * wins_player1 / rounds;
        
            printf("Pour %d parties, %s VS %s, \tle joueur %s\t a un winrate de %f%%\t et a gagné %d parties\t vs %d\t (%d égalités)\n", rounds, GR12_strplayers[j], GR12_strplayers[k], GR12_strplayers[j], winrate_player1, wins_player1, wins_player2, draws);
        }
    }
}