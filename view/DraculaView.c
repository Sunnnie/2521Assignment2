////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
// add your own #includes here

#define EACH_TURN 
#define EACH_ROUND  

#define NUM_LOCATIONS 100
// TODO: ADD YOUR OWN STRUCTS HERE

struct draculaView {
	// TODO: ADD FIELDS HERE
	GameView gv; 
	PlaceId *trail; 
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	DraculaView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}


	new->gv = GvNew(pastPlays, message); 

	return new;
}

void DvFree(DraculaView dv)
{

	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GvFree(dv->gv);

	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION

	*numTraps = trapLocationSize(dv->gv);
	
	return GvGetTrapLocation(dv->gv);
}

////////////////////////////////////////////////////////////////////////
// Making a Move
//cases: 
/*
Round round(gameView gv) 
{
	return (gv->round); 
}

PlaceId trail_location(gamwView gv) 
{
	return gv->trail; 
}
*/ 
PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId *moves = malloc(NUM_LOCATIONS*sizeof(PlaceId)); 

	if (round(dv->gv) == 0) {
		for (int a = 0; a < MAX_REAL_PLACE; a++) {
			if (a != ST_JOSEPH_AND_ST_MARY) (
				*numReturnedMoves++; 
			)
		}
	} else {
		//get all connection && one more case here need to be added: where no place to go 
		PlaceId *reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, round(dv->gv), GvGetPlayerLocation(dv->gv, PLAYER_DRACULA), numReturnedLocs);
		PlaceId *vaildMoves = malloc(NUM_LOCATIONS*sizeof(PlaceId)); 


		//check whether double_back and hidden happen in the last 5(set 6 here toncopy the whole trails array) round
		PlaceId *trails = malloc(6*sizeof(PlaceId)); 
		trails = trail_location(dv->gv); 

		int hidden = 0;
		int double_back = 0; 

		//can hide at sea 
		if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == SEA) {
			hidden = 1; 
		}

		//only consider the last 5 moves here
		for (int i = 5; i > 0; i--) {
			if (trails[i] >= DOUBLE_BACK_1 && trails[i] <= DOUBLE_BACK_5) {
				double_back = 1; 
			}
			if(trails[i] == HIDE) {
				hidden = 1; 
			}
		}

		int num = 0; 
		if (hidden == 0) {
			vaildMoves[num] = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA); 
			*numReturnedMoves++;
			num++;
		} 
		
		if (double_back == 0) {
			for (int b = 5; b > 0; b--) {
				vaildMoves[num] = trails[b]; 
				*numReturnedMoves++; 
				num++;
			}
		}
	 

		//consider all possible moves here
		int j = 0; 
		while(reachable[j] != NULL) {
			//check whether is visited already 
			int visited = 0;
			
			for (int a = 5; a > 0; a--) {
				if (reachable[j] == trails[a]) {
					visited = 1; 
				}
			}

			if (visited == 0) {
				vaildMoves[num] = reachable[j]; 
				*numReturnedMoves++;
				num++;
			}

		}

		return vaildMoves; 
	}




	

	*numReturnedMoves = 0; //not sure on this one 
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv);
	PlaceId curr = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	PlaceId *reachable = malloc(MAX_REAL_PLACE*sizeof(PlaceId));
	reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, round_num, curr, numReturnedLocs);
		
	return reachable;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv); 
	PlaceId curr = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	PlaceId *reachable_type = malloc(MAX_REAL_PLACE*sizeof(PlaceId));
	reachable_type = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round_num, curr, true, false, true, numReturnedLocs);
	
	return reachable_type;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv);
	PlaceId curr = GvGetPlayerLocation(dv->gv, player);
	PlaceId *reachable = malloc(MAX_REAL_PLACE*sizeof(PlaceId));
	reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, round_num, curr, numReturnedLocs);
		
	return reachable;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv); 
	PlaceId curr = GvGetPlayerLocation(dv->gv, player);
    PlaceId *reachable_type = malloc(MAX_REAL_PLACE*sizeof(PlaceId));
	reachable_type = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round_num, curr, true, true, true, numReturnedLocs);
	
	return reachable_type;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
