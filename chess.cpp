#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#include"square.h"
#include"chessPiece.h"
#include"player.h"

// ******** GLOBAL VARS *********
std:: vector<Square> ChessBoard(64); // chess board contains 64 squares
Square lastOldSq; 
Square lastNewSq;
Player playerBlk;
Player playerWht;
bool whiteTurn = true;
bool quitBtn = false; 
bool blackIsChecked = false;
bool whiteIsChecked = false;

// required to implement En Passant 
Square lastFromSq; 
Square lastToSq;
ChessPiece lastPiece; 
std:: vector<std:: vector<Square>> boardHistory;
std:: vector<int> HR_X_MOVES = {1, 1, 2, 2, -1, -1, -2, -2};
std:: vector<int> HR_Y_MOVES = {2, -2, 1, -1, 2, -2, 1, -1};

//******* until func ********
void printSqVec(std::vector<Square> sq){ // print information of each square (DEBUG FUNCTION)
    for(int i = 0; i < sq.size();i++){
        sq.at(i).printSquareInf();
    }
}

void printMsg(std:: string msg){ //pretty message printing
    for(int i = 0; i < msg.size() + 4; i++){
        std:: cout << "-";
    }
    std:: cout<< "\n| " << msg << " |\n";
    for(int i = 0; i < msg.size() + 4; i++){
        std:: cout << "-";
    }
    std:: cout<< "\n\n";
}

bool searchInArr(char letter, std:: string arr){
    for(int i = 0; i < arr.size(); i++){
        if(letter == arr[i]){
            return true;
        }
    }
    return false;
}

// ************************* //

void initBoard(){// ititiate chess board squares
    for(int j = 0; j < 8; j++){
        for(int i = 0; i < 8; i++){
            ChessBoard.at(i + (j * 8)).setState(true);
            ChessBoard.at(i + (j * 8)).setX(i);
            ChessBoard.at(i + (j * 8)).setY(j);

        }
    }
}

std:: string drawPiece(pieceType type){ 
    std:: string squareStr;
    switch (type)
    {
    case PAWN: squareStr = "| PAW ";
        break;
    case ROOK: squareStr = "| ROK ";
        break;
    case HORSE: squareStr = "| HRS ";
        break;
    case BISHUP: squareStr = "| BIS ";
        break;
    case QUEEN: squareStr = "| Qun ";
        break;
    case KING: squareStr = "| Kin ";
        break;
    
    default: squareStr = "|     ";
        break;
    }
    return squareStr;
}

void drawChessBoard(){  
    std:: string boardStr = "  ";
    int size = sqrt(ChessBoard.size());
    for(int i = 0; i < size; i++){
        boardStr += "+-----";
    }
    boardStr += "+\n";
    for(int i = 0; i < size; i++){
        std:: string rowNum = std:: to_string(1 + i);
        boardStr += rowNum + " ";
        for(int j = 0; j < size; j++){
            if(ChessBoard.at(j + (i * 8)).isEmpty()){ boardStr += "|     "; }
            else{ 
                boardStr += drawPiece(ChessBoard.at(j + (i * 8)).piece.getType());
            }
        }
        boardStr += "|\n  ";
        for(int j = 0; j < size; j++){ // this might be deleted. maKING square too big makes drawing chess pieces harder
            if(ChessBoard.at(j + (i * 8)).isEmpty()){ boardStr += "|     "; }
            else{ 
                if(ChessBoard.at(j + (i * 8)).piece.getColor() == WHITE){
                    boardStr += "| WTE ";
                }
                else { 
                    boardStr += "| BLK ";
                }
            }
        }
        boardStr += "|\n  ";
        for(int j = 0; j < size; j++){
            boardStr += "+-----"; 
        }
        boardStr += "+\n";

    }
    boardStr += "  ";
    for(int i = 0; i < 8; i ++){
        char col = 'a';
        col = col + i;
        std:: string colLetter(1, col);
        boardStr += "   " + colLetter + "  ";
    }
    std:: cout<< boardStr << std:: endl;
}

