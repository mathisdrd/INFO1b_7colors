#include "../head/GameShow.h"

extern Color (*GR12_players[6])(GameState*, Color);

void GR12_show_map(GameState* map){
  	for (int i=0; i<(map->size); i++){
		for (int j=0; j<(map->size); j++){
            Color value = get_map_value(map, i, j);
			switch(value){
				case PLAYER_1 : printf("^ "); 																		  break;
				case PLAYER_2 : printf("v "); 																		  break;
				case RED :      printf("A "); 																		  break;
				case GREEN :    printf("B "); 																		  break;
				case BLUE :     printf("C "); 																		  break;
				case YELLOW :   printf("D "); 																		  break;
				case MAGENTA :  printf("E "); 																		  break;
				case CYAN :     printf("F "); 																		  break;
				case WHITE :    printf("G "); 																		  break;
				default : 		printf("\n\n[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); break;
			}
        }
        printf("\n");
	}
}

void GR12_fancy_show_map(GameState* map){
	// Affichage de l'avancée :
	int nb_pixel_P1 = GR12_player_area(map, PLAYER_1) * 2/map->size;
	int nb_pixel_P2 = GR12_player_area(map, PLAYER_2) * 2/map->size;

	printf("⟡"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⩤⩥"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⟡\n");
	printf("|");
	for(int i=0; i<nb_pixel_P2; i++){printf("\033[48;5;0m \033[0m");}
	for(int i=0; i<((2*map->size) - nb_pixel_P1 - nb_pixel_P2); i++){printf(" ");}
	for(int i=0; i<nb_pixel_P1; i++){printf("\033[47m \033[0m");}
	printf("|\n");
	printf("⟡"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⩤⩥"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⟡\n");
	// Fin affichage de l'avancée

	// Affichage de la map
	printf("⟡"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⩤⩥"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⟡\n");
	for (int i=0; i<(map->size); i++){
		if (map->size % 2){if (i == map->size/2){printf("⟠");}else {printf("|");}}
		else {if (i == map->size/2-1){printf("∆");} else if (i == map->size/2){printf("∇");} else {printf("|");}}
		for (int j=0; j<(map->size); j++){
			Color value = get_map_value(map, i, j);
			switch(value){
				case PLAYER_1 : printf("\033[47m  \033[0m");       												  break;
				case PLAYER_2 : printf("\033[48;5;0m  \033[0m");   												  break;
				case RED :      printf("\033[41m  \033[0m");       												  break;
				case GREEN :    printf("\033[42m  \033[0m");       												  break;
				case BLUE :     printf("\033[44m  \033[0m");       												  break;
				case YELLOW :   printf("\033[43m  \033[0m");       												  break;
				case MAGENTA :  printf("\033[45m  \033[0m");       												  break;
				case CYAN :     printf("\033[106m  \033[0m");      												  break;
				case WHITE :    printf("\033[48;5;208m  \033[0m"); 												  break;
				default : 		printf("\n\n[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); break;
			}
		}
		if (map->size % 2){if (i == map->size/2){printf("⟠\n");}else {printf("|\n");}}
		else {if (i == map->size/2-1){printf("∆\n");} else if (i == map->size/2){printf("∇\n");} else {printf("|\n");}}
	}
	printf("⟡"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⩤⩥"); for(int i=0; i<(map->size)-1; i++){printf("∼");} printf("⟡\n");
}

void GR12_ASCII_title(){
	printf(" _____         _\n|___  |__ ___ | | ___  _ __ ___\n   / / __/ _ \\| |/ _ \\| '__/ __|\n  / / (_| (_) | | (_) | |  \\__ \\\n /_/ \\___\\___/|_|\\___/|_|  |___/\n\n");
}

void GR12_begin_game(GameState* map, int player1, int player2){
	char* strplayer1;
  	char* strplayer2;
  	switch(player1){
    	case 0 :
      		strplayer1 = " _    _                             \n| |  | |                            \n| |__| |_   _ _ __ ___   __ _ _ __  \n|  __  | | | | '_ ` _ \\ / _` | '_ \\ \n| |  | | |_| | | | | | | (_| | | | |\n|_|  |_|\\__,_|_| |_| |_|\\__,_|_| |_|\n";
      		break;
    	case 1 :
      		strplayer1 = " _____                 _                 \n|  __ \\               | |                \n| |__) |__ _ _ __   __| | ___  _ __ ___  \n|  _  // _` | '_ \\ / _` |/ _ \\| '_ ` _ \\ \n| | \\ \\ (_| | | | | (_| | (_) | | | | | |\n|_|  \\_\\__,_|_| |_|\\__,_|\\___/|_| |_| |_|\n";
    		break;
		case 2 :
			strplayer1 = " ____                 _                       \n|  _ \\ __ _ _ __   __| | ___  _ __ ___    _   \n| |_) / _` | '_ \\ / _` |/ _ \\| '_ ` _ \\ _| |_ \n|  _ < (_| | | | | (_| | (_) | | | | | |_   _|\n|_| \\_\\__,_|_| |_|\\__,_|\\___/|_| |_| |_| |_|\n";
			break;
		case 3 :
			strplayer1 = "  ____                   _       \n / ___|_ __ ___  ___  __| |_   _ \n| |  _| '__/ _ \\/ _ \\/ _` | | | |\n| |_| | | |  __/  __/ (_| | |_| |\n \\____|_|  \\___|\\___|\\__,_|\\__, |\n						   |___/\n";
			break;
		case 4 :
			strplayer1 = " _   _                                       _      \n| | | | ___  __ _  ___ _ __ ___   ___  _ __ (_) ___ \n| |_| |/ _ \\/ _` |/ _ \\ '_ ` _ \\ / _ \\| '_ \\| |/ __|\n|  _  |  __/ (_| |  __/ | | | | | (_) | | | | | (__ \n|_| |_|\\___|\\__, |\\___|_| |_| |_|\\___/|_| |_|_|\\___|\n			|___/\n";
			break;
		case 5 :
			strplayer1 = " __  __ _              _ \n|  \\/  (_)_  _____  __| |\n| |\\/| | \\ \\/ / _ \\/ _` |\n| |  | | |>  <  __/ (_| |\n|_|  |_|_/_/\\_\\___|\\__,_|\n";
			break;
		default:
			strplayer1 = "Oups";
			break;
  	}

  	switch(player2){
    	case 0 :
      		strplayer2 = "\t\t _    _                             \n\t\t| |  | |                            \n\t\t| |__| |_   _ _ __ ___   __ _ _ __  \n\t\t|  __  | | | | '_ ` _ \\ / _` | '_ \\ \n\t\t| |  | | |_| | | | | | | (_| | | | |\n\t\t|_|  |_|\\__,_|_| |_| |_|\\__,_|_| |_|\n";
      		break;
    	case 1 :
      		strplayer2 = "\t\t _____                 _                 \n\t\t|  __ \\               | |                \n\t\t| |__) |__ _ _ __   __| | ___  _ __ ___  \n\t\t|  _  // _` | '_ \\ / _` |/ _ \\| '_ ` _ \\ \n\t\t| | \\ \\ (_| | | | | (_| | (_) | | | | | |\n\t\t|_|  \\_\\__,_|_| |_|\\__,_|\\___/|_| |_| |_|\n";
    		break;
		case 2 :
			strplayer2 = "\t\t ____                 _                       \n\t\t|  _ \\ __ _ _ __   __| | ___  _ __ ___    _   \n\t\t| |_) / _` | '_ \\ / _` |/ _ \\| '_ ` _ \\ _| |_ \n\t\t|  _ < (_| | | | | (_| | (_) | | | | | |_   _|\n\t\t|_| \\_\\__,_|_| |_|\\__,_|\\___/|_| |_| |_| |_|\n";
			break;
		case 3 :
			strplayer2 = "\t\t  ____                   _       \n\t\t / ___|_ __ ___  ___  __| |_   _ \n\t\t| |  _| '__/ _ \\/ _ \\/ _` | | | |\n\t\t| |_| | | |  __/  __/ (_| | |_| |\n\t\t \\____|_|  \\___|\\___|\\__,_|\\__, |\n\t\t						   |___/\n";
			break;
		case 4 :
			strplayer2 = "\t\t _   _                                       _      \n\t\t| | | | ___  __ _  ___ _ __ ___   ___  _ __ (_) ___ \n\t\t| |_| |/ _ \\/ _` |/ _ \\ '_ ` _ \\ / _ \\| '_ \\| |/ __|\n\t\t|  _  |  __/ (_| |  __/ | | | | | (_) | | | | | (__ \n\t\t|_| |_|\\___|\\__, |\\___|_| |_| |_|\\___/|_| |_|_|\\___|\n\t\t			|___/\n";
			break;
		case 5 :
			strplayer2 = "\t\t __  __ _              _ \n\t\t|  \\/  (_)_  _____  __| |\n\t\t| |\\/| | \\ \\/ / _ \\/ _` |\n\t\t| |  | | |>  <  __/ (_| |\n\t\t|_|  |_|_/_/\\_\\___|\\__,_|\n";
			break;
		default:
			strplayer2 = "Oups";
			break;
  	}
  	
  	printf("%s\t__      ____  \n\t\\ \\    / / /  \n\t \\ \\  / / /__ \n\t  \\ \\/ / / __|\n\t   \\  / /\\__ \\\n\t    \\/_/ |___/\n%s\n", strplayer1, strplayer2);

  	printf("Voici l'état initial : \n\n");
	GR12_fancy_show_map(map);
}

void GR12_end_game(Color winner){
	switch(winner){
		case PLAYER_1 : printf("Le joueur 1 a gagné !\n\n"); break;
		case PLAYER_2 : printf("Le joueur 2 a gagné !\n\n"); break;
		case EMPTY : printf("Égalité stricte ! Aucun gagnant !\n\n"); break;
		default : printf("[\033[31mERROR\033[0m] Quelque chose d'inattendu s'est produit T-T\n\n"); break;
	}
}