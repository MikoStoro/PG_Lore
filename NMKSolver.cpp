#include <iostream>
#include <string>
#include <string.h>
#define A 1
#define B 2
#define NONE 0

using namespace std;

class Game {
private:
    int n, m, k;
    int** board;
    int firstPlayer = A;
    int chain = 0;
    int movesCount = 0;

    struct move {
        int x;
        int y;
        int player;
    
};

    move* moves;

public:
    Game(int n, int m, int k) {
        this->n = n;
        this->m = m;
        this->k = k;
        board = new int* [n];
        moves = new move[n*m];
        for (int i = 0; i < n; i++) {
            board[i] = new int[m];
            for (int j = 0; j < m; j++) {
                board[i][j] = NONE;
            
}
        
}
    
}

    //returns 1 when first player wins
    //returns -1 when second player wins
    //returns 0 when no one wins 
    int chainUp(int x, int y) {
        if (board[x][y] == NONE) {
            chain = 0;
        
}
        else if (board[x][y] == firstPlayer) {
            if (chain < 0) {
                    chain = 1;
            
}else {
                    chain += 1;
            
}
            if (chain >= k) { return 1; 
}
        
}
        else {
            if (chain > 0) {
                chain = -1;
            
}
            else {
                chain -= 1;
            
}
            if (chain <= -1 * k) { return -1; 
}
        
}
        return 0;
    
}


    void resetChain() {
        chain = 0;
    
}

    void loadBoard() {
        for (int x = 0; x < n; x++){
            char* line = new char[2*m];

            fgets(line, 2*m, stdin);
            if(line[0] = '\n'){
                fgets(line, 2*m, stdin);
            
}
            for (int y = 0; y < m; y++) {
                 board[x][y] = (line[y*2] - '0');
            
}

            delete[] line;
        
}
    
}

