#ifndef NODEDB_GAME_H
#define NODEDB_GAME_H
#include"square.h"
#include"player.h"
#include<cstdlib>
#include<string>
#include<iostream>
#include<vector>



class Game{
    public: 
    void init_game();

    private: 
    Player player_white; 
    Player player_black;
    std:: vector<Square> CB; 

};

#endif