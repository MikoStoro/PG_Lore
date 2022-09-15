#include <iostream>
#include <string.h>
#include <math.h>
#define _CRT_NO_WARNINGS
#define CLASSNUMBER 4
using namespace std;

enum fieldStates {
        EMPTY = ' ',
        SHIPPART = '+',
        DESTROYEDPART = 'x',
        RADAR = '@',
        ENGINE = '%',
        CANNON = '!',
        ENCAN = '*',
        REEF = '#',
        UNKNOWN = '?',
        UNDEFINED = NULL
};

enum directions {
        N,
        E,
        S,
        W
};

enum turnDirections {
        F = 0,
        L = -1,
        R = 1
};

enum shipClasses {
        CAR = 0,
        BAT = 1,
        CRU = 2,
        DES = 3
};

enum shipSizes {
        CAR_SIZE = 5,
        BAT_SIZE = 4,
        CRU_SIZE = 3,
        DES_SIZE = 2
};

enum gamemodes {
        BASIC = 0,
        EXTENDED = 1
};

struct Area {
        int x1, y1, x2, y2;
};

struct Field;
struct Player;
struct Area;
struct Board;
struct Game;

struct Ship {
        int xPosition = NULL;
        int yPosition = NULL;
        int direction = NULL;

        bool set = false;
        bool radar = true;
        bool engine = true;
        bool cannon = true;
        int moved = 0;
        int shots = 0;

        int shipClass = NULL;
        int size = NULL;
        char* parts = NULL;

        Field** fields = NULL;
};

struct Player {
        char id = NULL;
        Ship* ships[CLASSNUMBER] = {
};
        int shipCount[CLASSNUMBER] = { -1, -1, -1, -1 
};
        Area* startingArea = new Area;
        int partsLeft = 0;
        bool AI = false;
};

struct Field {
        char state = EMPTY;
        char* part = NULL;
        Ship* ship = NULL;
        Player* player = NULL;
        Player* visible[2] = {NULL, NULL
};
};

struct Board {
        Field** fields = NULL;
        int height = 21;
        int width = 10;
};

struct Game {
        Player playerA, playerB;
        Player* activePlayer = &playerA;
        Board board;
        bool** renderBoard = NULL;
        int seed = NULL;
        bool ended = false;
        bool set = false;
        bool extended = false;
        bool autoShooting = false;
        char winner = NULL;
};

//allocate board and set its default state
void initBoard(Game* game);
//delete board
void freeBoard(Game* game);
//allocate ships
void setFleet(Player* player, int carriers, int battleships, int cruisers, int destroyers);
//sets advanced parts in ships (radar, engine, cannon)
void setExtendedParts(Ship* ship);
//delete ships
void clearFleet(Player* player);
//returns SHIPPART given any kind of ship part (engine, radar, etc.)
char getSimpleChar(char fieldChar);
//returns number of digits in the arhgument
int getDigits(int num);

// print board for given mode (0 - basic, 1 - extende)
void printBoard(Game* game, bool mode);
void printColumnNumbers(int widthDigits, int heightDigits, int width);
void printRowNumber(int rowNum, int heightDigits);

//prints board as seen by currently active player (also supports two modes)
void radarPrint(Game* game, bool mode);

//sets starting area for given player - y1,x1: left upper corner, y2,x2: right lower corner
void setStartingArea(Player* player, int y1, int x1, int y2, int x2);

//returns player->ships index for given class tag
int getClassIndex(char* shipClass);
//returns player address given player's id
Player* getPlayer(Game* game, char id);
//returns direction values for given direction tag 
int getDirection(char dir);
int getTurnDirection(char dir);
//compares const char* to char* (for recognising commands)
bool compare(const char command[], char input[]);
//changes currently active player
void changePlayer(Game* game);
//sets x and y modifiers for given direction
void setDirectionModifiers(int direction, int* xMod, int* yMod);
//checks if ship can be placed on given field. returns NULL if suceeds. returns error code on failure
char spaceAvailable(Game* game, int x, int y, Ship* ship, int D);
//places ship 'index' of class 'C' on field y,x, facing direction 'D'. If state is given, ship may enter board with some of its parts destroyed
char placeShip(Game* game, char P, int y, int x, char D, int index, int shipClass, char* state = NULL);
//places reef on field y,x
void reef(Game* game, int y, int x);
//currently active player shoots at field y, x. If ship is specified, that ship fires the shot.
char shoot(Game* game, int y, int x, Ship* ship = NULL);
//currently active player can fire cannon of index-th ship of class C, at field y,x. If inRange is specified, function assumes that the target is in ship's range
char extendedShoot(Game* game, int index, char* C, int y, int x, bool inRange = false);
//index-th carrier of currently active player sends a spy plane at field y,x
void spy(Game* game, int index, int y, int x);
//sets ship's moves and shots nmumber to 0
void resetShip(Player* player);
//currently active player's index-th ship of class C moves in direction D
char move(Game* game, int index, char* C, char D);
//prints ship's class tag, given its value 
void printClassTag(int C);
//prints a string of characters 0 and 1, 0 meaning given ship's destroyed part and 1 meaning a healthy part
void printShipParts(Ship ship);
//returns char tag of a direction (N, E, S, W), given its value
char getDirectionTag(int dir);
//saves state of the game
void save(Game* game);
//program starts recieving state commands
void getStateCommands(Game* game);
//program starts recieving player commands
void getPlayerCommands(Game* game, const char* endSequence);
//program finds active player's ship that can perform a shot at given coordinates, then calls extendedShoot();
void autoFire(Game* game, Player* player, int y, int x);

