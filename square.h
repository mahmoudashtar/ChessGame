#ifndef NODEDB_SQUARE_H
#define NODEDB_SQUARE_H
#include<cstdlib> 
#include<iostream>
#include "chessPiece.h"


class Square{
    public: 
    ChessPiece piece;

    void setX(int val);
    void setY(int val);
    void setState(bool val);
    
    int getX();
    int getY();
    bool isEmpty();
    void printSquareInf();

    private: 
    int x; 
    int y; 
    bool empty; 

};


#endif

