#ifndef NODEDB_CHESSPIECE_H
#define NODEDB_CHESSPIECE_H
#include<vector>
#include<cstdlib>

enum pieceType { PAWN, ROOK, BISHUP, HORSE, QUEEN, KING}; 

enum pieceColor { WHITE, BLACK};

class ChessPiece {
    public: 
    bool flagged; // for annpassant rule for pawns

    void setType( pieceType type);
    void setColor(pieceColor color);
    void move(int xPos, int yPos);
    pieceType getType();
    pieceColor getColor();
    void pieceInfo();

    private: 
    pieceType type; 
    pieceColor color; 
};

#endif