int main()
{
        Game game;
        char input[256];
        game.playerA.id = 'A';
        game.playerB.id = 'B';
        //set default game parameters
        initBoard(&game);
        setFleet(&(game.playerA), 1, 2, 3, 4);
        setFleet(&(game.playerB), 1, 2, 3, 4);
        setStartingArea(&(game.playerA), 0, 0, 9, 9);
        setStartingArea(&(game.playerB), 11, 0, 20, 9);
        srand(game.seed);

        while (cin >> input) {
                if (game.ended) {
                        break;
                
}
                if (compare("[state]", input)) {
                        getStateCommands(&game);
                
}
                if (compare("[playerA]", input)) {
                        if (game.activePlayer == &(game.playerA)) {
                                getPlayerCommands(&game, "[playerA]");
                        
}
                        else {
                                cout << "INVALID OPERATION  \"[playerB] \": THE OTHER PLAYER EXPECTED" << endl;
                                game.ended = true;
                        
}
                
}
                if (compare("[playerB]", input)) {
                        if (game.activePlayer == &(game.playerB)) {
                                getPlayerCommands(&game, "[playerB]");
                        
}
                        else {
                                cout << "INVALID OPERATION  \"[playerB] \": THE OTHER PLAYER EXPECTED" << endl;
                                game.ended = true;
                        
}
                
}
        
}

        //free memory
        freeBoard(&game);
        clearFleet(&(game.playerA));
        clearFleet(&(game.playerB));
        return 0;
}

void initBoard(Game* game) {
        game->board.fields = new Field * [game->board.width];

        for (int i = 0; i < game->board.width; i++) {
                game->board.fields[i] = new Field[game->board.height];

                for (int j = 0; j < game->board.height; j++) {
                        game->board.fields[i][j].state = EMPTY;
                
}
        
}
        //temporary board used for PRINT (player) commands
        game->renderBoard = new bool* [game->board.width];
        for (int i = 0; i < game->board.width; i++) {
                game->renderBoard[i] = new bool[game->board.height];

                for (int j = 0; j < game->board.height; j++) {
                        game->renderBoard[i][j] = false;
                
}
        
}
}

void freeBoard(Game* game) {
        for (int i = 0; i < game->board.width; i++) {
                delete[] game->board.fields[i];
        
}
        delete[] game->board.fields;

        for (int i = 0; i < game->board.width; i++) {
                delete[] game->renderBoard[i];
        
}
        delete[] game->renderBoard;

}

char getSimpleChar(char fieldChar) {
        if (fieldChar == CANNON || fieldChar == RADAR || fieldChar == ENGINE || fieldChar == ENCAN) {
                return SHIPPART;
        
}
        else {
                return fieldChar;
        
}
}

int getDigits(int num) {
        int res = 0;
        for (int i = 1; i < num; i *= 10) {
                res += 1;
        
}
        return res;
}

void printColumnNumbers(int widthDigits, int heightDigits, int width) {
        for (int i = 0; i < widthDigits; i++) {
                for (int j = 0; j < heightDigits; j++) {
                        cout << " ";
                
}
                for (int j = 0; j < width; j++) {
                        int temp = j;
                        for (int k = 0; k < widthDigits - i - 1; k++) {
                                temp /= 10;
                        
}
                        cout << temp % 10;
                
}
                cout << '\n';
        
}
}


void printRowNumber(int rowNum, int heightDigits) {
        for (int j = 0; j < heightDigits; j++) {
                int temp = rowNum;
                for (int k = 0; k < heightDigits - j - 1; k++) {
                        temp /= 10;
                
}
                cout << temp % 10;
        
}
}

void printBoard(Game* game, bool mode) {

        int wDigits = 0;
        int hDigits = 0;
        //print column numbers
        if (mode == 1) {
                wDigits = getDigits(game->board.width);
                hDigits = getDigits(game->board.height);
                printColumnNumbers(wDigits, hDigits, game->board.width);
        
}

        for (int i = 0; i < game->board.height; i++) {
                //print row numbers
                if (mode == 1) {
                        printRowNumber(i, hDigits);
                
}

                //print fields
                for (int j = 0; j < game->board.width; j++) {
                        char fieldChar = game->board.fields[j][i].state;
                        if (mode == 0) {
                                cout << getSimpleChar(fieldChar);
                        
}
                        else {
                                if (fieldChar == ENCAN) {
                                        cout << char(ENGINE);
                                
}
                                else {
                                        cout << fieldChar;
                                
}
                        
}
                
}
                cout << '\n';
        
}
        cout << "PARTS REMAINING:: A : " << game->playerA.partsLeft << " B : " << game->playerB.partsLeft << endl;
}