ChessPiece makePiece(pieceColor color, pieceType type){ //creates chess piece
    ChessPiece piece;
    piece.setColor(color);
    piece.setType(type);
    piece.flagged = false;
    return piece;
}

void putPieces(){ // orders pieces on boards 
    for(int i = 0; i < 16; i++){
        if(i == 0 || i ==7) {ChessBoard.at(i).piece = makePiece(WHITE, ROOK) ;}
        else if(i == 1 || i ==6) ChessBoard.at(i).piece = makePiece(WHITE, HORSE);
        else if(i == 2 || i ==5) ChessBoard.at(i).piece = makePiece(WHITE, BISHUP);
        else if(i == 3) ChessBoard.at(i).piece = makePiece(WHITE, QUEEN);
        else if(i == 4) ChessBoard.at(i).piece = makePiece(WHITE, KING);
        else ChessBoard.at(i).piece = makePiece(WHITE, PAWN);
        ChessBoard.at(i).setState(false);
    }
    for(int i = 48; i < 64; i++){
        if(i == 56 || i ==63) ChessBoard.at(i).piece = makePiece(BLACK, ROOK);
        else if(i == 57 || i ==62) ChessBoard.at(i).piece = makePiece(BLACK, HORSE);
        else if(i == 58 || i ==61) ChessBoard.at(i).piece = makePiece(BLACK, BISHUP);
        else if(i == 59) ChessBoard.at(i).piece = makePiece(BLACK, QUEEN);
        else if(i == 60) ChessBoard.at(i).piece = makePiece(BLACK, KING);
        else ChessBoard.at(i).piece = makePiece(BLACK, PAWN);
        ChessBoard.at(i).setState(false);
    }
}

int findSquare(int x, int y){ // returns square index in ChessBoard vector
    for(int i = 0; i < ChessBoard.size(); i++){
        if(ChessBoard.at(i).getX() == x && ChessBoard.at(i).getY() == y){
            return i;
        }
    }
    return -1;
}

bool checkEnpassant(Square mySq){// checks if En Passant rule is applicable 
    if(lastPiece.getType() == PAWN && abs(lastFromSq.getY() - lastToSq.getY()) == 2 && abs(mySq.getX() - lastToSq.getX()) == 1 && abs(mySq.getY()) - abs(lastToSq.getY()) == 0){
        return true;
    }
    return false;
}

std:: vector<Square>  calcPawnMoves(Square sq){
    std:: vector<Square> moves(0);
    if(sq.isEmpty()) return moves;
    ChessPiece myPAWN = sq.piece;
    int myY = sq.getY();
    int myX = sq.getX();
    int squareIndex = findSquare( myX, myY);
    int destination = 1;
    if(myPAWN.getColor() == BLACK){ destination = -1; }
   
    int firstInd = findSquare(myX, myY + (1 * destination) );
    int secondInd = findSquare(myX, myY + (2 * destination) );
    int thirdInd = findSquare(myX - 1, myY + (1 * destination) );
    int fourthInd = findSquare(myX + 1, myY + (1 * destination) );
    if(firstInd != -1 && ChessBoard.at(firstInd).isEmpty()){
        moves.push_back(ChessBoard.at(firstInd));
        if(secondInd != -1 && ChessBoard.at(secondInd).isEmpty() && ((myY == 1 && destination == 1) ||(myY == 6 && destination == -1) ) ){
            moves.push_back(ChessBoard.at(secondInd));
        }
    }
    if(thirdInd != -1 && !ChessBoard.at(thirdInd).isEmpty() && ChessBoard.at(thirdInd).piece.getColor() != myPAWN.getColor()){
        moves.push_back(ChessBoard.at(thirdInd));
    }
    if(fourthInd != -1 && !ChessBoard.at(fourthInd).isEmpty() && ChessBoard.at(fourthInd).piece.getColor() != myPAWN.getColor()){
        moves.push_back(ChessBoard.at(fourthInd));
    }
    // implement En Passant rule
    if(checkEnpassant(sq)){
        Square newMove = lastToSq;
        int destination = 1;
        if(myPAWN.getColor() == BLACK){ destination = -1;}
        newMove.setY(newMove.getY() + destination);
        moves.push_back(newMove);
    }
    return moves;
}

