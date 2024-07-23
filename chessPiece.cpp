#include<cstdlib>
#include<iostream>
#include<vector>
#include"chessPiece.h"

void ChessPiece :: setType( pieceType type){this->type = type;}
void ChessPiece :: setColor(pieceColor color){ this->color = color;}
pieceType ChessPiece :: getType(){ return type;}
pieceColor ChessPiece :: getColor(){ return color;}



void ChessPiece :: pieceInfo(){
    std:: cout<< "pieceInfo:\n";
    (color == WHITE)? std:: cout<< "color: WHITE, " : std:: cout<< "color: black, ";
    std:: cout<< "type: ";
    switch (type)
    {
    case KING: std:: cout<< "king\n";
        break;
    case QUEEN: std:: cout<< "queen\n";
        break;
    case ROOK: std:: cout<< "rook\n";
        break;
    case HORSE: std:: cout<< "horse\n";
        break;
    case BISHUP: std:: cout<< "bishup\n";
        break;
    case PAWN: std:: cout<< "pawn\n";
        break;
    default: std:: cout<< "ERROR: type undefined";
        break;
    }
}


