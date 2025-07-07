#include "Pin.h"
#include <exception>
#include <iostream>
#include <ostream>
#include <pstl/glue_algorithm_defs.h>
#include <vector>
#include <random>
#include <algorithm>
#include <map>



// Constructor definition
Pin::Pin() {
    initialize_deck();
    shuffleDeck();
    choose_dealer();

    allHands[0] = &north;
    allHands[1] = &east;
    allHands[2] = &hand;
    allHands[3] = &west;

    meldHands[0] = &meld_north;
    meldHands[1] = &meld_east;
    meldHands[2] = &meld_hand;
    meldHands[3] = &meld_west;

    // initilize_game(); // also prints your hand 
    // bet = bidding();

    // choose_cards(*allHands[2], *meldHands[2]);
    // trumpSuit = 0;
    // hand.clear();
    // card JD  = {11, 1, 0};
    // card JD2 = {12, 1, 1};
    // card QS = {12, 3, 0};
    // card QS2 = {13, 3, 0};
    // hand.push_back(JD);
    // hand.push_back(JD2);
    // hand.push_back(QS);
    // hand.push_back(QS2);
    // std::cout << std::endl;
    // print_hand(hand);
    // sortHands();
    // count_meld(hand);
    // doTrickTaking();
    for (int i = dealer; i < dealer+4; i++) { // this one has no bidding trump is 1 (diamonds)
        int h = i % 4;
        std::cout << h << "\n";
        trumpSuit = 1;
        shuffleDeck();
        clear_hands();
        deal_hands();
        print_hand(hand);
        choose_cards(hand, meld_hand);
        count_meld(meld_hand);
        // TODO figure out how to use your trick taking functionos in the right order
        // ? trick counting logic is there, not sure if it is in the trick taking function

    }
}

// Member functions definitions
void Pin::initialize_deck() {
    for (int repeat = 0; repeat < 2; ++repeat) {
        for (int r = 9; r <= 15; r++) {
            if (r != 10) {
                for (int s = 0; s <= 3; s++) {
                    deck.push_back(card(r,s,repeat));
                }
            }
        }
    }
}

void Pin::shuffleDeck() {
    std::random_device rd;
    std::mt19937 g(rd()); 
    std::shuffle(deck.begin(), deck.end(), g);
}

void Pin::choose_dealer() {
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> dist(0, players - 1);
    dealer = dist(gen);
}

void Pin::print_deck() {
    for (const auto& card : deck) {
        card.print_card();
    }
}

void Pin::deal_hands() {
    for (int i = dealer; i < 48+dealer; i++) {
        allHands[i % 4]->push_back(deck[i - dealer]);
    }
}

void Pin::print_hand(std::vector<card> choice) {
    for (const auto& card: choice) {
        card.print_card();
    }
}

void Pin::count_cards() {
    std::cout << north.size() << " cards in north's hand" << std::endl;
    std::cout << east.size() << " cards in east's hand" << std::endl;
    std::cout << hand.size() << " cards in your hand" << std::endl;
    std::cout << west.size() << " cards in west's hand" << std::endl;    
}

void Pin::clear_hands(){
    north.clear();
    east.clear();
    west.clear();
    hand.clear();
}
void Pin::initilize_game() {
    clear_hands();
    deal_hands();
    print_hand(hand);
}
void Pin::meld() {
    bidding();
     
}
int Pin::bidding() {
    bool y = true;
    bool n = true;
    bool e = true;
    bool w = true;
    int n_bid = 0;  
    int e_bid = 0;    
    int y_bid = 0;
    int w_bid = 0;
    int winning_bid;
    int temp_bid;
    int start = dealer;
    int currHigh = 0;
    

    while (y || n || e || w) {
        switch (start) {
            case 0:
                if (n) {
                    std::cout << "North Bid: ";
                    std::cin >> n_bid;
                    std::cout << std::endl;
                    if (n_bid <= 0 || n_bid <= currHigh || n_bid < 21) {
                        n = false;
                    } else {
                        currHigh = n_bid;
                    }
                    
                }
            case 1:
                if (e) {
                    std::cout << "East Bid: ";
                    std::cin >> e_bid;
                    std::cout << std::endl;
                    if (e_bid <= 0 || e_bid <= currHigh || e_bid < 21) {
                        e = false;
                    } else {
                        currHigh = e_bid;
                    }
                }

            case 2:
                if (y) {
                    std::cout << "Your Bid: ";
                    std::cin >> y_bid;
                    std::cout << std::endl;
                    if (y_bid == 0 || y_bid <= currHigh || y_bid < 21) {
                        y = false;
                    }  else {
                        currHigh = y_bid;
                    }
                }
            case 3:
                if (w) {
                    std::cout << "West Bid: ";
                    std::cin >> w_bid;
                    std::cout << std::endl;
                    if (w_bid == 0 || w_bid <= currHigh || w_bid < 21) {
                        w = false;
                    } else {
                        currHigh = w_bid;
                    }  
                }
                start = 0;
        }
        

    }
    std::cout << "Clubs, Diamonds, Hearts, Spades (0,1,2,3)" << std::endl;
    std::cout << "Please input your trump suit: ";
    std::cin >> trumpSuit;
    std::cout << std::endl;
    return currHigh;
    

}

