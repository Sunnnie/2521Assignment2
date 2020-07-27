////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.h: the Game View ADT
//
// You may augment this with as many of your own functions as you like,
// but do NOT remove or modify any of the existing function signatures,
// otherwise, your code will not be able to compile with our tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-11-30	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#ifndef FOD__GAME_VIEW_H_
#define FOD__GAME_VIEW_H_

#include <stdbool.h>

#include "Game.h"
#include "Places.h"
// add your own #includes here

typedef struct gameView *GameView;

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

/**
 * Creates a new view to summarise the current state of the game.
 *
 * @param pastPlays - A string of all the plays made in the game so far.
 *                    See the spec for details of the format. The string
 *                    can be assumed to be valid.
 *
 * @param messages - An  array containing a message for each play in the
 *                   game so far. It will have exactly the  same  number
 *                   of  elements as there are plays in `pastPlays`. The
 *                   message from the first play will be at index 0, and
 *                   so on. The contents of each message will be exactly
 *                   as provided by the player.
 *
 * The "Message" type is defined in Game.h.
 * You are free to ignore messages if you wish.
 */
GameView GvNew(char *pastPlays, Message messages[]);

/**
 * Frees all memory allocated for `gv`.
 * After this has been called, `gv` should not be accessed.
 */
void GvFree(GameView gv);

////////////////////////////////////////////////////////////////////////
// Game State Information

/**
 * Gets the current round number
 */
Round GvGetRound(GameView gv);

/**
 * Gets the current player; i.e., whose turn is it?
 */
Player GvGetPlayer(GameView gv);

/**
 * Gets the current game score; a positive integer between 0 and 366.
 */
int GvGetScore(GameView gv);

/**
 * Gets the current health points for the given player; an value between
 * 0 and 9 for a hunter, or >= 0 for Dracula.
 */
int GvGetHealth(GameView gv, Player player);

/**
 * Gets the current location of a given player.
 *
 * This function should return:
 * - NOWHERE if the given player has not had a turn yet.
 * - Otherwise, a PlaceId corresponding to a real location, if the given
 *   player  is  a hunter. Note that if the hunter was teleported to the
 *   Hospital of St Joseph and St Mary, ST_JOSEPH_AND_ST_MARY should  be
 *   returned.
 * - Otherwise, a PlaceId corresponding to a real location, if the given
 *   player is Dracula and his current location has been revealed in the
 *   play string. Note that Dracula's location could be revealed even if
 *   his  latest move was a HIDE or DOUBLE_BACK, if these moves refer to
 *   a previous LOCATION move (see the spec for details). Also note that
 *   if Dracula's latest move was TELEPORT, this means he is  in  Castle
 *   Dracula.
 * - Otherwise, CITY_UNKNOWN if Dracula is in a city, and SEA_UNKNOWN if
 *   Dracula is at sea.
 * 
 * This  function should never return HIDE or DOUBLE_BACK - if Dracula's
 * latest move was a HIDE or DOUBLE_BACK, it should still be able to  be
 * resolved to a city or sea.
 */
PlaceId GvGetPlayerLocation(GameView gv, Player player);

/**
 * Gets the location of the sleeping immature vampire.
 *
 * This function should return:
 * - NOWHERE  if the immature vampire does not exist (i.e., if it hasn't
 *   been spawned, or if it has already matured or been vanquished).
 * - A  PlaceId corresponding to a real location, if the location of the
 *   immature vampire has been revealed in the play string.  This  means
 *   Dracula's  location during the round in which he placed an immature
 *   vampire was revealed.
 * - Otherwise,  CITY_UNKNOWN.  Note that this function can never return
 *   SEA_UNKNOWN, since Dracula never places encounters at sea.
 */
PlaceId GvGetVampireLocation(GameView gv);

/**
 * Gets the locations of all active traps.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numTraps to the number of  traps.  The  array  should
 * contain  multiple  copies  of the same location if there are multiple
 * traps in that location. The locations can be in any order.
 *
 * NOTE: This  function  may  do  whatever  you  feel is suitable if the
 *       current player is a hunter, as it is  impossible  to  get  100%
 *       right  if  you do not know where Dracula has been. We will only
 *       be testing this function when Dracula is the current player.
 */
PlaceId *GvGetTrapLocations(GameView gv, int *numTraps);

////////////////////////////////////////////////////////////////////////
// Game History

/**
 * Gets  the complete move history, in chronological order, of the given
 * player as shown in the play string.
 *
 * This  function  should  return  the  moves in a dynamically allocated
 * array, and set *numReturnedMoves to the number of moves returned.
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 */
PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree);

/**
 * Gets  the given player's last `numMoves` moves in chronological order
 * as shown in the play string.
 *
 * This  function  should  return  the  moves in a dynamically allocated
 * array, and set *numReturnedMoves to the number of moves returned  (in
 * case  `numMoves`  is  greater than the number of moves the player has
 * actually made).
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 *
 * NOTE: This  function is very similar to GvGetMoveHistory, except that
 *       it gets only the last `numMoves` moves rather than the complete
 *       move history.
 */
PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree);

/**
 * Gets  the  complete  location history, in chronological order, of the
 * given player.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of locations returned.
 *
 * If  the given player is a hunter, this function should behave exactly
 * the same as GvGetMoveHistory. If the given  player  is  Dracula,  the
 * PlaceIds  returned  should  either  correspond  to real places, or be
 * CITY_UNKNOWN or SEA_UNKNOWN. See the comment for  GvGetPlayerLocation
 * for more details.
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 */
PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree);

/**
 * Gets  the  given   player's last `numLocs` locations in chronological
 * order.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of  locations  returned
 * (in  case  `numLocs` is greater than the number of  moves  the player
 * has actually made).
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 *
 * NOTE: This function is very similar to  GvGetLocationHistory,  except
 *       that  it gets only the last `numLocs` locations rather than the
 *       complete location history.
 */
PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree);

////////////////////////////////////////////////////////////////////////
// Making a Move

/**
 * Gets all the locations that can be reached by the given player in the
 * given round, assuming that they are at the given location.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of returned  locations.
 * The  array  can  be in any order but must contain unique entries. The
 * array should include the given location.
 *
 * If  the  given  player is a hunter, the function should use the given
 * round number for determining how far the player can travel by rail.
 *
 * If  the  given player is Dracula, the function must take into account
 * the fact that Dracula may not travel by rail or move to the  Hospital
 * of  St  Joseph  and  St  Mary. But it shouldn't take into account the
 * trail restriction.
 *
 * This  function  can  assume that the given player has already made at
 * least one move.
 */
PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs);

/**
 * Similar  to GvGetReachable, but the caller can restrict the transport
 * types to be considered. For example, if road and rail are  true,  but
 * boat is false, boat connections will be ignored.
 */
PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs);

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

#endif // !defined (FOD__GAME_VIEW_H_)






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