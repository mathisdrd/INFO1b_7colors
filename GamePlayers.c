#include "../head/GamePlayers.h"

extern const Color GR12_colors[7];
int main_avancement;

// On définit les différents joueurs disponibles
Color (*GR12_players[6])(GameState*, Color) = {
    GR12_player_human, 
    GR12_player_AI_random,
    GR12_player_AI_random_plus,
    GR12_player_AI_greedy,
    GR12_player_AI_hegemonic,
    GR12_player_AI_mixed,
};

Color GR12_player_human(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée
    int move; // Indice associé à la couleur jouée
    char strmove; // Carctère associé à la couleur jouée
    int verif; // Pour vérifier que le scanf réussis
    
    /* VERSION AFFICHE CLASSIQUE
    printf("Veuillez choisir une couleur à jouer : \033[30m(Lettre entre A et G)\033[0m\n");
    printf("[A]\t[B]\t[C]\t[D]\t[E]\t[F]\t[G]\n\n-> ");

    do{ // Script de vérification du scanf
        verif = scanf(" %c", &strmove);
        if (0 == verif || strmove < 65 || strmove > 71){
            printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valable compris entre A et G.\n        Veuillez réessayer :\n\n-> ");
        }
    } while (0 == verif || strmove < 65 || strmove > 71);
    printf("\n");

    move  = strmove - 65; // Il y a PLAYER_1 et PLAYER_2 et l'indicage commence à 0.
    //*/// FIN VERSION AFFICHAGE CLASSIQUE

    //* VERSION AFFICHAGE FANCY
    printf("Veuillez choisir une couleur à jouer : \033[30m(Lettre correspondante)\033[0m\n");
    printf("[R] Rouge | [V] Vert | [B] Bleu | [J] Jaune | [M] Magenta | [C] Cyan | [O] Orange\n\n-> ");

    do{ // Script de vérification du scanf
        verif = scanf(" %c", &strmove);
        if (0 == verif || !(strmove == 'R' || strmove == 'V' || strmove == 'B' || strmove == 'J' || strmove == 'M' || strmove == 'C' || strmove == 'O')){
            printf("\n[\033[31mERROR\033[0m] La valeur entrée n'est pas un caractère valide (R, V, B, J, M, C, O).\n        Veuillez réessayer :\n\n-> ");
            // On vide le buffer d'entrée
            while (getchar() != '\n');
        }
    } while (0 == verif || !(strmove == 'R' || strmove == 'V' || strmove == 'B' || strmove == 'J' || strmove == 'M' || strmove == 'C' || strmove == 'O'));
    printf("\n");

    // On associe la valeur entière en fonction de la couleur
    switch(strmove){
        case 'R': move = 0; break;
        case 'V': move = 1; break;
        case 'B': move = 2; break;
        case 'J': move = 3; break;
        case 'M': move = 4; break;
        case 'C': move = 5; break;
        case 'O': move = 6; break;
    }
    //*/// FIN VERSION AFFICHAGE FANCY

    // On récupère la couleur associée au choix de l'utilisateur
    color = GR12_colors[move];
    return color;
}

Color GR12_player_AI_random(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée
    color = GR12_colors[rand() %7];
    return color;
}

Color GR12_player_AI_random_plus(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée

    // On créé la liste pour l'appel de la fonction GR12_playable_colors et on récupère les couleurs jouables
    int playable[7]; GR12_playable_colors(map, player, playable);
    
    int count = 0; // On compte le nombre de coups jouables
    for (int i=0; i<7; i++){count += playable[i];}

    int playable_only[count]; // on créé une liste avec les couleurs jouables
    count = 0; // On met le compteur à 0 
    for (int i=0; i<7; i++){
        if (playable[i]){
            playable_only[count] = GR12_colors[i];
            count++; // Pour passer à la prochaine couleur
        }
    }

    // On récupère une valeur aléatoire dans les coups jouables pour ne pas avoir à faire un while.
    color = playable_only[rand() %count];

    return color;
}

