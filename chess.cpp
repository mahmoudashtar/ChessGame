#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<math.h>
#include"square.h"
#include"chessPiece.h"
#include"player.h"



std:: vector<Square> myB(64); 
bool whiteIsChecked = false;
bool blackIsChecked = false;
std:: vector<Square> whiteMoves(0); //  squares controlled by White
std:: vector<Square> blackMoves(0); //    square controlled by Black
Square lastOldSq; 
Square lastNewSq;
Player Winner;
Player playerBlk;
Player playerWht;
bool whiteTurn = true;
bool quitBtn = false; 

// to implement aanpassan 
Square lastFromSq; 
Square lastToSq;
ChessPiece lastPiece; 
std:: vector<std:: vector<Square>> gameHis;
std:: vector<std:: vector<Square>> boardHis;
std:: vector<int> HR_X_MOVES = {1, 1, 2, 2, -1, -1, -2, -2};
std:: vector<int> HR_Y_MOVES = {2, -2, 1, -1, 2, -2, 1, -1};

//util func
void printSqVec(std::vector<Square> sq){
    for(int i = 0; i < sq.size();i++){
        sq.at(i).printSquareInf();
    }
}

void printMsg(std:: string msg){
    for(int i = 0; i < msg.size() + 4; i++){
        std:: cout << "-";
    }
    std:: cout<< "\n| " << msg << " |\n";
    for(int i = 0; i < msg.size() + 4; i++){
        std:: cout << "-";
    }
    std:: cout<< "\n\n";
}