void Pin::choose_cards(std::vector<card>& hand, std::vector<card>& meld_hand){
    char ch;
    print_hand(hand);
    std::cout << "type y to include or n to not include" << std::endl;
    for (const auto& card : hand){
        card.print_card();
        std::cin >> ch;
        if (ch == 'y') {
            meld_hand.push_back(card);
        } 
    }
    print_hand(meld_hand);
}
void Pin::choose_all_cards(){
    for (int i = 0; i < 4; i++){
        std::cout << i << ":" << std::endl;
        choose_cards(*allHands[i],*meldHands[i]);
    }
}

int Pin::count_meld(std::vector<card>& hand){ // should only pass meld hand in for actual playing becuase we want that element
/*
    Pinochle 4
    Double 30
    Around:
        Ace 10
        King 8
        Queen 6
        Jack 4
    Marriage 2/4
    Run 15
    9_t 1
*/
    // rank of suit
    // Clubs, Dia, Hearts, Spades
    std::map<int, std::map<int, int>> suitRank;
    std::map<int, int> suits;
    std::map<int, int> ranks;

    for (const auto& card : hand) {
        suitRank[card.suit][card.rank]++;
        suits[card.suit]++;
        ranks[card.rank]++;
    }
    int meldPoints = 0;

    
    // Run in trump: A 10 K Q J (15 14 13 12 11)
    std::vector<int> run = {15, 14, 13, 12, 11};
    bool hasRun = true;
    for (int r : run) {
        if (suitRank[trumpSuit][r] < 1) {
            hasRun = false;
            break;
        }
    }
    if (hasRun) {
        std::cout << "Run in Trump: +15" << std::endl;
        meldPoints += 15;
    }
    // Marrige
    for (int s = 0; s <= 3; ++s) {
        if (suitRank[s][13] > 0 && suitRank[s][12] > 0) {
            int pts = (s == trumpSuit) ? 4 : 2;
            card che = {0, s,0};
            std::cout << "Marriage in " << che.p_suit() << ": +" << pts << std::endl;
            if (suitRank[s][13] > 1 && suitRank[s][12] > 1) pts *= 2;
            meldPoints += pts;
        }
    }
    std::map<int, int> aroundPoints = {
        {15, 10}, // Aces
        {13, 8},  // Kings
        {12, 6},  // Queens
        {11, 4}   // Jacks
    };
    for (auto [r, points] : aroundPoints) {
        bool hasAll = true;
        for (int s = 0; s < 4; ++s) {
            if (suitRank[s][r] == 0) {
                hasAll = false;
                break;
            }
        }
        card check = {r,0,0};
        if (hasAll) {
            std::cout << check.p_rank() <<"s around: +" << points << "\n";
            meldPoints += points;
        }
    }

        // Pinochle: Q♠ (12,3) + J♦ (11,1)
        int q_spades = suitRank[3][12];
        int j_diamonds = suitRank[1][11];
        int pinochles = std::min(q_spades, j_diamonds);
        if (pinochles >= 1) {
            int pts = (pinochles == 2) ? 30 : 4 * pinochles;
            std::cout << (pinochles == 2 ? "Double " : "") << "Pinochle: +" << pts << "\n";
            meldPoints += pts;
        }
    
        // 9 of trump
        if (suitRank[trumpSuit][9] > 0) {
            int p = suitRank[trumpSuit][9];
            std::cout << "9 of trump x" << p << std::endl;
            meldPoints += p;
        }
    
        std::cout << "Total Meld Points: " << meldPoints << std::endl;
    

        return meldPoints;
    
    
}
void Pin::all_count_meld(){
    for (int i = 0; i < 4; i++){
        count_meld(*allHands[i]);
    }
}

std::map<int, card> Pin::trick() { // this currently assumes no re nigs, but logic can be implemented 
    std::map<int, card> trick_hand;
    for(int i = tWinner; i < tWinner+4; i++) {
        int h = i% 4;
        print_person(h); // print whos turn it is
        print_hand(*allHands[h]);
        std::cout << "Please Select a Card (9-15) and suit (0-3 (CDHS))";
        int chosen_suit;
        int chosen_card;
        bool done = true;
        do {
            if (std::cin >> chosen_card && std::cin >> chosen_suit
            && chosen_card < 16 && chosen_card > 9  && chosen_card != 10 
            && chosen_suit >= 0 && chosen_suit < 4) done = false;
            else {
                std::cout << "ERROR: Invalid Format" << std::endl;
                std::cout << "Please Select a Card (9-14) and suit (0-3 (CDHS))";
         } }while (done);
        card temp(chosen_card, chosen_suit,0);
        auto& handVec = *allHands[h];   // reference to player's hand

        auto it = std::find(handVec.begin(), handVec.end(), temp);

        if (it == handVec.end()) {
            std::cout << "ERROR: that card isn't in your hand, try again" << std::endl;
            --i;               
            continue;
        }

        // 2. Add it to the trick
        trick_hand[h] = temp;

        // 3. Erase it from the hand
        handVec.erase(it);
    }

    return trick_hand;
    
}