std:: vector<Square> calcRookMoves(Square sq){
    std:: vector<Square> moves(0);
    ChessPiece myROOK = sq.piece;
    int myX = sq.getX();
    int myY = sq.getY();
    int dest = 1; 
    for(int j = 0; j < 2; j ++){
        for(int i = 1; i < 8; i ++){
            int ind = findSquare(myX, myY + (i * dest));
            if(ind == -1) break;
            if(ChessBoard.at(ind).isEmpty()){
                moves.push_back(ChessBoard.at(ind));
            }
            else if(ChessBoard.at(ind).piece.getColor() == myROOK.getColor()){ 
                break; 
            } 
            else{
                moves.push_back(ChessBoard.at(ind)); 
                break; 
            }
        }
        for(int i = 1; i < 8; i ++){
            int ind = findSquare(myX + (i * dest), myY);
            if(ind == -1) break;
            if(ChessBoard.at(ind).isEmpty()){
                moves.push_back(ChessBoard.at(ind));
            }
            else if(ChessBoard.at(ind).piece.getColor() != myROOK.getColor()){ 
                moves.push_back(ChessBoard.at(ind));
                break; 
            } 
            else{ 
                break; 
            }
        }
        dest = -1; 
    }
    return moves;
}

std:: vector<Square> calcBishupMoves(Square sq){
    std:: vector<Square> moves(0);
    ChessPiece ChessBoardi = sq.piece;
    int myX = sq.getX();
    int myY = sq.getY();
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX + j , myY + j);
        if(ind == -1) break;
        else if(ChessBoard.at(ind).isEmpty()) {
            moves.push_back(ChessBoard.at(ind));
        }
        else if(ChessBoard.at(ind).piece.getColor() != ChessBoardi.getColor()){
            moves.push_back(ChessBoard.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX + j, myY - j);
        if(ind == -1) break;
        else if(ChessBoard.at(ind).isEmpty()) {
            moves.push_back(ChessBoard.at(ind));
        }
        else if(ChessBoard.at(ind).piece.getColor() != ChessBoardi.getColor()){
            moves.push_back(ChessBoard.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX - j, myY - j);
        if(ind == -1) break;
        else if(ChessBoard.at(ind).isEmpty()) {
            moves.push_back(ChessBoard.at(ind));
        }
        else if(ChessBoard.at(ind).piece.getColor() != ChessBoardi.getColor()){
            moves.push_back(ChessBoard.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX - j, myY + j);
        if(ind == -1) break;
        else if(ChessBoard.at(ind).isEmpty()) {
            moves.push_back(ChessBoard.at(ind));
        }
        else if(ChessBoard.at(ind).piece.getColor() != ChessBoardi.getColor()){
            moves.push_back(ChessBoard.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    return moves;
    
}

std:: vector<Square> calcQueenMoves(Square sq){
    std:: vector<Square> moves(0);
    std:: vector<Square> moves_2(0);
    moves = calcBishupMoves(sq);
    moves_2 = calcRookMoves(sq);
    for(int i = 0; i < moves_2.size(); i ++){
        moves.push_back(moves_2.at(i));
    }
    return moves;
}

std:: vector<Square> calcKingMoves(Square sq){
    std:: vector<Square> moves(0);
    ChessPiece myKing = sq.piece;
    int x = sq.getX();
    int y = sq.getY(); 
    int ind_1 = findSquare(x + 1, y);
    int ind_2 = findSquare(x - 1, y);
    int ind_3 = findSquare(x + 1, y + 1);
    int ind_4 = findSquare(x - 1, y + 1);
    int ind_5 = findSquare(x , y + 1);
    int ind_6 = findSquare(x , y - 1);
    int ind_7 = findSquare(x - 1, y - 1);
    int ind_8 = findSquare(x + 1, y - 1);
    if(ind_1 != -1 && (ChessBoard.at(ind_1).isEmpty() || ChessBoard.at(ind_1).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_1)); }
    if(ind_2 != -1 && (ChessBoard.at(ind_2).isEmpty() || ChessBoard.at(ind_2).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_2)); }
    if(ind_3 != -1 && (ChessBoard.at(ind_3).isEmpty() || ChessBoard.at(ind_3).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_3)); }
    if(ind_4 != -1 && (ChessBoard.at(ind_4).isEmpty() || ChessBoard.at(ind_4).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_4)); }
    if(ind_5 != -1 && (ChessBoard.at(ind_5).isEmpty() || ChessBoard.at(ind_5).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_5)); }
    if(ind_6 != -1 && (ChessBoard.at(ind_6).isEmpty() || ChessBoard.at(ind_6).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_6)); }
    if(ind_7 != -1 && (ChessBoard.at(ind_7).isEmpty() || ChessBoard.at(ind_7).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_7)); }
    if(ind_8 != -1 && (ChessBoard.at(ind_8).isEmpty() || ChessBoard.at(ind_8).piece.getColor() != myKing.getColor()) ){ moves.push_back(ChessBoard.at(ind_8)); }
    return moves;
}