void fillRenderBoard(Game* game) {
        int width = game->board.width, height = game->board.height;
        float range = 0;
        int x, y;

        //select ship to scan with
        for (int i = 0; i < CLASSNUMBER; i++) {
                for (int j = 0; j < game->activePlayer->shipCount[i]; j++) {
                        Ship ship = game->activePlayer->ships[i][j];
                        x = ship.xPosition;
                        y = ship.yPosition;
                        //scan fields surrounding the ship
                        if (ship.radar) {
                                range = ship.size;
                        
}
                        else {
                                range = 1;
                        
}
                        for (int k = range * -1; k <= range; k++) {
                                for (int l = range * -1; l <= range; l++) {
                                        if (x + k >= 0 && x + k < width && y + l >= 0 && y + l < height) {
                                                float distance = sqrt(pow(abs(l), 2) + pow(abs(k), 2));
                                                Field originalField = game->board.fields[x + k][y + l];
                                                if (distance <= range) {
                                                        //update the temporary board (which contains only fields visible to the player)
                                                        game->renderBoard[x + k][y + l] = true;
                                                
}
                                        
}
                                
}
                        
}
                
}
        
}
}

void radarPrint(Game* game, bool mode) {
        int width = game->board.width, height = game->board.height;
        
        fillRenderBoard(game);

        int wDigits = 0;
        int hDigits = 0;
        //print column numbers
        if (mode == 1) {
                wDigits = getDigits(game->board.width);
                hDigits = getDigits(game->board.height);
                printColumnNumbers(wDigits, hDigits, game->board.width);
        
}
        for (int i = 0; i < height; i++) {
                //print row number
                if (mode == 1) {
                        printRowNumber(i, hDigits);
                
}
                //print fields
                for (int j = 0; j < width; j++) {
                        Field field = game->board.fields[j][i];

                        //field is printed only if it's on the temporary board
                        if (game->renderBoard[j][i] == true || field.visible[0] == game->activePlayer || field.visible[1] == game->activePlayer) {
                                if (mode == 0)
                                        cout << getSimpleChar(field.state);
                                else {
                                        if (field.state == ENCAN) {
                                                cout << char(ENGINE);
                                        
}
                                        cout << field.state;
                                
}
                                //clear render board
                                game->renderBoard[j][i] = false;
                        
}
                        else {
                                cout << '?';
                        
}
                
}
                cout << '\n';
        
}
}

void clearFleet(Player* player) {
        Ship* ship;
        for (int i = 0; i < CLASSNUMBER; i++) {
                for (int j = 0; j < player->shipCount[i]; j++) {
                        ship = &player->ships[i][j];
                        delete[] ship->parts;
                        delete[] ship->fields;
                
}
        
}

        for (int i = 0; i < CLASSNUMBER; i++) {
                player->shipCount[i] = -1;
                delete[] player->ships[i];
                player->ships[i] = NULL;
        
}
}

void setParts(Ship* ship) {
        for (int i = 0; i < ship->size; i++) {
                ship->parts[i] = SHIPPART;
        
}
}

void setExtendedParts(Ship* ship) {
        if (ship->parts[0] != DESTROYEDPART)
                ship->parts[0] = RADAR;

        if (ship->shipClass != DES) {
                if (ship->parts[1] != DESTROYEDPART)
                        ship->parts[1] = CANNON;
                if (ship->parts[ship->size - 1] != DESTROYEDPART)
                        ship->parts[ship->size - 1] = ENGINE;
        
}
        else {
                if (ship->parts[1] != DESTROYEDPART)
                        ship->parts[1] = ENCAN;
        
}
}

