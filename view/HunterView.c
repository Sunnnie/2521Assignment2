////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct hunterView {
	GameView view;
	int encounter_Dracula;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}
    GameView view = malloc(sizeof(struct gameView));
    new->view = GvNew(pastPlays, messages[]);
	new -> encounter_Dracula = 0;
	for(int i = 0; pastPlays[i] != '\0'; i++){
		if(pastPlays[i] == V) new -> encounter_Dracula = 1;
		break;
	}
	return new;
}

void HvFree(HunterView hv)
{
	free(hv-> view);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	return hv -> view -> round;
}

Player HvGetPlayer(HunterView hv)
{
	return hv -> view ->player;
}

int HvGetScore(HunterView hv)
{
	return hv -> view ->gamescore;
}

int HvGetHealth(HunterView hv, Player player)
{
	return hv -> view -> playerStats[player] ->health;
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	int last = hv -> view -> playerStats[player] -> moveHistSize -1;
	if (last == -1) return NOWHERE;
	
	if(hv ->view ->playStats[player] -> health == 0)
	return HOSPITAL_PLACE;
	
	return hv->view->playStats[player] -> moveHist[last];
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return hv -> view -> vampLocationE;
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	// if the Dracula's location is known by hunter

	for(int i = round; round > 0; i--){
		if(hv -> encounter_Dracula == 1){
			PlaceId id = GvGetPlayerLocation(hv -> view, hv -> view -> player);
			return id;
			break;
		}
	}
	// if the Dracula's location is unknown

	return NOWHERE;
}

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*pathLength = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	if(hv -> view -> round = 0){
		*numReturnedLocs = 0;
		return NULL;
	}
	
	for(int i = 0; i < NUM_PLAYERS; i++){
		if(i == player) 
			PlaceId id = hv -> view -> palyStats[i] -> moveHist[hv->view->round];
		i++;
		break;
	}

	return GvGetReachable(hv->view, hv->view->i, hv->view->round, hv->view->id, *numReturnedLocs);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	if(hv -> view -> round = 0){
		*numReturnedLocs = 0;
		return NULL;
	}
	
	for(int i = 0; i < NUM_PLAYERS; i++){
		if(i == player) 
			PlaceId id = hv -> view -> palyStats[i] -> moveHist[hv->view->round];
		i++;
		break;
	}

	return GvGetReachableByType(GameView hv->view, hv->view->i, hv->view->round, 
								hv->view->id, road, rail, boat, *numReturnedLocs);
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
