CC = gcc
OBJS = chessSystem.o game.o gameId.o internalFunctions.o intKey.o player.o playerInTour.o tournament.o map.o main.o
EXEC = chess
DEBUG_FLAG = # assign -g for debug
COMP_FLAG = -std=c99 -Wall -Werror -pedantic-errors # need to add -DNDEBUG


$ (EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ *.c .tests/chessSystemTestsExample.c -L -lmap



chessSystem.o: chessSystem.c mtm_map/map.h chessSystem.h tournament.h playerInTour.h intKey.h internalFunctions.h player.h game.h gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
main.o: tests/chessSystemTestsExample.c chessSystem.h game.h gameId.h internalFunctions.h intKey.h player.h playerInTour.h tournament.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/chessSystemTestsExample.c -o main.o 
map.o: mtm_map/map.c mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) ./mtm_map/map.c
game.o: game.c game.h chessSystem.h gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
gameId.o: gameId.c gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
internalFunctions.o: internalFunctions.c internalFunctions.h map.h chessSystem.h tournament.h playerInTour.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
intKey.o: intKey.c intKey.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
player.o: player.c player.h map.h chessSystem.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
playerInTour.o: playerInTour.c playerInTour.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
tournament.o: tournament.c game.h chessSystem.h gameId.h intKey.h map.h internalFunctions.h tournament.h playerInTour.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS) $(EXEC)