void setFleet(Player* player, int carriers, int battleships, int cruisers, int destroyers) {

        if (player->shipCount[0] != -1) {
                clearFleet(player);
        
}

        player->ships[CAR] = new Ship[carriers];
        player->ships[BAT] = new Ship[battleships];
        player->ships[CRU] = new Ship[cruisers];
        player->ships[DES] = new Ship[destroyers];

        player->shipCount[CAR] = carriers;
        player->shipCount[BAT] = battleships;
        player->shipCount[CRU] = cruisers;
        player->shipCount[DES] = destroyers;

        for (int i = 0; i < carriers; i++) {
                Ship ship;
                ship.shipClass = CAR;
                ship.size = CAR_SIZE;
                ship.parts = new char[ship.size];
                ship.fields = new Field * [ship.size];
                setParts(&ship);
                setExtendedParts(&ship);
                player->ships[CAR][i] = ship;
        
}
        for (int i = 0; i < battleships; i++) {
                Ship ship;
                ship.shipClass = BAT;
                ship.size = BAT_SIZE;
                ship.parts = new char[ship.size];
                ship.fields = new Field * [ship.size];
                setParts(&ship);
                setExtendedParts(&ship);

                player->ships[BAT][i] = ship;
        
}
        for (int i = 0; i < cruisers; i++) {
                Ship ship;
                ship.shipClass = CRU;
                ship.size = CRU_SIZE;
                ship.parts = new char[ship.size];
                ship.fields = new Field * [ship.size];
                setParts(&ship);
                setExtendedParts(&ship);

                player->ships[CRU][i] = ship;
        
}
        for (int i = 0; i < destroyers; i++) {
                Ship ship;
                ship.shipClass = DES;
                ship.size = DES_SIZE;
                ship.parts = new char[ship.size];
                ship.fields = new Field * [ship.size];
                setParts(&ship);
                setExtendedParts(&ship);

                player->ships[DES][i] = ship;
        
}
}


void setStartingArea(Player* player, int y1, int x1, int y2, int x2) {
        player->startingArea->x1 = x1;
        player->startingArea->x2 = x2;
        player->startingArea->y1 = y1;
        player->startingArea->y2 = y2;
}


int getClassIndex(char* shipClass) {
        if (memcmp(shipClass, "CAR", strlen(shipClass)) == 0) {
                return CAR;
        
}
        if (memcmp(shipClass, "BAT", strlen(shipClass)) == 0) {
                return BAT;
        
}
        if (memcmp(shipClass, "CRU", strlen(shipClass)) == 0) {
                return CRU;
        
}
        if (memcmp(shipClass, "DES", strlen(shipClass)) == 0) {
                return DES;
        
}
        return NULL;
}

int getDirection(char dir) {
        switch (dir) {
        case 'N': {return N; 
}
        case 'S': {return S; 
}
        case 'W': {return W; 
}
        case 'E': {return E; 
}
        default: {return NULL; 
}
        
}
}

int getTurnDirection(char dir) {
        switch (dir) {
        case 'L': {return L; 
}
        case 'R': {return R; 
}
        case 'F': {return F; 
}
        default: {return NULL; 
}
        
}
}


bool compare(const char command[], char input[]) {
        if (memcmp(input, command, strlen(command)) == 0) {
                return true;
        
}
        else {
                return false;
        
}
}

Player* getPlayer(Game* game, char id) {
        if (id == 'A') {
                return &(game->playerA);
        
}
        if (id == 'B') {
                return &(game->playerB);
        
}
        return NULL;
}

void changePlayer(Game* game) {
        if (game->activePlayer == &(game->playerA)) {
                game->activePlayer = &(game->playerB);
        
}
        else {
                game->activePlayer = &(game->playerA);
        
}
}



void setDirectionModifiers(int direction, int* xMod, int* yMod) {
        switch (direction) {
        case N:
                *yMod = 1;
                *xMod = 0;
                break;
        case S:
                *yMod = -1;
                *xMod = 0;
                break;
        case W:
                *xMod = 1;
                *yMod = 0;
                break;
        case E:
                *xMod = -1;
                *yMod = 0;
                break;
        
}
}

char spaceAvailable(Game* game, int x, int y, Ship* ship, int D) {
        if (ship == NULL) {
                return NULL;
        
}

        int xMod = 0;
        int yMod = 0;
        setDirectionModifiers(D, &xMod, &yMod);

        for (int i = 0; i < ship->size; i++) {
                int tempX = x + i * xMod, tempY = y + i * yMod;

                //check if ship is on board
                if (tempX >= game->board.width || tempX < 0 ||
                        tempY >= game->board.height || tempY < 0) {
                        return 'B';
                
}

                if (game->board.fields[tempX][tempY].state == REEF) {
                        game->ended = true;
                        return 'R';
                
}

                //check if ship is too close to other ships
                bool tooClose = false;
                if (game->board.fields[tempX][tempY].ship != ship)
                        if (game->board.fields[tempX][tempY].ship != NULL)
                                if (game->board.fields[tempX][tempY].ship != ship)
                                        tooClose = true;
                if (tempX + 1 < game->board.width)
                        if (game->board.fields[tempX + 1][tempY].ship != NULL)
                                if (game->board.fields[tempX + 1][tempY].ship != ship)
                                        tooClose = true;
                if (tempX - 1 >= 0)
                        if (game->board.fields[tempX - 1][tempY].ship != NULL)
                                if (game->board.fields[tempX - 1][tempY].ship != ship)
                                        tooClose = true;
                if (tempY + 1 < game->board.height)
                        if (game->board.fields[tempX][tempY + 1].ship != NULL)
                                if (game->board.fields[tempX][tempY + 1].ship != ship)
                                        tooClose = true;
                if (tempY - 1 >= 0)
                        if (game->board.fields[tempX][tempY - 1].ship != NULL)
                                if (game->board.fields[tempX][tempY - 1].ship != ship)
                                        tooClose = true;
                if (tooClose) {
                        game->ended = true;
                        return 'C';
                
}
        
}
        return NULL;
}