std:: vector<Square> calcHorseMoves(Square sq){
    std:: vector<Square> moves(0);
    int myColor = sq.piece.getColor();
    int x = sq.getX();
    int y = sq.getY();
    for(int i = 0; i < HR_X_MOVES.size(); i++){
        int newX = x + HR_X_MOVES.at(i); 
        int newY = y + HR_Y_MOVES.at(i);
        int newsquareIndex = findSquare(newX, newY);
        if( newsquareIndex != -1 && (ChessBoard.at(newsquareIndex).isEmpty() || ChessBoard.at(newsquareIndex).piece.getColor() != myColor) ){
            moves.push_back(ChessBoard.at(newsquareIndex));
        }
    }
    return moves;
}

void move(Square oldSq, Square newSq){
    int oldInd = findSquare(oldSq.getX(), oldSq.getY());
    int newInd = findSquare(newSq.getX(), newSq.getY());
    if(ChessBoard.at(oldInd).piece.getType() == PAWN && ChessBoard.at(newInd).isEmpty() && (ChessBoard.at(oldInd).getX() != ChessBoard.at(newInd).getX())){
        int dest = -1; 
        if(ChessBoard.at(oldInd).piece.getColor() == BLACK){dest = 1;}
        ChessBoard.at(newInd + 8*dest).setState(true);
    }
    ChessBoard.at(newInd).piece = ChessBoard.at(oldInd).piece;
    ChessBoard.at(newInd).setState(false);
    ChessBoard.at(oldInd).setState(true);
    boardHistory.push_back(ChessBoard);
}

void setLastMove(Square from, Square to){
    /*
    this function sets important variables to enable checkEnpassant() 
    tracking last move is required to know when En Passant rule is aaplicable
    */ 
    lastFromSq = from;
    lastToSq = to;
    lastPiece = ChessBoard.at(findSquare(to.getX(), to.getY())).piece;
}

void undoMove(){
    boardHistory.pop_back();
    ChessBoard = boardHistory.back();
}

std:: vector<Square> calcMoves(Square sq){
    std:: vector<Square> moves(0);
    if(sq.isEmpty()) return moves;
    int type = sq.piece.getType();
    switch(type){
        case PAWN:
            moves = calcPawnMoves(sq);
            break;
        case ROOK:
            moves = calcRookMoves(sq);
            break;
        case HORSE: 
            moves = calcHorseMoves(sq);
            break;
        case BISHUP:
            moves = calcBishupMoves(sq);
            break;
        case QUEEN:
            moves = calcQueenMoves(sq);
            break;
        case KING:
            moves = calcKingMoves(sq);
            break;
        default:
            std:: cout<< "[ERROR] no type\n";
            break;
    }
    return moves;
}

