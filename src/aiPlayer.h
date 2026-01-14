#ifndef AIPLAYER
#define AIPLAYER

#include "card.h"
#include <vector>

class aiPlayer {
public:
    //constructor
    aiPlayer(int pos);

    void startRound(std::vector<card> deltHand);
    card chooseMove(std::vector<card> currTrick);

private:
    int position;
    int team;
    std::vector<card> myHand;

};


#endif