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
*/ 
PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION




	

	*numReturnedMoves = 0; //not sure on this one 
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv);
	PlaceId curr = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	PlaceId *reachable = malloc(NUM_LOCATIONS*sizeof(PlaceId));
	reachable = GvGetReachable(dv->gv, PLAYER_DRACULA, round_num, curr, numReturnedLocs);
		
	return reachable;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv); 
	PlaceId curr = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	PlaceId *reachable_type = malloc(NUM_LOCATIONS*sizeof(PlaceId));
	reachable_type = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round_num, curr, true, false, true, numReturnedLocs);
	
	return reachable_type;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	Round round_num = round(dv->gv);
	PlaceId curr = GvGetPlayerLocation(dv->gv, player);
	PlaceId *reachable = malloc(NUM_LOCATIONS*sizeof(PlaceId));
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
	PlaceId *reachable_type = malloc(NUM_LOCATIONS*sizeof(PlaceId));
	reachable_type = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round_num, curr, true, true, true, numReturnedLocs);
	
	return reachable_type;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
