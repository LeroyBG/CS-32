/* Leroy Betterton Gage
 Project 3
 5/30/2023
 Player.cpp */

/* Place your Player member function implementations here, as well as those for HumanPlayer, BadPlayer, and SmartPlayer. */

#include "Player.h"
#include "Board.h"
#include "Board.h"

#include <iostream>
#include <cstdlib> // for random numbers
#include <ctime>
#include <chrono>

using namespace std;

// <--------------- TIMER --------------->
class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

// Advance jumpy timer only after jumpInterval calls to elapsed
class JumpyTimer
{
public:
  JumpyTimer(int jumpInterval)
   : m_jumpInterval(jumpInterval), m_callsMade(0)
  {
      actualElapsed();
  }
  double elapsed()
  {
      m_callsMade++;
      if (m_callsMade == m_jumpInterval)
      {
          m_lastElapsed = m_timer.elapsed();
          m_callsMade = 0;
      }
      return m_lastElapsed;
  }
  double actualElapsed()
  {
      m_lastElapsed = m_timer.elapsed();
      return m_lastElapsed;
  }
private:
  Timer m_timer;
  int m_jumpInterval;
  int m_callsMade;
  int m_lastElapsed;
};


// <-------- Player Base Class implementation -------->

// Human Player implementation

// FIX!
int HumanPlayer::chooseMove(const Board &b, Side s) const{
    // not actually moving, just returning a number corresponding to what you want to move to
    
    // find number of valid moves
    int validMoves = 0, index = 1, end_index = (b.holes()+1); // count 1 through [number of holes], stop before accessing the one after last
    
    while(index != end_index){
        if (b.beans(s, index) > 0)
            validMoves++;
        index++;
    }
    
    if (validMoves == 0){ // if we can't make any moves
        return -1;
    }
    
    // move can only be a hole within bounds that has beans
    int playerchoice = 0;
    while (true){ // loop only stops when we break
        cout << "Select a hole, " << name() << ": ";
        cin >> playerchoice;
        cin.ignore(10000, '\n');
        cin.clear();
        
        // check parameters (player can't choose 0 aka their pot)
        if (playerchoice < 1 || playerchoice > b.holes()){
            cout << "The hole number must be from 1 to " << b.holes() << ".\n";
            continue;
        }
        else if (b.beans(s, playerchoice) <= 0){
            cout << "There are no beans in that hole.\n";
            continue;
        }
        break;
    }
    
    return playerchoice;
}

// <-------- Bad Player implementation -------->
int BadPlayer::chooseMove(const Board &b, Side s) const{ // praying to god this works
    srand(unsigned(time(NULL))); // 'unsigned' is to silence warning about time_t conversion
    
    // find number of valid moves
    int validMoves = 0, index = 1, end_index = (b.holes()+1); // count 1 through [number of holes], stop before accessing the one after last
    
    while(index != end_index){
        if (b.beans(s, index) > 0)
            validMoves++;
        index++;
    }
    
    if (validMoves == 0){ // if we can't make any moves
        return -1;
    }
    
    // randomly choose a move
    int move_number = rand()%validMoves + 1; // returns a number from 1 to the number of valid moves
    
    index = 0; // reset index
    
    while (move_number > 0){
        index++; // index starts at 1
        if (b.beans(s, index) > 0)
            move_number--;
    }
    return index;
}