char placeShip(Game* game, char P, int y, int x, char D, int index, int shipClass, char* state) {
        static int totalShipsPlaced = 0;
        int xMod = 0;
        int yMod = 0;
        int direction = getDirection(D);
        Player* player;

        //P is passed only in SHIP command
        if (P == NULL) {
                player = game->activePlayer;
        
}
        else {
                player = getPlayer(game, P);
        
}

        Ship* ship = &player->ships[shipClass][index];

        if (ship->set == true) {
                game->ended = true;
                return 'P';
        
}
        if (game->activePlayer->shipCount[shipClass] <= index) {
                game->ended = true;
                return 'A';
        
}

        setDirectionModifiers(direction, &xMod, &yMod);

        //check if ship starts and ends in starting area
        Area startingArea = *(player->startingArea);
        if (x < startingArea.x1 || x > startingArea.x2 || x + xMod * (ship->size - 1) < startingArea.x1 || x + xMod * (ship->size - 1) > startingArea.x2 ||
                y < startingArea.y1 || y > startingArea.y2 || y + yMod * (ship->size - 1) < startingArea.y1 || y + yMod * (ship->size - 1) > startingArea.y2) {
                game->ended = true;
                return 'S';
        
}

        char spaceCode = spaceAvailable(game, x, y, ship, direction);
        if (spaceCode == 'C') { return 'C'; 
}
        if (spaceCode == 'R') { return 'R'; 
}

        //set ship's properties
        ship->xPosition = x;
        ship->yPosition = y;
        ship->direction = direction;

        //for command SHIP - set ship's parts (originally decalred in SET_FLEET)
        if (state != NULL) {
                for (int i = 0; i < ship->size; i++) {
                        char part = NULL;
                        if (state[i] == '0') { part = 'x'; 
}
                        if (state[i] == '1') { part = '+'; 
}
                        ship->parts[i] = part;
                
}
                setExtendedParts(ship);
        
}

        //place ship on board 
        for (int i = 0; i < ship->size; i++) {
                Field* field = &(game->board.fields[x + i * xMod][y + i * yMod]);
                field->state = ship->parts[i];
                field->part = &(ship->parts[i]);
                field->player = game->activePlayer;
                field->ship = ship;
                ship->fields[i] = field;
        
}
        ship->set = true;

        //update player's healthy parts counter
        for (int i = 0; i < ship->size; i++) {
                if (ship->parts[i] != DESTROYEDPART)
                        player->partsLeft += 1;
        
}

        //check if all ships have been placed
        if (game->set == false) {
                totalShipsPlaced += 1;
                int* Afleet = game->playerA.shipCount;
                int* Bfleet = game->playerB.shipCount;
                if (totalShipsPlaced == (Afleet[0] + Afleet[1] + Afleet[2] + Afleet[3] + Bfleet[0] + Bfleet[1] + Bfleet[2] + Bfleet[3])) {
                        game->set = true;
                
}
        
}
        return NULL;
}

void reef(Game* game, int y, int x) {
        if (y < 0 || x < 0 || y >= game->board.height || x >= game->board.width) {
                cout << "error" << endl;
                game->ended = true;
                return;
        
}

        game->board.fields[x][y].state = REEF;
}

char shoot(Game* game, int y, int x, Ship* ship) {
        if (game->set == false) {
                game->ended = true;
                return 'P';
        
}
        if (y < 0 || x < 0 || y >= game->board.height || x >= game->board.width) {
                game->ended = true;
                return 'E';
        
}
        if (ship != NULL) {
                ship->shots += 1;
        
}
        Field* field = &(game->board.fields[x][y]);
        char* hitpart = field->part;
        if (hitpart != NULL && *hitpart != DESTROYEDPART) {

                if (*hitpart == RADAR) {
                        field->ship->radar = false;
                
}
                if (*hitpart == ENGINE) {
                        field->ship->engine = false;
                
}
                if (*hitpart == ENCAN) {
                        field->ship->engine = false;
                        field->ship->cannon = false;
                
}
                if (*hitpart == CANNON) {
                        field->ship->cannon = false;
                
}
                *hitpart = DESTROYEDPART;
                field->state = DESTROYEDPART;
                field->player->partsLeft -= 1;
        
}
        return NULL;
}

