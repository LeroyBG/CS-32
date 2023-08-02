/* Leroy Betterton Gage
 Project 3
 5/30/2023
 Game.cpp */

// Place your Game member function implementations here.
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

using namespace std;

void Game::display() const{
    cout << "\tNorth" << endl;
    
    // NORTH BEANS
    cout << "   "; // horizontal tab before beans
    for (int i = 1; i <= m_board.holes(); i++) // beans
        cout << beans(NORTH, i) << " ";
    cout << endl;
    
    
    cout << m_board.beans(NORTH, 0) << "  ";
    for (int i = 0; i < m_board.holes(); i++)
        cout << "  ";
    cout << " " << m_board.beans(SOUTH, 0) << endl;
    
    // SOUTH BEANS
    cout << "   "; // horizontal tab
    for (int i = 1; i <= m_board.holes(); i++)
        cout << beans(SOUTH, i) << " ";
    cout << endl;
    
    cout << "\tSouth" << endl;
}

bool Game::move(Side s){ // not sure abt capture condition
    /* Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false. */
    
    
    Player* moveMe = m_north; // get pointers to players depending on whose turn it is
    Player* opposite = m_south;
    if (s == SOUTH){
        moveMe = m_south;
        opposite = m_north;
    }
    bool move_made = false;
    
    while(true){
        bool move_possible = (m_board.beansInPlay(s) > 0); // we can only move if there's a bean in play on our side
        if (!move_possible){ // no possible moves (can't move from pot/0)
            cout << moveMe->name() << " has no beans left to sow.\n";
            cout << "Sweeping remaining beans into " << opposite->name() << "\'s pot.\n";

            // sweep all of opponent's beans into their hole
            for (int i = 1; i <= m_board.holes(); i++){
                m_board.moveToPot(opponent(s), i, opponent(s)); // both sides should be empty after this
            }
            
            display();
            cerr << "there shouldn't be any beans in play on this board^\n";
            
            return false; // end the move
        }
        
        int move_hole = moveMe->chooseMove(m_board, s); // retrieve that side's player's move
        // print player decision
        if (moveMe->isInteractive() == false)
            cout << moveMe->name() << " chooses hole " << move_hole << endl;
        
        Side endSide; // side where move ends up
        int endHole; // hole where move ends up
        
        m_board.sow(s, move_hole, endSide, endHole); // sow once according the player's move
        move_made = true;
        display(); // display outcome of the move
        
        // check end condition
        if (endSide == s && endHole == 0){ // we landed in our pot
            cout << moveMe ->name() << " gets another turn\n";
            continue; // move again
        }
        else if((m_board.beans(endSide, endHole) == 1) && (endSide == s) && endHole != 0 && m_board.beans(opponent(s), endHole) > 0){ /* if the hole (not pot) we just ended on was empty a moment before (and on our side), we check if we can capture*/
            if (m_board.beans(opponent(s), endHole) > 0){ // if there's at least 1 bean in the opposite hole
                m_board.moveToPot(opponent(s), endHole, s); // capture those beans
                m_board.moveToPot(s, endHole, s); // move our beans to the hole too
                cout << "Capture!\n";
                display(); // display outcome of capture
                // maybe add some message notifying of a capture?

                break; // end the turn
            }
        }
        break; // end turn
        
    }
    return move_made;
}


//bool Game::move(Side s){ // not sure abt capture condition
//    /* Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false. */
//    bool move_possible = false;
//    for (int i = 1; i <= m_board.holes(); i++){ // make sure there at least one move to be made
//        if (m_board.beans(s, i) > 0){
//            move_possible = true;
//            break;
//        }
//    }
//
//    Player* moveMe = m_north; // get pointers to players depending on whose turn it is
//    Player* opposite = m_south;
//    if (s == SOUTH){
//        moveMe = m_south;
//        opposite = m_north;
//    }
//
//    if (!move_possible){ // no possible moves (can't move from pot/0)
//        cout << moveMe->name() << " has no beans left to sow.\n";
//        cout << "Sweeping remaining beans into " << opposite->name() << "\'s pot.\n";
//        display();
//        cerr << "there shouldn't be any beans in play on this board^\n";
//        // sweep all of opponent's beans into their hole
//        for (int i = 1; i <= m_board.holes(); i++){
//            m_board.moveToPot(opponent(s), i, opponent(s)); // both sides should be empty after this
//        }
//
//        return false; // end the move
//    }
//
//    int move_hole = moveMe->chooseMove(m_board, s); // retrieve that side's player's move
//    // print player decision
//    cout << moveMe->name() << " chooses hole " << move_hole << endl;
//
//
//    // ----- we know there's at least one move we can make
//    Side endSide; // side where move ends up
//    int endHole; // hole where move ends up
//
//    while (move_hole > 0){ // MAKE SURE TO UPDATE
//
//        m_board.sow(s, move_hole, endSide, endHole); // sow once according the player's move
//
//        display(); // display outcome of the move
//
//        // check end condition
//        if (endSide == s && endHole == 0){ // we landed in our hole
//            cout << moveMe ->name() << " gets another turn\n";
//            move_hole = moveMe->chooseMove(m_board, s); // re promp move
//            continue; // move again
//        }
//        else if((m_board.beans(endSide, endHole) == 1) && (endSide == s) && endHole != 0){ /* if the hole (not pot) we just ended on was empty a moment before (and on our side), we check if we can capture*/
//            if (m_board.beans(opponent(s), endHole) > 0){ // if there's at least 1 bean in the opposite hole
//                m_board.moveToPot(opponent(s), endHole, s); // capture those beans
//                m_board.moveToPot(s, endHole, s); // move our beans to the hole too
//                display(); // display outcome of capture
//                // maybe add some message notifying of a capture?
//                cout << "Capture!\n";
//                break; // end the turn
//            }
//        }
//        break; // end turn
//    }
//    return true;
//}

void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    /* If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side. */
    
    over = (m_board.beansInPlay(SOUTH) == 0 || m_board.beansInPlay(NORTH) == 0); // over is true if neither side can make a move

    hasWinner = true; // true unless there's a tie
    int southpot = m_board.beans(SOUTH, 0), northpot = m_board.beans(NORTH, 0); // retrieve num of beans in each pot

    if(southpot == northpot){
        hasWinner = false; // tie!
    }
    else if (southpot > northpot){ // south wins
        winner = SOUTH;
    }
    else{ // north wins
        winner = NORTH;
    }
}

void Game::play(){
    // if either player is interactive, we DONT need prompting
    bool needPrompt = !(m_south->isInteractive() || m_north->isInteractive());

    display(); // display the board to begin
    Side toMove = SOUTH;

    while(move(toMove)){ // while we can move

        toMove = opponent(toMove); // switch who moves

        if (needPrompt)
        promptEnter();
    }


    // ----- GAME OVER -----
    bool isOver, hasWinner; // will be manipulated by status function
    Side winner;
    status(isOver, hasWinner, winner);

    string endMessage;
    if (!hasWinner){
        endMessage += "TIE: no winner\n";
    }
    else if (winner == SOUTH){ // south wins
        endMessage += (m_south->name() + " wins!");
    }
    else{ // north wins
        endMessage += (m_north->name() + " wins!");
    }


    cout << endMessage << endl;
}

void Game::promptEnter() const{
    // function won't terminate until user presses enter

    string userInput;
    cout << "Press ENTER to continue";
    getline(cin, userInput);

}

