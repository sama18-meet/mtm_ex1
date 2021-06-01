CC = gcc
OBJS = chessSystem.o games.o gameId.o player.o playerInTour.o tournament.o intKey.o internalFunctions.o chessSystemTestsExample.o
EXEC = chess
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror

$(EXEC) : $(OBJS)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(OBJS) -o $@  -L. -lmap

chessSystem.o: chessSystem.c mtm_map/map.h chessSystem.h tournament.h intKey.h internalFunctions.h player.h game.h gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) chessSystem.c
game.o: game.c game.h chessSystem.h gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG)  game.c
gameId.o: gameId.c gameId.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) gameId.c 
internalFunctions.o: internalFunctions.c internalFunctions.h mtm_map/map.h chessSystem.h tournament.h playerInTour.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) internalFunctions.c
intKey.o: intKey.c intKey.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) intKey.c
player.o: player.c player.h mtm_map/map.h chessSystem.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) player.c
playerInTour.o: playerInTour.c playerInTour.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) playerInTour.c
tournament.o: tournament.c game.h chessSystem.h gameId.h intKey.h mtm_map/map.h internalFunctions.h tournament.h playerInTour.h player.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tournament.c
chessSystemTestsExample.o: tests/chessSystemTestsExample.c mtm_map/map.h tests/../test_utilities.h tests/../chessSystem.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/chessSystemTestsExample.c


clean:
	rm -f $(OBJS) $(EXEC)