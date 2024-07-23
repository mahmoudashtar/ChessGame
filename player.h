#ifndef NODEDB_PLAYER_H
#define NODEDB_PLAYER_H
#include<cstdlib>
#include<vector>
#include<string>
#include"ChessPiece.h"


class Player{
    public:
    void setName(std:: string val);
    void setChessSet(std:: vector<ChessPiece> val);
    void setColor(pieceColor val);
    pieceColor getColor();
    std:: string getName();
    std:: vector<ChessPiece> getSet();

    private:
    pieceColor color;
    std:: string name;
    std:: vector<ChessPiece> chessSet;

};

#endif