char extendedShoot(Game* game, int index, int C, int y, int x, bool inRange = false) {
        Ship* ship = &game->activePlayer->ships[C][index];
        if (ship->cannon == false) {
                game->ended = true;
                return 'C';
        
}

        if (ship->shipClass == CAR) {
                return shoot(game, y, x, ship);
        
}

        if (ship->shots >= ship->size) {
                return 'T';
        
}
        if (inRange) {
                return shoot(game, y, x, ship);
        
}
        else {
                //check if target is in range
                int cannonX, cannonY, xMod, yMod;
                setDirectionModifiers(ship->direction, &xMod, &yMod);
                cannonX = ship->xPosition + xMod;
                cannonY = ship->yPosition + yMod;
                float distance = sqrt(pow(abs(cannonY - y), 2) + pow(abs(cannonX - x), 2));
                if (distance > ship->size) {
                        game->ended = true;
                        return 'F';
                
}
                else {
                        //FIREEEEE
                        return shoot(game, y, x, ship);
                
}
        
}
}

char autoFire(Game* game, int y, int x) {
        for (int i = 3; i >= 0; i--) {
                for (int j = 0; j < game->activePlayer->shipCount[i]; j++) {
                        Ship* ship = &game->activePlayer->ships[i][j];
                        int cannonX, cannonY, xMod, yMod;
                        setDirectionModifiers(ship->direction, &xMod, &yMod);
                        cannonX = ship->xPosition + xMod;
                        cannonY = ship->yPosition + yMod;
                        float distance = sqrt(pow(abs(cannonY - y), 2) + pow(abs(cannonX - x), 2));
                        if (distance <= ship->size) {
                                return extendedShoot(game, j, i, y, x, true);
                        
}
                
}
        
}
        return 'N';
}

void spy(Game* game, int index, int y, int x) {
        Ship* ship = &game->activePlayer->ships[0][index];
        if (ship->cannon == false) {
                cout << "INVALID COMMAND \"SPY " << index << " " << y << " " << x << "\": CANNOT SEND PLANE" << endl;
                game->ended;
                return;
        
}
        if (ship->shots >= ship->size) {
                cout << "INVALID COMMAND \"SPY " << index << " " << y << " " << x << "\": ALL PLANES SENT" << endl;
                game->ended;
                return;
        
}
        for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                        if (x + i >= 0 && x + i < game->board.width && y + j >= 0 && y + j < game->board.height) {
                                Field* field = &game->board.fields[x + i][y + j];
                                if (field->visible[0] == NULL || field->visible[0] == game->activePlayer) {
                                        field->visible[0] = game->activePlayer;
                                
}
                                else {
                                        field->visible[1] = game->activePlayer;
                                
}
                        
}
                
}
        
}
}

void resetShip(Player* player) {
        for (int i = 0; i < CLASSNUMBER; i++) {
                for (int j = 0; j < player->shipCount[i]; j++) {
                        Ship* ship = &player->ships[i][j];
                        ship->moved = 0;
                        ship->shots = 0;
                
}
        
}
}

char move(Game* game, int index, char* C, char D) {
        Player* player = game->activePlayer;
        int classIndex = getClassIndex(C);
        Ship* ship = &(player->ships[classIndex][index]);

        if (ship->moved >= 3 || (ship->shipClass == CAR && ship->moved >= 2)) {
                return 'M';
        
}

        if (ship->engine == false) {
                return 'E';
        
}

        int direction = ship->direction;
        int turnDirection = getTurnDirection(D);
        int newX = ship->xPosition, newY = ship->yPosition;
        int xMod = 0, yMod = 0;
        setDirectionModifiers(direction, &xMod, &yMod);
        //move 1 space in current direction
        newX -= (1 * (xMod));
        newY -= (1 * (yMod));
        //set new dierction for ship 
        if (turnDirection != F) {
                direction += turnDirection;
                if (direction > W) { direction = N; 
}
                if (direction < N) { direction = W; 
};
                setDirectionModifiers(direction, &xMod, &yMod);
                newX -= ((ship->size - 1) * (xMod));
                newY -= ((ship->size - 1) * (yMod));
        
}

        //check if space is available
        char spaceCode = spaceAvailable(game, newX, newY, ship, direction);

        if (spaceCode != NULL) {
                return spaceCode;
        
}

        ship->direction = direction;
        ship->xPosition = newX;
        ship->yPosition = newY;

        for (int i = 0; i < ship->size; i++) {
                Field* oldField = ship->fields[i];
                Field* newField = &(game->board.fields[newX + i * xMod][newY + i * yMod]);
                //clear old field
                oldField->part = NULL;
                oldField->player = NULL;
                oldField->state = EMPTY;
                oldField->ship = NULL;
                //set new field
                newField->part = &(ship->parts[i]);
                newField->player = game->activePlayer;
                newField->ship = ship;
                newField->state = ship->parts[i];

                ship->fields[i] = newField;
        
}
        ship->moved += 1;
        return NULL;
}

void printClassTag(int C) {
        switch (C) {
        case 0: {cout << "CAR" << " "; return; 
}
        case 1: {cout << "BAT" << " "; return; 
}
        case 2: {cout << "CRU" << " "; return; 
}
        case 3: {cout << "DES" << " "; return; 
}
        
}
}

