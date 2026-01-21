#include "aiPlayer.h"

AIPlayer::AIPlayer(int pos) {
    this->position = pos;
    if (pos == 0) 
        this->team = true; // us
    else
        this->team = false; // them
    hMeld = 0;
    dMeld = 0;
    cMeld = 0;
    sMeld = 0;
}
/*
* This should be used at the start of each round to initilize the hand, meld can be choosen
*/
void AIPlayer::startRound(std::vector<card> deltHand) { 
    this->myHand = deltHand;
    hMeld = 0;
    dMeld = 0;
    cMeld = 0;
    sMeld = 0;
}


/*
* Deal with 4 possible senarios:
* Playing first
* having a card of the same suit
* not having a card of the same suit, and having trump --> check if a trump has already been played
* not having a card of the same suit, and not having trump --> you have to know who's leading
*/
card AIPlayer::chooseMove(std::vector<card> currTrick, bool leader, int trump) {
    // senario 1
    if (currTrick.empty()) { 
        card toPlay = myHand[0];
        for (const auto& c: myHand) {
            if (c > toPlay)
                toPlay = c;
        }
        return toPlay;
    }
    //senario 2
    card leadCard = currTrick[0];
    std::vector<card> possibleOptions;
    for (const auto& c: myHand) { // TODO Optimize this to first check for the required suit, then only check what needs to be checked
        if (c.suit == leadCard.suit) 
            possibleOptions.push_back(c);
    }
    if (!possibleOptions.empty()) {
        if (leader == team) 
            return possibleOptions.at(0);
        else 
            return possibleOptions.back();
    } // if it is empty, then we have to play a trump suit
    for (const auto& c: myHand) {
        if (c.suit == trump) 
            possibleOptions.push_back(c);
    }
    if (!possibleOptions.empty()) 
        return possibleOptions.back();
     
    return myHand.back();
    
    // TODO optimize so that we actually know who's winning at a given point to make it better for like which card to play
    // ! like right now this assumes the leader will always win, but we know that that is not always the case

}

int AIPlayer::chooseBid(int currBid) { // use the highest meld for this
    int maxBid = biddingMeld+10;
    if (currBid < maxBid) 
        return currBid+1;
    return -1;
}

int AIPlayer::chooseTrump() {
    return personalTrump;
}

//hmeld
//dmeld
//smeld
//cmeld

void AIPlayer::choosePersonalTrump() {
    biddingMeld = hMeld;
    personalTrump = 0;
    if (dMeld > biddingMeld) {
        biddingMeld = dMeld;
        personalTrump = 1;
    }
    if (sMeld > biddingMeld) {
        biddingMeld = sMeld;
        personalTrump = 2;
    }
    if (cMeld > biddingMeld) {
        biddingMeld = cMeld;
        personalTrump = 3;
    }

}


