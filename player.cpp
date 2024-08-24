#include"player.h"



void  Player::  setName(std:: string val){
    name = val;
}
void  Player:: setChessSet(std:: vector<ChessPiece> val){
    chessSet = val;
}
void Player:: setColor(pieceColor val){
    color = val;
}
pieceColor Player:: getColor(){
    return color;
}
std:: string Player :: getName(){
    return name;
}
std::  vector<ChessPiece> Player:: getSet(){
    return chessSet;
}


