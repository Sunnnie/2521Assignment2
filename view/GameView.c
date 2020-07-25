////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
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
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"

#define MAX_TRAPS 3 
#define MOVES_SIZE 7
#define ROUND_SIZE (MOVES_SIZE * NUM_PLAYERS + NUM_PLAYERS) // size of each round
#define LAST_ROUND_SIZE (ROUND_SIZE-1) // size of last round
#define MOVE_DELIM " "
#define PLACE_ABBREV_SIZE 2
#define START_PLAY_ACTION 3

// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct playerStats{
	int health;
	PlaceId *moveHist; //chronological order
	int moveHistSize;//stores size of array

};
typedef struct playerStats * PlayerStats;

struct gameView {
	int gamescore;
	Round round;
	PlaceId *trapLocations; // array of trap locations
	int trapLocationSize; 
	PlaceId vampLocation;
	PlayerStats *playerStats; //array of playerstats
	Player player; // current player
	PlaceId *trail; // queue array of trail locations
	int trailSize;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

// Gets place ID from move
PlaceId getPlaceFromMove(char *move)
{
	char abbrev[PLACE_ABBREV_SIZE+1];//accounts for null string operator
	strncpy(abbrev, move+1, PLACE_ABBREV_SIZE);//skips first element and copies location into abbrev
	return placeAbbrevToId(abbrev); // see use of Function in Places.h
}
//Returns player name from the first element of move string 
Player playerFromChar(char c)
{
	if (c == 'G')
		return PLAYER_LORD_GODALMING;
	if (c == 'S')
		return PLAYER_DR_SEWARD;
	if (c == 'H')
		return PLAYER_VAN_HELSING;
	if (c == 'M')
		return PLAYER_MINA_HARKER;
	if (c == 'D')
		return PLAYER_DRACULA;
	return -1;
}
// TO DO updates health of Dracula and Hunters
void updateHealth(GameView gv, int healthChange, Player player)
{
	gv->playerStats[player]->health += healthChange; // update player health
	if (player != PLAYER_DRACULA && gv->playerStats[player]->health > GAME_START_HUNTER_LIFE_POINTS)
		gv->playerStats[player]->health = GAME_START_HUNTER_LIFE_POINTS; // set hunter health to max 
	if (gv->playerStats[player]->health <= 0)
	{
		gv->playerStats[player]->health = 0;//if hunter health is negative or 0 set to 0
		if (player != PLAYER_DRACULA)
			gv->gamescore -= SCORE_LOSS_HUNTER_HOSPITAL; // update game score -4
	}	
} 
// removes first place in an array that matches parameter "place" and shifts down to fill empty spots
void removePlace(PlaceId *places, int *size, PlaceId place)
{
	if (*size == 0)
		return;

	int pos = -1;
	for (int i = 0; i < *size; i++)
	{
		if (places[i] == place)
		{
			pos = i;
			break;
		}
	}

	for (int i = pos; i < *size -1; i++)
	{
		places[i] = places[i+1];
	}
	places[*size-1] = NOWHERE;
	*size -= 1;
}
// assuming the array is big enough to fit what we are inserting
void addPlace(PlaceId *places, int *size, PlaceId place)
{
	places[*size] = place;
	*size += 1;
}

//handles the encounters for the hunters
void handleHunter(GameView gv, char *move, Player player)
{
	PlaceId place = getPlaceFromMove(move); //gets placeId for the player
	int lastPos = gv->playerStats[player]->moveHistSize-1;

	if (lastPos >= 0 && gv->playerStats[player]->moveHist[lastPos] == place)
	{
		updateHealth(gv, LIFE_GAIN_REST, player);
	}

	addPlace(gv->playerStats[player]->moveHist, &(gv->playerStats[player]->moveHistSize), place);


	for (int i = START_PLAY_ACTION; i < MOVES_SIZE; i++) // starts loop from action move
	{
		char encounter = move[i];
		if (encounter == 'T')
		{
			updateHealth(gv, -LIFE_LOSS_TRAP_ENCOUNTER, player);
			removePlace(gv->trapLocations, &(gv->trapLocationSize), place);
			//To do: confirm if hunter dies from trap vampire and dracula is not seen from pastPlays.
		}
		else if (encounter == 'V')
		{
			gv->vampLocation = NOWHERE;
		}
		else if (encounter == 'D')
		{
			updateHealth(gv, -LIFE_LOSS_DRACULA_ENCOUNTER, player);
			updateHealth(gv, -LIFE_LOSS_HUNTER_ENCOUNTER, PLAYER_DRACULA);
		}

	}
}

void handleDracula(GameView gv, char *move)
{
	PlaceId place = getPlaceFromMove(move);
	Player player = PLAYER_DRACULA;

	if (place == TELEPORT)
	{
		place = CASTLE_DRACULA;
	}
	else if (place == HIDE)
	{
		place = gv->trail[gv->trailSize-1];
	}
	else if (place >= DOUBLE_BACK_1 && place <= DOUBLE_BACK_5)
	{
		int back = place - DOUBLE_BACK_1 + 1;
		place = gv->trail[gv->trailSize-back];
	}

	addPlace(gv->playerStats[player]->moveHist, &(gv->playerStats[player]->moveHistSize), place);

	if (gv->trailSize == TRAIL_SIZE)
	{
		removePlace(gv->trail, &(gv->trailSize), place);
	}
	addPlace(gv->trail, &(gv->trailSize), place);

	if (placeIsSea(place))
	{
		updateHealth(gv, -LIFE_LOSS_SEA, player);
	}

	if (place == CASTLE_DRACULA)
	{
		updateHealth(gv, LIFE_GAIN_CASTLE_DRACULA, player);
	}

	for (int i = START_PLAY_ACTION; i < MOVES_SIZE; i++)
	{
		char encounter = move[i];
		if (encounter == 'T')
		{
			if (gv->trapLocationSize == MAX_TRAPS)
				removePlace(gv->trapLocations, &(gv->trapLocationSize), place);
			addPlace(gv->trapLocations, &(gv->trapLocationSize), place);
		}
		// TO DO replace 4 with # define
		else if (encounter == 'V' && i == 4)
		{
			gv->vampLocation = place;
		}
		else if (encounter == 'M')
		{
			removePlace(gv->trapLocations, &(gv->trapLocationSize), place);
		}
		else if (encounter == 'V')
		{
			gv->vampLocation = NOWHERE;
			gv->gamescore -= SCORE_LOSS_VAMPIRE_MATURES;
		}
	}
	gv->gamescore -= SCORE_LOSS_DRACULA_TURN;
}

// handles all plays 
void handlePastPlays(char *pastPlays, GameView gv)
{
	char *move = strtok(strdup(pastPlays), MOVE_DELIM); // splits pastplays into player moves
	while (move != NULL)
	{
		//process move
		Player player = playerFromChar(move[0]); // keeps track of player 
		if(player != PLAYER_DRACULA)
		{
			handleHunter(gv, move, player);
		}else 
		{
			handleDracula(gv, move);
		}
		move = strtok(NULL, MOVE_DELIM); 
	}
}

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(*new));
	assert(new != NULL);
	new->gamescore = GAME_START_SCORE; // set gamescore 
	new->vampLocation = NOWHERE; // set intial vampire location
	new->trapLocations = malloc(MAX_TRAPS*sizeof(PlaceId));
	assert(new->trapLocations != NULL);
	new->trapLocationSize = 0;

	for (int i = 0; i < MAX_TRAPS; i++)
	{
		new->trapLocations[i] = NOWHERE; //initialise trap locations
	}
	
	new->trail = malloc(TRAIL_SIZE *sizeof(PlaceId));
	new->trailSize = 0;

	for (int i = 0; i < TRAIL_SIZE; i++)
	{
		new->trail[i] = NOWHERE; //initialise trail
	}

	int pastPlaysSize = strlen(pastPlays); //size of pastplays
	new->round = (pastPlaysSize + 1)/ROUND_SIZE; //computes current round
	int sizeofLastRound = pastPlaysSize % ROUND_SIZE; //size of last round 
	
	if (sizeofLastRound == 0 || sizeofLastRound == LAST_ROUND_SIZE) //if first round or start of last round set to first player
	{
		new->player = PLAYER_LORD_GODALMING;
	}else
	{
		new->player = sizeofLastRound/MOVES_SIZE; 
	}
	new->playerStats = malloc(NUM_PLAYERS*sizeof(PlayerStats)); // dinamically allocate memory for playerstats array
	assert(new->playerStats != NULL);
	// loop initialises all player stats
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		new->playerStats[i] = malloc(sizeof(struct playerStats));//allocates memory for playerstats structs
		assert(new->playerStats[i] != NULL);
		if (i == PLAYER_DRACULA)
		{
			new->playerStats[i]->health = GAME_START_BLOOD_POINTS;
		}else 
		{
			new->playerStats[i]->health = GAME_START_HUNTER_LIFE_POINTS;
		}
		new->playerStats[i]->moveHist = malloc(new->round * sizeof(PlaceId));//dynamically allocates memory to round move history
		assert(new->playerStats[i]->moveHist != NULL);
		for (int j =0 ; j < new->round; j++)
		{
			new->playerStats[i]->moveHist[j] = NOWHERE;
		}
		new->playerStats[i]->moveHistSize = 0;
	}
	handlePastPlays(pastPlays, new);

	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv->trapLocations);
	free(gv->trail);
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		free(gv->playerStats[i]->moveHist);
		free(gv->playerStats[i]);
	}
	free(gv->playerStats);
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
	return gv->player;
}

int GvGetScore(GameView gv)
{
	return gv->gamescore;
}

int GvGetHealth(GameView gv, Player player)
{
	return gv->playerStats[player]->health;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	int last = gv->playerStats[player]->moveHistSize -1;
	if (last == -1)
		return NOWHERE;

	if (gv->playerStats[player]->health == 0)
		return HOSPITAL_PLACE;
	//TO DO check header file and add edge cases/ read descriptions
	
	return gv->playerStats[player]->moveHist[last];
}

PlaceId GvGetVampireLocation(GameView gv)
{	
	return gv->vampLocation;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: confirm new array required 
	*numTraps = gv->trapLocationSize;
	return gv->trapLocations;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
