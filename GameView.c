// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Places.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
static void readPlay(GameView currentView);
static void draculaPlays(char *play, GameView currentView);
static void hunterPlays(char *play, GameView currentView);

static void score(GameView currentView);
// static void health(GameView currentView);

static int rounds;
static int hospital;
static int matured;
static int draculaHealth;

#define STRING_OF_ROUND 40
#define STRING_OF_MOVE 8
#define PERIOD_OF_VAMPIRE_SPAWN 13

struct gameView {
    Round roundNum;
    PlayerID player;
    int currLocation[NUM_PLAYERS];
    int score; 
    int health[NUM_PLAYERS];  
    char *pastPlays;
    int pastPos[NUM_PLAYERS][TRAIL_SIZE];
};


// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->pastPlays = pastPlays;
    gameView->roundNum = (sizeof(pastPlays)/(STRING_OF_ROUND*sizeof(char)));
    rounds = gameView->roundNum;
    gameView->player = (rounds % 5);

    int counter;
    for(counter = 0; counter < 4; counter++) {
        gameView->health[counter] = GAME_START_HUNTER_LIFE_POINTS;
    }

    readPlay(gameView);

    score(gameView);

    return gameView;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->roundNum;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->player;
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->health[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->currLocation[player];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
  int i;
  for(i=0; i<TRAIL_SIZE; i++){
    trail[i] = currentView->pastPos[player][i];
  }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// static function that reads the pastplay string
static void readPlay(GameView currentView) {
    
    draculaHealth = GAME_START_BLOOD_POINTS;
    hospital = 0;
    matured = 0;
    char play[STRING_OF_MOVE] = {0};
    int counter, string;

    while(currentView->pastPlays[counter] != 0) {
        for(string = 0; string < STRING_OF_MOVE; counter++, string++) {
            play[string] = currentView->pastPlays[counter];
        }
        if(play[0] == 'D') {
            draculaPlays(play, currentView);
        } else {
            hunterPlays(play, currentView);
        }
    }
}

// static function to read the dracula's move
static void draculaPlays(char *play, GameView currentView) {

    char abbrev[3] = {0};
    abbrev[0] = play[1];
    abbrev[1] = play[2];
    abbrev[2] = 0;

    if(play[6] == 'V') {
        matured++;
    }

    currentView->currLocation[PLAYER_DRACULA] = abbrevToID(abbrev);

    if(idToType(currentView->currLocation[PLAYER_DRACULA]) == SEA) {
        draculaHealth = draculaHealth - LIFE_LOSS_SEA;
    } else if(currentView->currLocation[PLAYER_DRACULA] == CASTLE_DRACULA ||
            (abbrev[0] == 'T' && abbrev[1] == 'P')) {
        draculaHealth = draculaHealth + LIFE_GAIN_CASTLE_DRACULA;
    }
}

static void hunterPlays(char *play, GameView currentView) {

    char abbrev[3] = {0};
    abbrev[0] = play[1];
    abbrev[1] = play[2];
    abbrev[2] = 0;

    int player;

    if(play[0] == 'G') {
        player = PLAYER_LORD_GODALMING;
    } else if(play[0] == 'S') {
        player = PLAYER_DR_SEWARD;
    } else if(play[0] == 'H') {
        player = PLAYER_VAN_HELSING;
    } else if(play[0] == 'M') {
        player = PLAYER_MINA_HARKER;
    }

}

/*
// static function to calculate health of all players
static void health(GameView currentView) {
    
    hospital = 0;
    int string, counter = 0;
    char readPlay[STRING_OF_MOVE] = {0};
    char lastPlace[]

    while(pastPlays[counter] != NULL) {
        for(string = 0; string < STRING_OF_MOVE; counter++, string++) {
            readPlay[counter] = pastPlays[counter];
        }

        if(readPlay[])
    }
}
*/

// static function to calculate current score of the game
static void score(GameView currentView) {
    // initialzing total score to 366
    int totalScore = GAME_START_SCORE;

    // decreasing score by hospital visits
    totalScore = totalScore - (SCORE_LOSS_HUNTER_HOSPITAL * hospital);

    // decreasing score by vampires matured
    totalScore = totalScore - (SCORE_LOSS_VAMPIRE_MATURES * matured);

    // decrease score by the rounds played
    totalScore = totalScore - rounds;
    
    currentView->score = totalScore;
}