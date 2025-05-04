#FLAGS=-Wall -Werror -lm -g
FLAGS=-Wall -Werror -g

7colors : o/GameState.o o/GameShow.o o/GameCore.o o/GamePlayers.o
	gcc $(FLAGS) o/GameState.o o/GameShow.o o/GameCore.o o/GamePlayers.o -o 7colors

o/GameState.o: head/GameState.h src/GameState.c
	gcc $(FLAGS) -c src/GameState.c -o o/GameState.o

# Les fonctions du groupe :

o/GamePlayers.o: head/GamePlayers.h src/GamePlayers.c
	gcc $(FLAGS) -c src/GamePlayers.c -o o/GamePlayers.o

o/GameShow.o: head/GameShow.h src/GameShow.c
	gcc $(FLAGS) -c src/GameShow.c -o o/GameShow.o

o/GameCore.o: head/GameCore.h src/GameCore.c
	gcc $(FLAGS) -c src/GameCore.c -o o/GameCore.o