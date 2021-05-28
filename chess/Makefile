CC=gcc
OBJS = election.o election_votes_list.o chessSystemTestsExample.o
EXEC = chess
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ -L. -lmap
election.o : election.c election.h mtm_map/map.h election_votes_list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) election.c
election_votes_list.o : election_votes_list.c mtm_map/map.h election_votes_list.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) election_votes_list.c 
electionTestsExample.o : tests/electionTestsExample.c  mtm_map/map.h tests/../test_utilities.h tests/../election.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG)  tests/electionTestsExample.c
clean:	
	  rm -f $(OBJS) $(EXEC)