    void printBoard() {
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < m; y++) {
                putchar(board[x][y]+'0');
                putchar(' ');
            
}
            putchar('\n');
        
}
    
}

    void setFirstPlayer(int player) {
        firstPlayer = player;
    
}

    void addMove(move mov) {
        board[mov.x][mov.y] = mov.player;
    
}

    void reverseMove(move mov) {
        board[mov.x][mov.y] = NONE;
    
}

    void printMove(move mov) {
        board[mov.x][mov.y] = mov.player;
        printBoard();
        board[mov.x][mov.y] = NONE;
    
}

    int getOtherPlayer() {
        if (firstPlayer == A) {
            return B;
        
}
        else {
            return A;
        
}
    
}

    int getOtherPlayer(int player) {
        if (player == A) {
            return B;
        
}
        else {
            return A;
        
}
    
}

    int checkStateHorizontal() {
        resetChain();
        for (int y = 0; y < m; y++) {
            for (int x = 0; x < n; x++) {
                int res = chainUp(x, y);
                if (res != 0) { return res; 
}
            
}
            resetChain();
        
}
        resetChain();
        return 0;
    
}

    int checkStateVertical() {
        resetChain();
        for (int x = 0; x < n; x++) {
            for (int y = 0; y < m; y++) {
                int res = chainUp(x, y);
                if (res != 0) { return res; 
}
            
}
            resetChain();
        
}
        resetChain();
        return 0;
    
}

    int checkStateDiagnal() {
        resetChain();
        int x, y;
        y = 0;
        for (x = 0; x < n; x++) {
            int tx = x;
            int ty = y;
            while (tx < n && ty < m) {
                int res = chainUp(tx, ty);
                if (res != 0) { return res; 
}
                tx++; ty++;
            
}
            resetChain();
        
}
        x = 0;
        for (y = 1; y < m; y++) {
            int tx = x;
            int ty = y;
            while (tx < n && ty < m) {
                int res = chainUp(tx, ty);
                if (res != 0) { return res; 
}
                tx++; ty++;
            
}
            resetChain();
        
}
        y = 0;
        for (x = n-1; x >=0 ; x--) {
            int tx = x;
            int ty = y;
            while (tx >= 0 && ty < m) {
                int res = chainUp(tx, ty);
                if (res != 0) { return res; 
}
                tx--; ty++;
            
}
            resetChain();
        
}
        x = n-1;
        for (y = 1; y < m; y++) {
            int tx = x;
            int ty = y;
            while (tx >= 0 && ty < m) {
                int res = chainUp(tx, ty);
                if (res != 0) { return res; 
}
                tx--; ty++;
            
}
            resetChain();
        
}
        resetChain();
        return 0;
    
}

    void evaluate(int result) {
        if (result == 0) { printf("BOTH_PLAYERS_TIE\n");
}
        if (result > 0) {
            if(firstPlayer == A){
                    printf("FIRST_PLAYER_WINS\n");
            
}else{
                printf("SECOND_PLAYER_WINS\n");
            
}
        
}
        if(result<0){
                if(firstPlayer == A){
                printf("SECOND_PLAYER_WINS\n");
            
}else{
                printf("FIRST_PLAYER_WINS\n");
            
}
        
}
    
}

    int playerLoses(int player){
        if(player == firstPlayer){
            return -1;
        
}else{
            return 1;
        
}
    
}

    int playerWins(int player){
         if(player == firstPlayer){
            return 1;
        
}else{
            return -1;
        
}
    
}

    

    int minmax(int player){
        int ret = checkVictory();
        if(ret!=0){
            return(ret);
        
}

        bool noMovesMade = true;
        ret = playerLoses(player);
        for(int x = 0; x< n; x++){
            for(int y = 0; y < m; y++){
                if (board[x][y] == NONE) {
                    noMovesMade = false;
                    move m = createMove(x,y,player);
                    addMove(m);
                    int score = minmax(getOtherPlayer(player));
                    reverseMove(m);
                    if(score == 0){ret = 0;
}
                    if(score == playerWins(player)){

                        return score;
                    
}
                
}
            
}
        
}
        if(noMovesMade){
            return 0;
        
}else{
            return ret;
        
}
        
    
}

    //returns 1 when first player wins
    //returns -1 when second player wins
    //returns 0 when no one wins 
    int checkVictory() {
        int state = checkStateHorizontal();
        if (state != 0) { return state; 
}

        state = checkStateVertical();
        if (state != 0) { return state; 
}

        state = checkStateDiagnal();
        if (state != 0) { return state; 
}
        return 0;
    
}

    move createMove(int x, int y, int player) {
        move m;
        m.x = x;
        m.y = y;
        m.player = player;
        return m;
    
}



    int generateMoves(int player, bool cut = false) {
        movesCount = 0;
        int victory = checkVictory();
        if (victory!=0) { return victory; 
}

        for (int x = 0; x < n; x++) {
            for (int y = 0; y < m; y++) {
                if (board[x][y] == NONE) {
                    move mov = createMove(x, y, player);
                    if(cut){
                        addMove(mov);
                        victory = checkVictory();
                        if (victory != 0) {
                            movesCount = 1;
                            moves[0] = mov;
                            reverseMove(mov);
                            return victory;
                        
}
                        reverseMove(mov);
                    
}    
                    moves[movesCount] = mov;
                    movesCount++;
                
}
            
}
        
}

        return 0;
    
}

    void freeMemory(){
        for(int i = 0; i < n; i++){
            delete[] board[i];
        
}
        delete[] board;
        delete[] moves;
    
}

    void printMoves(){
        printf("%d\n", movesCount);
        for(int i = 0; i < movesCount; i++){
            printMove(moves[i]);
        
}
    
}

};

void getInput() {
    Game* game = nullptr;
    char command[35];
    int n, m, k, player;
    int check = scanf("%s %d %d %d %d", command, &n, &m, &k, &player);
    

    while (check != EOF) {   
        if (!strcmp(command, "GEN_ALL_POS_MOV")) {
            game = new Game(n,m,k);
            game->loadBoard();
            game->generateMoves(player);
            game->printMoves();
            game->freeMemory();
            delete game;
        
}
        if (!strcmp(command, "GEN_ALL_POS_MOV_CUT_IF_GAME_OVER")) {
            game = new Game(n, m, k);
            game->loadBoard();
            game->generateMoves(player, true);
            game->printMoves();
            game->freeMemory();
            delete game;
        
}

        if (!strcmp(command, "SOLVE_GAME_STATE")) {
            game = new Game(n, m, k);
            game->setFirstPlayer(player);
            game->loadBoard();
            game->evaluate(game->minmax(player));
            game->freeMemory();
            delete game;
        
}
        check = scanf("%s %d %d %d %d", command, &n, &m, &k, &player);
    
}
}


int main(){
    getInput();
    return 0;
}