void initBoard(){
    for(int j = 0; j < 8; j++){
        for(int i = 0; i < 8; i++){
            myB.at(i + (j * 8)).setState(true);
            myB.at(i + (j * 8)).setX(i);
            myB.at(i + (j * 8)).setY(j);

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

void drawCB(){
    std:: string boardStr = "  ";
    int size = sqrt(myB.size());
    for(int i = 0; i < size; i++){
        boardStr += "+-----";
    }
    boardStr += "+\n";
    for(int i = 0; i < size; i++){
        std:: string rowNum = std:: to_string(1 + i);
        boardStr += rowNum + " ";
        for(int j = 0; j < size; j++){
            if(myB.at(j + (i * 8)).isEmpty()){ boardStr += "|     "; }
            else{ 
                boardStr += drawPiece(myB.at(j + (i * 8)).piece.getType());
            }
        }
        boardStr += "|\n  ";
        for(int j = 0; j < size; j++){ // this might be deleted. maKING square too big makes drawing chess pieces harder
            if(myB.at(j + (i * 8)).isEmpty()){ boardStr += "|     "; }
            else{ 
                if(myB.at(j + (i * 8)).piece.getColor() == WHITE){
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

ChessPiece makePiece(pieceColor color, pieceType type){
    ChessPiece piece;
    piece.setColor(color);
    piece.setType(type);
    piece.flagged = false;
    return piece;
}

std:: vector<ChessPiece> makeSet(pieceColor color){
    std:: vector<ChessPiece> mySet;
    mySet.push_back(makePiece(color, KING));
    mySet.push_back(makePiece(color, QUEEN));
    for(int i = 0; i < 8; i++){
        mySet.push_back(makePiece(color, PAWN));
        if(i < 2){
            mySet.push_back(makePiece(color, ROOK));
            mySet.push_back(makePiece(color, HORSE));
            mySet.push_back(makePiece(color, BISHUP));
        }
    }
    return mySet;
}

void putPieces(){
    for(int i = 0; i < 16; i++){
        if(i == 0 || i ==7) {myB.at(i).piece = makePiece(WHITE, ROOK) ;}
        else if(i == 1 || i ==6) myB.at(i).piece = makePiece(WHITE, HORSE);
        else if(i == 2 || i ==5) myB.at(i).piece = makePiece(WHITE, BISHUP);
        else if(i == 3) myB.at(i).piece = makePiece(WHITE, QUEEN);
        else if(i == 4) myB.at(i).piece = makePiece(WHITE, KING);
        else myB.at(i).piece = makePiece(WHITE, PAWN);
        myB.at(i).setState(false);
    }
    for(int i = 48; i < 64; i++){
        if(i == 56 || i ==63) myB.at(i).piece = makePiece(BLACK, ROOK);
        else if(i == 57 || i ==62) myB.at(i).piece = makePiece(BLACK, HORSE);
        else if(i == 58 || i ==61) myB.at(i).piece = makePiece(BLACK, BISHUP);
        else if(i == 59) myB.at(i).piece = makePiece(BLACK, QUEEN);
        else if(i == 60) myB.at(i).piece = makePiece(BLACK, KING);
        else myB.at(i).piece = makePiece(BLACK, PAWN);
        myB.at(i).setState(false);
    }
}

int findSquare(int x, int y){
    for(int i = 0; i < myB.size(); i++){
        if(myB.at(i).getX() == x && myB.at(i).getY() == y){
            return i;
        }
    }
    return -1;
}

bool checkAnpasant(Square mySq){
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
    int sqInd = findSquare( myX, myY);
    int destination = 1;
    if(myPAWN.getColor() == BLACK){ destination = -1; }
   
    int firstInd = findSquare(myX, myY + (1 * destination) );
    int secondInd = findSquare(myX, myY + (2 * destination) );
    int thirdInd = findSquare(myX - 1, myY + (1 * destination) );
    int fourthInd = findSquare(myX + 1, myY + (1 * destination) );
    if(firstInd != -1 && myB.at(firstInd).isEmpty()){
        moves.push_back(myB.at(firstInd));
        if(secondInd != -1 && myB.at(secondInd).isEmpty() && ((myY == 1 && destination == 1) ||(myY == 6 && destination == -1) ) ){
            moves.push_back(myB.at(secondInd));
        }
    }
    if(thirdInd != -1 && !myB.at(thirdInd).isEmpty() && myB.at(thirdInd).piece.getColor() != myPAWN.getColor()){
        moves.push_back(myB.at(thirdInd));
    }
    if(fourthInd != -1 && !myB.at(fourthInd).isEmpty() && myB.at(fourthInd).piece.getColor() != myPAWN.getColor()){
        moves.push_back(myB.at(fourthInd));
    }
    // to implement aanpassant rule
    if(checkAnpasant(sq)){
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
            if(myB.at(ind).isEmpty()){
                moves.push_back(myB.at(ind));
            }
            else if(myB.at(ind).piece.getColor() == myROOK.getColor()){ 
                break; 
            } 
            else{
                moves.push_back(myB.at(ind)); 
                break; 
            }
        }
        for(int i = 1; i < 8; i ++){
            int ind = findSquare(myX + (i * dest), myY);
            if(ind == -1) break;
            if(myB.at(ind).isEmpty()){
                moves.push_back(myB.at(ind));
            }
            else if(myB.at(ind).piece.getColor() != myROOK.getColor()){ 
                moves.push_back(myB.at(ind));
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
    ChessPiece myBi = sq.piece;
    int myX = sq.getX();
    int myY = sq.getY();
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX + j , myY + j);
        if(ind == -1) break;
        else if(myB.at(ind).isEmpty()) {
            moves.push_back(myB.at(ind));
        }
        else if(myB.at(ind).piece.getColor() != myBi.getColor()){
            moves.push_back(myB.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX + j, myY - j);
        if(ind == -1) break;
        else if(myB.at(ind).isEmpty()) {
            moves.push_back(myB.at(ind));
        }
        else if(myB.at(ind).piece.getColor() != myBi.getColor()){
            moves.push_back(myB.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX - j, myY - j);
        if(ind == -1) break;
        else if(myB.at(ind).isEmpty()) {
            moves.push_back(myB.at(ind));
        }
        else if(myB.at(ind).piece.getColor() != myBi.getColor()){
            moves.push_back(myB.at(ind)); 
            break;
        }
        else{ 
            break;
        }
    }
    for(int j = 1; j < 8; j ++){
        int ind = findSquare(myX - j, myY + j);
        if(ind == -1) break;
        else if(myB.at(ind).isEmpty()) {
            moves.push_back(myB.at(ind));
        }
        else if(myB.at(ind).piece.getColor() != myBi.getColor()){
            moves.push_back(myB.at(ind)); 
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
    if(ind_1 != -1 && (myB.at(ind_1).isEmpty() || myB.at(ind_1).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_1)); }
    if(ind_2 != -1 && (myB.at(ind_2).isEmpty() || myB.at(ind_2).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_2)); }
    if(ind_3 != -1 && (myB.at(ind_3).isEmpty() || myB.at(ind_3).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_3)); }
    if(ind_4 != -1 && (myB.at(ind_4).isEmpty() || myB.at(ind_4).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_4)); }
    if(ind_5 != -1 && (myB.at(ind_5).isEmpty() || myB.at(ind_5).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_5)); }
    if(ind_6 != -1 && (myB.at(ind_6).isEmpty() || myB.at(ind_6).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_6)); }
    if(ind_7 != -1 && (myB.at(ind_7).isEmpty() || myB.at(ind_7).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_7)); }
    if(ind_8 != -1 && (myB.at(ind_8).isEmpty() || myB.at(ind_8).piece.getColor() != myKing.getColor()) ){ moves.push_back(myB.at(ind_8)); }
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
        int newSqInd = findSquare(newX, newY);
        if( newSqInd != -1 && (myB.at(newSqInd).isEmpty() || myB.at(newSqInd).piece.getColor() != myColor) ){
            moves.push_back(myB.at(newSqInd));
        }
    }
    return moves;
}

void move(Square oldSq, Square newSq){
    int oldInd = findSquare(oldSq.getX(), oldSq.getY());
    int newInd = findSquare(newSq.getX(), newSq.getY());
    if(myB.at(oldInd).piece.getType() == PAWN && myB.at(newInd).isEmpty() && (myB.at(oldInd).getX() != myB.at(newInd).getX())){
        int dest = -1; 
        if(myB.at(oldInd).piece.getColor() == BLACK){dest = 1;}
        myB.at(newInd + 8*dest).setState(true);
    }
    myB.at(newInd).piece = myB.at(oldInd).piece;
    myB.at(newInd).setState(false);
    myB.at(oldInd).setState(true);
    gameHis.push_back(myB);
}

void setLastMove(Square from, Square to){

    lastFromSq = from;
    lastToSq = to;
    lastPiece = myB.at(findSquare(to.getX(), to.getY())).piece;
}

void undoMove(){
    gameHis.pop_back();
    myB = gameHis.back();
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
    for(int i = 0; i < myB.size(); i ++){
        if(!myB.at(i).isEmpty() && myB.at(i).piece.getColor() == color){
            std:: vector<Square> pieceMoves = calcMoves(myB.at(i));
            for(int j = 0; j < pieceMoves.size(); j++){
                allMoves.push_back(pieceMoves.at(j));
            }
        }
    }
    
    return allMoves; 
}

bool isChecked(pieceColor color){// to is if a player is checked
    std:: vector<Square> allMoves = calcAllMoves(color);
    for(int i = 0; i < allMoves.size(); i++){
        int ind = findSquare(allMoves.at(i).getX(), allMoves.at(i).getY());
        if( !myB.at(ind).isEmpty() && 
            myB.at(ind).piece.getColor() != color && 
            myB.at(ind).piece.getType() == KING ){
            if(color == WHITE) blackIsChecked = true;
            else whiteIsChecked = true;
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
            move(curPiece, myB.at(ind));
            isChecked(BLACK);
            if(!whiteIsChecked){
                legalMoves.push_back(moves.at(i));
            }
            undoMove();
            isChecked(BLACK);            
        }
        else{
            move(curPiece, myB.at(ind));
            isChecked(WHITE);
            if(!blackIsChecked){
                legalMoves.push_back(moves.at(i));
            }
            undoMove();
            isChecked(WHITE);
        }
    }
    return legalMoves;
}

void showWinner(Player winner){
    printMsg( "\n*********\n*********\n" + winner.getName() + " WON\n*********\n*********");
}

bool gameOver(){
    std:: vector<Square> allWhiteMoves;
    std:: vector<Square> allBlackMoves;

    for(int i = 0; i < myB.size(); i++){
        Square sq = myB.at(i);
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

    if(isChecked(BLACK) && allWhiteMoves.size() == 0 ){
        showWinner(playerBlk);
        return true;

    } else if(isChecked(WHITE) && allBlackMoves.size() == 0){
        showWinner(playerWht);
        return true;
    }
    else {
        return false;
    }

}

void promptMove(){
    std:: string turn = "White To Move  ***  ";
    if( !whiteTurn){
        turn = "Black to Move ***";
    }
    printMsg( turn + "Choose a piece to move");
}

bool quitGame(char input){
    if(input == 'q') {
        char answer;
        printMsg( "ARE YOU SURE YOU WANT TO QUIT THE GAME? (type y to quit)");
        std:: cin >> answer;
        if(answer == 'y'){
            std:: cout << "\n********\nQUITING GAME\n********\n";
            return true;
        }
    }
    return false;
}

bool checkInbound(char &colLetter, int &col, int &row){
    if(row < 1 || row > 8 || colLetter < 'a' || colLetter > 'h'){
            std:: cout << "\nPlease choose a square on the board\n";
            return false;
        }
        row -= 1;
        col = colLetter - 97;
    return true;
}

bool unvalidSq(int sqInd){
    if(myB.at(sqInd).isEmpty()){
        printMsg("EMPTY SQUARE");
        return true;
    }
    if(whiteTurn){
        if(myB.at(sqInd).piece.getColor() != WHITE){
            printMsg("must choose a white piece");
            return true;
        }
    }
    else{
        if(myB.at(sqInd).piece.getColor() != BLACK){
            printMsg("must choose a black piece");
            return true;
        }
    }
    return false;
}

int main(){
    std:: string nameWhite , nameBlack;
    printMsg("Enter the names of Player A (white) and B (black)");
    std:: cin >> nameWhite >> nameBlack;
    playerWht.setName(nameWhite);
    playerWht.setColor(WHITE);
    playerBlk.setName(nameBlack);
    playerBlk.setColor(BLACK);
    std:: string myBoard; 
    initBoard();
    putPieces();
    playerWht.setChessSet(makeSet(WHITE));
    playerBlk.setChessSet(makeSet(BLACK));
    drawCB();
    gameHis.push_back(myB);
    std:: cout<< myBoard << std:: endl;
    char colLetter;
    int col;
    int row; 
    char quit;
    while(!gameOver()){
        promptMove();
        std:: cin >> colLetter >> row;
        if(quitGame(colLetter)) break;
        if(!checkInbound(colLetter, col, row)) continue;
        int sqInd = findSquare(col, row);
        if(unvalidSq(sqInd)) continue;
        
        std:: vector<Square> moves = calcMoves(myB.at(sqInd));
        moves = legalise(moves, myB.at(sqInd)); 
        if(moves.size() == 0){
            printMsg("THIS PIECE HAS NO MOVES, PLEASE CHOOSE A PIECE THAT HAS MOVES");
            continue;
        }
        std:: cout<< "Choose a move:\n";
        for(int i = 0; i < moves.size(); i++){
            char x = moves.at(i).getX() + 97;
            int y = moves.at(i).getY() + 1;
            std:: cout<< i << ": " <<  x  << y << std:: endl;
            if(i == moves.size() -1){
                std:: cout<< i+1 << ": change piece\n";
            }
        }
        int moveNum; 
        std:: cin>> moveNum; 
        if(moveNum == moves.size()){ //change chosen piece 
            continue;
        }
        printMsg( "MOVING PIECE");
        move(myB.at(sqInd), moves.at(moveNum));
        setLastMove(myB.at(sqInd), moves.at(moveNum));
        drawCB();
        if(whiteTurn && isChecked(WHITE)){
            blackIsChecked = true;
            printMsg( "*** BLACK CHECKED ***");
        }
        else if(!whiteTurn && isChecked(BLACK)){
            printMsg( "*** WHITE CHECKED ***");
        }

        (whiteTurn)? whiteTurn = false : whiteTurn = true;
    }
    return 0;
}


