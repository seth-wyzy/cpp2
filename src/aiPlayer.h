#ifndef AIPLAYER
#define AIPLAYER

#include "card.h"
#include <vector>


class AIPlayer {
public:
    //constructor
    AIPlayer(int pos);

    void startRound(std::vector<card> deltHand);
    card chooseMove(std::vector<card> currTrick, bool leader, int trump);
    std::vector<card> getMyHand() {return myHand;};
    void setMeld(int m) {meld = m;};
    

private:
    int position;
    int meld;
    bool team; // true if us, false if them
    std::vector<card> myHand;

};


#endif