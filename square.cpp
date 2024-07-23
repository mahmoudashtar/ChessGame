#include<cstdlib> 
#include<iostream>
#include "square.h"
// #include"chessPiece.h"


void Square :: setX(int val){
    x = val; 
}
void Square ::  setY(int val){
    y = val;
}
void Square ::  setState(bool val){
    empty = val;
}

int  Square :: getX(){ return x; }
int  Square :: getY(){ return y; }
bool Square ::  isEmpty(){return empty;}



// util func
void Square :: printSquareInf(){
    std:: cout<< "square info: x: " << x << " y: " << y << " empty: " << empty << std:: endl;
    if(!empty){
        std:: cout<< "type: " << piece.getType();
    }
}


