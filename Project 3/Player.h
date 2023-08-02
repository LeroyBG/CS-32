/* Leroy Betterton Gage
 Project 3
 5/30/2023
 Player.h */

/* Place your Player class definition here, as well as the definitions for the HumanPlayer, BadPlayer, and SmartPlayer classes. */

#ifndef PLAYER_H
#define PLAYER_H

#include "Side.h"
#include <string>
#include <vector>

class Board;
class JumpyTimer;

class Player{
public:
    Player(std::string name) : m_name(name) {}
    std::string name() const{
        return m_name;
    }
    virtual bool isInteractive() const{
        return false;
    }
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player(){}
    
private:
    std::string m_name;
};

class HumanPlayer : public Player{
    // helper functions?
public:
    HumanPlayer(std::string name) : Player(name){} // use player constructor to assign name
    virtual bool isInteractive() const{
        return true;
    }
    virtual int chooseMove(const Board& b, Side s) const; // TODO: implement
    virtual ~HumanPlayer(){} // TODO: implement
};

class BadPlayer : public Player{
public:
    BadPlayer(std::string name) : Player(name){} // use player constructor to assign name
    // don't need isInteractive
    int chooseMove(const Board& b, Side s) const;
    ~BadPlayer(){} // TODO: implement
    
};

class SmartPlayer : public Player{
    void display(Board m_board) const;
    int helpMechooseAmove(Board &currState, Side moverSide, Side playerSide, int depth, JumpyTimer &timer, double timeLimit) const;
    Side simulateMove(Board &simulate, Side startSide, int hole) const; // applies a given move to a board and returns the side whose turn it is
    bool tieState(const Board currState, Side s, std::vector<int> &holesWithBeans) const; // check if we're at a tie
    int evaluate(const Board &b, Side s) const; // evaluate the current board
public:
    SmartPlayer(std::string name) : Player(name){} // use player constructor to assign name
    // don't need isInteractive
    virtual int chooseMove(const Board& b, Side s) const; // TODO: implement (best move)
    virtual ~SmartPlayer(){} // TODO: implement
};

#endif