std:: vector<Square>  calcAllMoves(pieceColor color){ //calc all moves of a player
    std:: vector<Square> allMoves(0);
    for(int i = 0; i < ChessBoard.size(); i ++){
        if(!ChessBoard.at(i).isEmpty() && ChessBoard.at(i).piece.getColor() == color){
            std:: vector<Square> pieceMoves = calcMoves(ChessBoard.at(i));
            for(int j = 0; j < pieceMoves.size(); j++){
                allMoves.push_back(pieceMoves.at(j));
            }
        }
    }
    
    return allMoves; 
}

int chooseMove(std:: vector<Square> moves){
    int moveNum;
    std:: cout<< "Choose a move:\n";
    for(int i = 0; i < moves.size(); i++){
        char x = moves.at(i).getX() + 97;
        int y = moves.at(i).getY() + 1;
        std:: cout<< i << ": " <<  x  << y << std:: endl;
        if(i == moves.size() -1){
            std:: cout<< i+1 << ": change piece\n";
        }
    }
    std:: cin>> moveNum; 
    while(moveNum < 0 || moveNum > moves.size() ){
        printMsg("GIVE A VALID CHOICE");
        std:: cin >> moveNum;
    }
    return moveNum;
}

bool isGivingCheck(pieceColor color){// true if a player is checked
    std:: vector<Square> allMoves = calcAllMoves(color);
    for(int i = 0; i < allMoves.size(); i++){
        int ind = findSquare(allMoves.at(i).getX(), allMoves.at(i).getY());
        if( !ChessBoard.at(ind).isEmpty() && 
            ChessBoard.at(ind).piece.getColor() != color && 
            ChessBoard.at(ind).piece.getType() == KING ){
            if(color == WHITE){
                blackIsChecked = true;
            }
            else {
                whiteIsChecked = true;
            }
            return true;
        }
    }
    if(color == WHITE) blackIsChecked = false;
    else whiteIsChecked = false;
    return false;
}

std:: vector<Square> legalise(std:: vector<Square> moves,  Square curPiece){ 
    pieceColor color = curPiece.piece.getColor();
    std:: vector<Square> legalMoves;
    for(int i = 0; i < moves.size(); i++){
        int x = moves.at(i).getX();
        int y = moves.at(i).getY();
        int ind = findSquare(x,y);

        if(color == WHITE){
            move(curPiece, ChessBoard.at(ind));
            isGivingCheck(BLACK);
            if(!whiteIsChecked){
                legalMoves.push_back(moves.at(i));
            }
            undoMove();
            isGivingCheck(BLACK);            
        }
        else{
            move(curPiece, ChessBoard.at(ind));
            isGivingCheck(WHITE);
            if(!blackIsChecked){
                legalMoves.push_back(moves.at(i));
            }
            undoMove();
            isGivingCheck(WHITE);
        }
    }
    return legalMoves;
}

bool gameOver(){
    std:: vector<Square> allWhiteMoves;
    std:: vector<Square> allBlackMoves;

    for(int i = 0; i < ChessBoard.size(); i++){
        Square sq = ChessBoard.at(i);
        if(!sq.isEmpty()){
            std:: vector<Square> moves = calcMoves(sq);
            moves = legalise(moves, sq);
            if(sq.piece.getColor() == WHITE){
                allWhiteMoves.insert( allWhiteMoves.end(), moves.begin(), moves.end() );
            }
            else if(sq.piece.getColor() == BLACK){
                allBlackMoves.insert(allBlackMoves.end(), moves.begin(), moves.end());
            }
        }
    }

    if(isGivingCheck(BLACK) && allWhiteMoves.size() == 0 ){
        printMsg("BLACK WINS");
        return true;

    } else if(isGivingCheck(WHITE) && allBlackMoves.size() == 0){
        printMsg("WHITE WINS");
        return true;
    }
    else {
        return false;
    }

}