Color GR12_player_AI_greedy(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée
    int move = 0; // Indice correspondant

    int actual_count = GR12_player_area(map, player); // Nombre de cases du terrain occupées actuellement par player
    int expansion = 0; // Avancement du terrain à l'état i
    int max = -1; // Maximum d'avancement de terrain

    GameState copy = {.map = NULL, .size = 0}; // On déclare un GameState pour copier la map actuelle
    
    // On créé la liste pour l'appel de la fonction GR12_playable_colors et on récupère les couleurs jouables
    int playable[7]; GR12_playable_colors(map, player, playable);

    for (int i=0; i<7; i++){ // On parcourt les 7 couleurs
        if (playable[i] == 0){continue;} // Si la couleur n'est pas jouable on passe
        
        // On (ré)initialise la copie de map
        GR12_copy_game_state(map, &copy);

        // On update la map avec la couleur colors[i]
        GR12_update_map(&copy, player, GR12_colors[i]);

        // On regarde le nombres de cases gagnées avec la couleur colors[i]
        expansion = GR12_player_area(&copy, player) - actual_count;

        if (expansion>max){ // On regarde si l'avancement de terrain avec colors[i] est plus grand que le maximum trouvé jusque là
            max = expansion; // Si l'avancement est plus grand avec colors[i], on met à jour le nouveau maximum d'avancement...
            move = i; // et le coup correspondant
        }
    }
    color = GR12_colors[move]; // La couleur avec le plus grand avancement est donc la couleur colors[move]

    // On free la copie
    free(copy.map);
	copy.map = NULL;

    return color;
}

Color GR12_player_AI_hegemonic(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée
    int move = -1; // Indice correspondant

    int actual_count = GR12_reachable_squares(map, player); // Nombre de cases du terrain occupées actuellement par player
    int expansion = 0; // Augmentation du nombre de cases atteignables par player à l'état i.
    int max = -map->size*map->size; // Maximum d'augmentation

    GameState copy = {.map = NULL, .size = 0}; // On déclare un GameState pour copier la map actuelle

    int playable[7]; GR12_playable_colors(map, player, playable);

    for (int i=0; i<7; i++){ // On parcourt les 7 couleurs
        if (playable[i] == 0){continue;} // Si la couleur n'est pas jouable on passe
        
        // On (ré)initialise la copie de map
        GR12_copy_game_state(map, &copy);

        // On update la map avec la couleur colors[i]
        GR12_update_map(&copy, player, GR12_colors[i]);

        // On regarde le nombres de cases atteignables  avec la couleur colors[i]
        expansion = GR12_reachable_squares(&copy, player) - actual_count;
        if (expansion>max){ // On regarde si l'avancement de terrain avec colors[i] est plus grand que le maximum trouvé jusque là
            max = expansion; // Si l'avancement est plus grand avec colors[i], on met à jour le nouveau maximum d'avancement...
            move = i; // ...et le coup correspondant
        }
    }

    color = GR12_colors[move]; // La couleur avec le plus grand avancement est donc la couleur colors[move]

    // On free la copie
    free(copy.map);
	copy.map = NULL;

    return color;
}

Color GR12_player_AI_mixed(GameState* map, Color player){
    Color color = ERROR; // Couleur jouée
    int move = -1; // Indice correspondant

    int actual_count = GR12_reachable_squares(map, player); // Nombre de cases du terrain occupées actuellement par player
    int expansion = 0; // Augmentation du nombre de cases atteignables par player à l'état i.
    int max = -map->size*map->size; // Maximum d'augmentation

    GameState copy = {.map = NULL, .size = 0}; // On déclare un GameState pour copier la map actuelle

    int playable[7]; GR12_playable_colors(map, player, playable);

    for (int i=0; i<7; i++){ // On parcourt les 7 couleurs
        if (playable[i] == 0){continue;} // Si la couleur n'est pas jouable on passe
        
        // On (ré)initialise la copie de map
        GR12_copy_game_state(map, &copy);

        // On update la map avec la couleur colors[i]
        GR12_update_map(&copy, player, GR12_colors[i]);

        // On regarde le nombres de cases atteignables  avec la couleur colors[i]
        expansion = GR12_reachable_squares(&copy, player) - actual_count;

        if (expansion>max){ // On regarde si l'avancement de terrain avec colors[i] est plus grand que le maximum trouvé jusque là
            max = expansion; // Si l'avancement est plus grand avec colors[i], on met à jour le nouveau maximum d'avancement...
            move = i; // et le coup correspondant
        }
    }

    if (0 >= max){ // Si l'avancement des frontières est nul ou négatif, on joue de manière gloutonne
        color = GR12_player_AI_greedy(map, player);
    } else { // Sinon on joue hégémonique normalement
        color = GR12_colors[move]; // La couleur avec le plus grand avancement est donc la couleur colors[move]
    }

    // On free la copie
    free(copy.map);
	copy.map = NULL;

    return color;    
}