int Pin::hand_winner() {
    trick_cards = trick();
    if (!checkTricks(trick_cards, tWinner)) return -1;
    std::pair<int, card> currWinner = {tWinner, trick_cards[tWinner]};
    for (int i = tWinner+1; i < tWinner+3; i++) {
        int h = i % 4;
        if ((trick_cards[h].suit == currWinner.second.suit && trick_cards[h].rank > currWinner.second.rank)
            || trick_cards[h].suit == trumpSuit && currWinner.second.suit != trumpSuit) {
            currWinner = {h, trick_cards[h]};
        }
    }
    return currWinner.first;
}

void Pin::takeTrick(){
    switch (hand_winner()) {
        case 0: case 2:
            for (const auto& it: trick_cards){
                usCards.push_back(it.second);
                lastTrick = true;
            }
            break;
        case 1: case 3:
            for (const auto& it: trick_cards) {
                themCards.push_back(it.second);
                lastTrick = false;
            }
            break;
    }
    trick_cards.clear();
}

void Pin::doTrickTaking() {
    for (const auto& it: *allHands[0]) {
        trick();
        takeTrick();
    }
}

void Pin::tPoints() {
    for (const auto& it: usCards) {
        if (it.rank >= 13) {
            usPoints++;
        }
    }
    for (const auto& it: themCards) {
        if (it.rank >= 13) {
            themPoints++;
        }
    }
    if (lastTrick) {
        usPoints++;
    } else {
        themPoints++;
    }
}


void Pin::print_person(int per) { // prints whos turn it is based on an integer [0-3]
    switch (per) {
        case 0:
            std::cout << "North\n";
            break;
        case 1:
            std::cout << "East\n";
            break;
        case 2:
            std::cout << "You\n";
            break;
        case 3:
            std::cout << "West\n";
            break;
        default:
            std::cout << "Invalid Person\n";
    }   
}

void Pin::sortHands() {
    // lambda to compare two cards: suit first, then rank
    auto cardLess = [](const card &a, const card &b) {
        if (a.suit != b.suit) 
            return a.suit < b.suit;
        return a.rank < b.rank;
    };

    // sort each hand in allHands[0-3]
    for (int i = 0; i < 4; ++i) {
        std::vector<card>* handPtr = allHands[i];
        std::sort(handPtr->begin(), handPtr->end(), cardLess);
    }
}

bool Pin::checkTricks(std::map<int, card> currTrick, int startPlayer) {
    if (currTrick[(startPlayer+1)%4].suit != currTrick[(startPlayer+0)%4].suit && currTrick[1].suit != trumpSuit) {
        for (const auto& card: *allHands[(startPlayer+1)%4]) {
            if (card.suit == trumpSuit) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+1)%4].rank <= currTrick[(startPlayer+0)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+1)%4]) {
            if (card.suit == currTrick[(startPlayer+0)%4].suit && card.rank > currTrick[(startPlayer+0)%4].rank) {
                return false;
            }
        }
    }
    // next person
    if (currTrick[(startPlayer+2)%4].suit != currTrick[(startPlayer+0)%4].suit && currTrick[(startPlayer+2)%4].suit != trumpSuit) {
        for (const auto& card: *allHands[(startPlayer+2)%4]) {
            if (card.suit == trumpSuit) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+2)%4].rank <= currTrick[(startPlayer+0)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+2)%4]) {
            if (card.suit == currTrick[(startPlayer+0)%4].suit && card.rank > currTrick[(startPlayer+0)%4].rank) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+2)%4].rank <= currTrick[(startPlayer+1)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+2)%4]) {
            if (card.suit == currTrick[(startPlayer+1)%4].suit && card.rank > currTrick[(startPlayer+1)%4].rank) {
                return false;
            }
        }
    }
    // final person
    if (currTrick[(startPlayer+3)%4].suit != currTrick[(startPlayer+0)%4].suit && currTrick[(startPlayer+3)%4].suit != trumpSuit) {
        for (const auto& card: *allHands[(startPlayer+3)%4]) {
            if (card.suit == trumpSuit) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+3)%4].rank <= currTrick[(startPlayer+0)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+3)%4]) {
            if (card.suit == currTrick[(startPlayer+0)%4].suit && card.rank > currTrick[(startPlayer+0)%4].rank) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+3)%4].rank <= currTrick[(startPlayer+1)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+3)%4]) {
            if (card.suit == currTrick[(startPlayer+1)%4].suit && card.rank > currTrick[(startPlayer+1)%4].rank) {
                return false;
            }
        }
    } else if (currTrick[(startPlayer+3)%4].rank <= currTrick[(startPlayer+2)%4].rank) {
        for (const auto& card: *allHands[(startPlayer+3)%4]) {
            if (card.suit == currTrick[(startPlayer+2)%4].suit && card.rank > currTrick[(startPlayer+2)%4].rank) {
                return false;
            }
        }
    }
    return true;
}