// <-------- Smart Player implementation -------->
int SmartPlayer::chooseMove(const Board &b, Side s) const{
    // Initialize a timer
    double timeLimit = 4990; // 4.99 seconds; allow 10 ms for wrapping up
    JumpyTimer timer(1000);
    
    
    
    
    
    // first check for a tie (no possible moves)
    vector<int> holesWithBeans;
    if (tieState(b, s, holesWithBeans)){
        return -1;
    }
    if (holesWithBeans.size() == 1){ // if we can only make one move
        return holesWithBeans[0]; // return that move
    }

    // holesWithBeans contains indexes of all valid moves
    Board first_copy = b;
    int max_value; // the maximum value we can get from a move
    int max_move_hole = holesWithBeans[0]; // the first / only move we can make
    Side turn_post_move = simulateMove(first_copy, s, holesWithBeans[0]); // store whether or not this move ends our turn
    
    /* timeLimit -= timer.elapsed(); -- subtract the amount of time it took us to get here (removed because uneccesary right now) */
    
    // allocate a fraction of the time to the first call
    double thisbranch_timeLimit = timeLimit / holesWithBeans.size(); // would be 0.833 seconds if limited to 5 seconds and playing with 6 holes
    
    // get a timer for before the recursive call
    double startTime = timer.elapsed();
    
    max_value = helpMechooseAmove(first_copy, turn_post_move, s, 1, timer, thisbranch_timeLimit); // we call helpMe but tell it whose turn it is, so it knows whether to select the highest or lowest value

    if (max_value >= 1000){
        return holesWithBeans[0];
    }
    
    // subtract time this branch took
    timeLimit -= (timer.elapsed() - startTime);
    
    for (int i = 1; i < holesWithBeans.size(); i++){ // start at second element
        thisbranch_timeLimit = (timeLimit/ (holesWithBeans.size() - i));
        // reset starttime
        startTime = timer.elapsed();
        
        
        timeLimit -= timer.elapsed();
        Board other_copies = b; // make a copy of the board
        // then manipulate this copy according to the specified move
        
        turn_post_move = simulateMove(other_copies, s, holesWithBeans[i]);
        int move_eval_holder = helpMechooseAmove(other_copies, turn_post_move, s, 1, timer, thisbranch_timeLimit);        // evaluation of making the move indicated by holesWithBeans[i]

        timeLimit -= (timer.elapsed() - startTime); // adjust time limit
        if (timeLimit <= 0)
            timeLimit = 0;
        if (move_eval_holder >= 1000){
            return holesWithBeans[i];
        }
        if (move_eval_holder > max_value){
            max_value = move_eval_holder;
            max_move_hole = holesWithBeans[i];
        }
    }
    cerr << "It took me " << timer.actualElapsed() << " ms to come up with this" << endl;
    return max_move_hole; // this will return the outcome that results in the most beans in my hole
}

int SmartPlayer::helpMechooseAmove(Board &currState, Side moverSide, Side playerSide, int depth, JumpyTimer &timer, double timeLimit) const{

    
    // edge case: we CANNOT make a move
    vector<int> holesWithBeans;
    if ( tieState(currState, moverSide, holesWithBeans) ){ // if it's a tie
        return 0; // we can go no further, this board is evaluated as 0
    }
    
    // holesWithBeans now contains all possible moves from here
    int current_evaluation = evaluate(currState, playerSide); // evaluation of current board
    if (timeLimit <= 0){
        return current_evaluation;
    }
    if (abs(current_evaluation) >= 1000){ // if we win, lose, or tie
        return current_evaluation;
    }
    if (timeLimit <= 0) // ran out of time
        return current_evaluation;
    
    // we know we can make at least 1 move
    
    
    // we want to return the maximum move we can make out of our availible moves
    Board first_copy = currState;
    
    // possible inputs from here
    // it's still our turn (1 or moves we can / have to make with this board)
    // it's no longer our turn, we have to consider the opponen't point of view
    // we still evaluate the board from our point of view, we just choose the one with the lowest value
    
    bool lookingformax = (moverSide == playerSide); /* if we're considering this from the person who's turn it is' point of view, we look for the max evaluated move, and vice versa */
    
    // make the first move we can make
    int polar_move_value = 0; // will store either the min or max value, depending on whether or not this move is our move or our opponent's
    // changed palyerside to moverside on next line
    Side move_outcome =  simulateMove(first_copy, moverSide, holesWithBeans[0]); // result of simulating this move may or may not end our turn
    //   we move to south's turn
    
    // calculate this branche's time limit
    double thisbranch_timelimit = (timeLimit / holesWithBeans.size());
    // store start time
    double startTime = timer.elapsed();
    
    // explore this branch
    polar_move_value = helpMechooseAmove(first_copy, move_outcome, playerSide, depth + 1, timer, thisbranch_timelimit);
    
    // adjust the time limit
    timeLimit -= (timer.elapsed() - startTime);
    if (timeLimit <= 0)
        timeLimit = 0;
    
    for (int i = 1; i < holesWithBeans.size(); i++){  // start with second element
        Board other_copies = currState;
        move_outcome = simulateMove(other_copies, moverSide, holesWithBeans[i]); // whose turn this move ends in to check if this move results in the end of the turn
        
        // calculate this branch's time limit
        thisbranch_timelimit = (timeLimit / holesWithBeans.size() - i);
        // reset start time
        startTime = timer.elapsed();
        
        // explore this branch
        int curr_move_value = helpMechooseAmove(other_copies, move_outcome, playerSide, depth + 1, timer, timeLimit); // store the max value of the series of moves resulting from sowing from the hole indicated by holesWithBeans[i]
        timeLimit -= (timer.elapsed() - startTime);
        if (timeLimit <= 0)
            timeLimit = 0;
        
        // retrieve the min or max of these values depending on what we're looking for
        
        // WRONG - need it to  find the index or something im not sure wait maybe im tryinng
        polar_move_value = (lookingformax) ? (max(curr_move_value, polar_move_value)) : (min(curr_move_value, polar_move_value));
        
    }
    return polar_move_value;
}