bool unvalidSq(int squareIndex){ // true if chosen square is empty or has opponents piece
    if(ChessBoard.at(squareIndex).isEmpty()){
        printMsg("EMPTY SQUARE");
        return true;
    }
    if(whiteTurn){
        if(ChessBoard.at(squareIndex).piece.getColor() != WHITE){
            printMsg("must choose a white piece");
            return true;
        }
    }
    else{
        if(ChessBoard.at(squareIndex).piece.getColor() != BLACK){
            printMsg("must choose a black piece");
            return true;
        }
    }
    return false;
}

int promptSquare(){ 
    /*
     Ensures input validity OR quits game
     This functions prompts players for to choose a chess piece to move
     - offers an option to quit game 
     - ensures input validity 
     - ensures chosen peice has valid moves
     */
    std:: cin.clear();
    std:: string message = "White To Move  ***  ";
    if( !whiteTurn){
        message = "Black to Move *** ";
    }
    while(true){
        printMsg( message + "Choose a piece to move");
        std:: string input; 
        std:: cin >> input;
        if(input == "q" || input == "quit" ){ // quitting option
            printMsg("Do you want to quit? type 'y' or 'yes' to quit");
            std:: string answer; 
            std:: cin >> answer;
            if(answer == "y"|| answer == "yes"){
                printMsg("   QUITTING   ");
                exit(EXIT_SUCCESS);
            }
            else{
                continue;
            }
        }
        else if(input.size() != 2 || !searchInArr(input[0], "abcdefgh") || !searchInArr(input[1], "12345678") ){ // invalid input
            printMsg("Choose a valid Square");
            continue;
        }
        else{ // valid squares
            int squareIndex = findSquare(input[0] - 97, std:: stoi(input.substr(1,1)) - 1);
            return squareIndex;
        }
    }
}

void handleChecks(){
    if(whiteTurn && isGivingCheck(WHITE)){
        printMsg( "*** BLACK CHECKED ***");
    }
    else if(!whiteTurn && isGivingCheck(BLACK)){
        printMsg( "*** WHITE CHECKED ***");
    }
}

void setPlayers(){// take players names and assign chess sets to them
    std:: string name1, name2; 
    printMsg("Enter the name of the first player");
    std:: cin >> name1; 
    printMsg("Enter the name of the second player"); 
    while(std:: cin >> name2){
        if(name2 == name1){
            printMsg("Players should have different names. Please renter the name of the second player");
        }
        else{
            playerWht.setName(name1);
            playerBlk.setName(name2);
            playerWht.setColor(WHITE);
            playerBlk.setColor(BLACK);
            break;
        }
    }
}

int main(){
    setPlayers();
    initBoard();
    putPieces();
    boardHistory.push_back(ChessBoard);

    while(!gameOver()){
        drawChessBoard();
        int squareIndex = promptSquare();
        if(unvalidSq(squareIndex)){
        continue;
        }
        std:: vector<Square> moves = calcMoves(ChessBoard.at(squareIndex));
        moves = legalise(moves, ChessBoard.at(squareIndex)); 
        if(moves.size() == 0){
            printMsg("CHOOSE A PIECE WITH LEGAL MOVES");
            continue;
        }
        int moveNum = chooseMove(moves);
        if(moveNum == moves.size()){ //opted to change chosen piece 
            continue;
        }

        move(ChessBoard.at(squareIndex), moves.at(moveNum));
        setLastMove(ChessBoard.at(squareIndex), moves.at(moveNum)); // Enpassant helper funtion
        handleChecks();
        (whiteTurn)? whiteTurn = false : whiteTurn = true;
    }
    return 0;
}