char getDirectionTag(int dir) {
        switch (dir) {
        case 0: {return 'N'; 
}
        case 1: {return 'E'; 
}
        case 2: {return 'S'; 
}
        case 3: {return 'W'; 
}
        default: {return NULL; 
}
        
}
}


void printShipParts(Ship ship) {
        for (int i = 0; i < ship.size; i++) {
                if (ship.parts[i] == DESTROYEDPART) {
                        cout << 0;
                
}
                else {
                        cout << 1;
                
}
        
}
}

void save(Game* game) {
        cout << "[state]" << endl;
        cout << "BOARD_SIZE " << game->board.height << " " << game->board.width << endl;
        cout << "NEXT_PLAYER " << game->activePlayer->id << endl;

        //Player A
        char id = 'A';
        Player* player = &game->playerA;
        for (int i = 0; i < 2; i++) {
                if (i == 1) {
                        id = 'B';
                        player = &game->playerB;
                
}
                Area area = *player->startingArea;
                cout << "INIT_POSITION " << id << " " << area.y1 << " " << area.x1 << " " << area.y2 << " " << area.x2 << endl;
                cout << "SET_FLEET " << id;
                for (int j = 0; j < CLASSNUMBER; j++) {
                        cout << " " << player->shipCount[j];
                
}
                cout << endl;
                for (int j = 0; j < CLASSNUMBER; j++) {
                        for (int k = 0; k < player->shipCount[j]; k++) {
                                Ship ship = player->ships[j][k];
                                cout << "SHIP " << id << " " << ship.yPosition << " " << ship.xPosition << " " << getDirectionTag(ship.direction) << " " << k << " ";
                                printClassTag(ship.shipClass);
                                printShipParts(ship);
                                cout << endl;
                        
}
                
}
        
}

        for (int i = 0; i < game->board.width; i++) {
                for (int j = 0; j < game->board.height; j++) {
                        if (game->board.fields[i][j].state == REEF) {
                                cout << "REEF " << j << " " << i << endl;
                        
}
                
}
        
}

        if (game->extended) {
                cout << "EXTENDED_SHIPS" << endl;
        
}
        if (game->autoShooting) {
                cout << "AUTO_SHOOTING" << endl;
        
}
        if (game->playerA.AI) {
                cout << "SET_AI_PLAYER A" << endl;
        
}
        if (game->playerB.AI) {
                cout << "SET_AI_PLAYER A" << endl;
        
}
        if (game->seed != NULL) {
                cout << "SRAND " << (game->seed + 1) << endl;
        
}

        cout << "[state]" << endl;
}


void getStateCommands(Game* game) {
        char input[256];

        while (game->ended == false) {
                cin >> input;

                if (compare("SET_FLEET", input)) {
                        int data[4];
                        char playerId;
                        std::cin >> playerId >> data[0] >> data[1] >> data[2] >> data[3];

                        setFleet(getPlayer(game, playerId), data[0], data[1], data[2], data[3]);
                
}

                if (compare("SHIP", input)) {
                        int x, y, i;
                        char P, D;
                        char C[4];
                        char s[6];
                        cin >> P >> x >> y >> D >> i >> C;
                        cin >> s;
                        char code = placeShip(game, P, x, y, D, i, getClassIndex(C), s);
                        if (code != NULL) {
                                cout << "INVALID OPERATION \"SHIP " << P << " " << x << " " << y << " " << D << " " << i << " " << C << " " << s << "\": ";
                                game->ended = true;
                                switch (code) {
                                case 'A': {cout << "ALL SHIPS OF THE CLASS ALREADY SET" << endl; break; 
}
                                case 'R': {cout << "PLACING SHIP ON REEF" << endl; break; 
}
                                case 'P': {cout << "SHIP ALREADY PRESENT" << endl; break; 
}
                                case 'S': {cout << "NOT IN STARTING POSITION" << endl; break; 
}
                                case 'C': {cout << "PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl; break; 
}
                                
}
                        
}
                
}

                if (compare("PRINT", input)) {
                        bool mode;
                        cin >> mode;
                        printBoard(game, mode);

                
}

                if (compare("INIT_POSITION", input)) {
                        char playerId;
                        int y1, x1, y2, x2;
                        cin >> playerId;
                        cin >> y1 >> x1 >> y2 >> x2;
                        setStartingArea(getPlayer(game, playerId), y1, x1, y2, x2);
                
}

                if (compare("BOARD_SIZE", input)) {
                        freeBoard(game);
                        cin >> game->board.height >> game->board.width;
                        initBoard(game);
                
}

                if (compare("SRAND", input)) {
                        cin >> game->seed;
                        srand(game->seed);
                
}

                if (compare("NEXT_PLAYER", input)) {
                        char id;
                        cin >> id;
                        game->activePlayer = getPlayer(game, id);
                
}

                if (compare("REEF", input)) {
                        int x, y;
                        cin >> y >> x;
                        reef(game, y, x);
                
}

                if (compare("EXTENDED_SHIPS", input)) {
                        if (game->extended == false) {
                                game->extended = true;
                        
}
                        else {
                                game->extended = false;
                        
}
                
}

                if (compare("AUTO_SHOOTING", input)) {
                        if (game->autoShooting == false) {
                                game->autoShooting = true;
                        
}
                        else {
                                game->autoShooting = false;
                        
}
                
}

                if (compare("SET_AI_PLAYER", input)) {
                        char p;
                        cin >> p;
                        Player* player = getPlayer(game, p);
                        if (player->AI == false) {
                                player->AI= true;
                        
}
                        else { player->AI = false; 
}
                
}

                if (compare("SAVE", input)) {
                        save(game);
                
}

                if (compare("[state]", input)) { break; 
}
        
}

}