int SmartPlayer::evaluate(const Board &b, Side s) const{
    // analyze the board and evaluate it based on how advantageous it is
    
    // calculate total beans in play and make sure it's consistent with how many should be there
    int totalbeans = b.totalBeans();

    
    // calculate number of beans needed to win
    int winBeans = (totalbeans / 2) + 1; // shouldn't have to worry abt integer division, as total beans are always even
    
    
    
    // - - - - - GAME OVER - - - - -
    if(b.beans(s, 0) >= winBeans) // if we've already won
        return 1000;
    else if (b.beans(opponent(s), 0) >= winBeans) // if opponent has already won
        return -1000;
    
    // are we unable to move?
    vector<int> holesWithBeans;
    vector<int> holeWithBeansOpp;
    tieState(b, opponent(s), holeWithBeansOpp);
    if (tieState(b, s, holesWithBeans)){ // if we've tied
        return 0;
    }
    if (holesWithBeans.size() == 0 || holeWithBeansOpp.size() == 0){ // either side can't make any moves
        // simulate a sweep
        int opponentBeans = b.beansInPlay(opponent(s)) + b.beans(opponent(s), 0);
        int ourBeans = b.beansInPlay(s) + b.beans(s, 0);
        // if their swept side is more than ours
        if (opponentBeans > ourBeans){ // we lose
            return -1000;
        }
        if (opponentBeans < ourBeans){// we win!
            return 1000;
        }
    }
    
    // GAME ISNT OVER: return the difference between my pot beans and my opponent's
    return b.beans(s, 0) - b.beans(opponent(s), 0);
}


Side SmartPlayer::simulateMove(Board &simulate, Side startSide, int hole) const{
    // simulate given move by given side (manipulates the board)
    
    // we presuppose that this hole is a number from 1-6, this function always be called with a valid move
    if (hole < 1 || hole > simulate.holes()){
        cerr << "**!SIMULATE CALLED WITH INVALID MOVE!**\n";
        return SOUTH;
    }
    if (simulate.beans(startSide, hole) <= 0) // hole must have beans
        cerr << "**!SIMULATE CALLED WITH INVALID MOVE (no beans in this hole)**\n";
    // first things first, sow
    Side endside;
    int endHole;
    
    simulate.sow(startSide, hole, endside, endHole);
        
    
    // then we check the state of the board
    
    /* ENDED ON POT
    if turn ended in our pot, move ends but turn doesn't (player stays the same) */
    if(endHole == 0 && endside == startSide){
        return startSide; // we end this move on the same side that we started
    }
    /* CAPTURE
    if turn ended in one of our holes (not pot) that was previously empty (now has 1 bean) */
    if (endHole != 0 && endside == startSide && simulate.beans(endside, endHole) == 1 && simulate.beans(opponent(endside), endHole) > 0){
        simulate.moveToPot(opponent(startSide), endHole, startSide); // capture the beans in the opposite hole
        simulate.moveToPot(startSide, endHole, startSide); // move the bean we just placed too
    }
    return opponent(startSide); // turn over
}

bool SmartPlayer::tieState(const Board currState, Side s, vector<int> &holesWithBeans) const{
    
    for (int i = 1; i <= currState.holes(); i++)
        if (currState.beans(s, i) > 0)
            holesWithBeans.push_back(i);
    
    if (holesWithBeans.size() == 0){ // there isn't a single move we can make
        // sweep all of opponent's beans into their hole
        int myTotalbeans = 0, opponentTotalbeans = 0;
        
        myTotalbeans += currState.beans(s, 0); /* my beans are just the beans in my pot, the rest of my holes are empty */
        
        opponentTotalbeans += currState.beans(opponent(s), 0); // opponent pot
        opponentTotalbeans += currState.beansInPlay(opponent(s)); // rest of opponentbeans
        
        if ((opponentTotalbeans + myTotalbeans) != currState.totalBeans()) // (should never be true, as
            cerr << "Error: Beans don't add up! SmartPlayer::evaluate\n";
        if (opponentTotalbeans == myTotalbeans)
            return true; // tie!
    }
    return false; // not a tie, win, or loss
}