void getPlayerCommands(Game* game, const char* endSequence) {
        char input[256];
        resetShip(game->activePlayer);

        while (game->ended == false) {
                cin >> input;

                if (compare("PLACE_SHIP", input)) {
                        int x, y, i;
                        char D;
                        char C[4];
                        cin >> x >> y >> D >> i >> C;
                        char code = placeShip(game, NULL, x, y, D, i, getClassIndex(C));
                        if (code != NULL) {
                                cout << "INVALID OPERATION \"PLACE_SHIP " << x << " " << y << " " << D << " " << i << " " << C << "\": ";
                                switch (code) {
                                case 'A': {cout << "ALL SHIPS OF THE CLASS ALREADY SET" << endl; break; 
}
                                case 'R': {cout << "PLACING SHIP ON REEF" << endl; break; 
}
                                case 'P': {cout << "SHIP ALREADY PRESENT" << endl; break; 
}
                                case 'S': {cout << "NOT IN STARTING POSITION" << endl; break; 
}
                                case 'C': {cout << "PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl; break; 
}

                                
}
                                game->ended = true;
                        
}

                
}

                if (compare("SHOOT", input)) {
                        char code;
                        if (game->extended == true && game->autoShooting == false) {
                                int i, y, x;
                                char C[4];
                                cin >> i >> C >> y >> x;
                                int c = getClassIndex(C);
                                code = extendedShoot(game, i, c, y, x);
                                if (code != NULL)
                                        cout << "INVALID OPERATION \"SHOOT " << i << " " << C << " " << y << " " << x << "\": ";
                        
}else {
                                int x, y;
                                cin >> y >> x;
                                if (game->extended == true) {
                                        code = autoFire(game, y, x);
                                
}
                                else {
                                        code = shoot(game, y, x);
                                
}
                                if (code != NULL)
                                        cout << "INVALID OPERATION \"SHOOT " << y << " " << x << "\": ";
                        
}
                        if (code != NULL) {
                                switch (code) {
                                case 'C': {cout << "SHIP CANNOT SHOOT" << endl; break; 
}
                                case 'F': {cout << "SHOOTING TOO FAR" << endl; break; 
}
                                case 'E': {cout << "FIELD DOES NOT EXIST" << endl; break; 
}
                                case 'P': {cout << "NOT ALL SHIPS PLACED" << endl; break; 
}
                                case 'T': {cout << "TOO MANY SHOOTS" << endl; break; 
}
                                case 'N': {cout << "SHOOTS ARE UNFEASIBLE" << endl; break; 
}
                                
}
                        
}
                
}

                if (compare("MOVE", input)) {
                        int i;
                        char D;
                        char C[4];
                        cin >> i >> C >> D;
                        char code = move(game, i, C, D);
                        if (code != NULL) {
                                cout << "INVALID OPERATION \"MOVE " << i << " " << C << " " << D << "\": ";
                                switch (code) {
                                case 'R': { cout << "PLACING SHIP ON REEF" << endl; break; 
}
                                case 'C': {cout << "PLACING SHIP TOO CLOSE TO OTHER SHIP" << endl; break; 
}
                                case 'B': {cout << "SHIP WENT FROM BOARD" << endl; break; 
}
                                case 'E': {cout << "SHIP CANNOT MOVE" << endl; break; 
}
                                case 'M': {cout << "SHIP MOVED ALREADY" << endl; break; 
}
                                
}
                                game->ended = true;
                        
}
                
}

                if (compare("PRINT", input)) {
                        bool mode;
                        cin >> mode;
                        radarPrint(game, mode);
                
}

                if (compare("SPY", input)) {
                        int x, y, i;
                        cin >> i >> y >> x;
                        spy(game, i, y, x);
                
}

                if (compare(endSequence, input)) {
                        if (game->set == true) {
                                if (game->playerA.partsLeft <= 0) {
                                        game->winner = 'B';
                                
}
                                else if (game->playerB.partsLeft <= 0) {
                                        game->winner = 'A';
                                
}
                        
}
                        if (game->winner != NULL) {
                                cout << game->winner << " won" << endl;
                                game->ended = true;
                        
}
                        changePlayer(game);
                        break;
                
}
        
}
}
