/* Leroy Betterton Gage
 Project 3
 5/30/2023
 Board.cpp */

#include "Board.h"
#include <cassert>
#include <iostream>
using namespace std;
// Place your Board member function implementations here.

Board::Board(int nHoles, int nInitialBeansPerHole){
    // TODO: IMPLEMENT
    if (nHoles < 1){ // if nHoles not positive
        nHoles = 1;
        m_holes = 2; // add one for the pot
    }
    if (nInitialBeansPerHole < 0){ // if initial beans is negative
        nInitialBeansPerHole = 0;
    }
    
    m_holes = nHoles + 1; // add one for pot
    int total_holes = 2 * m_holes;
    m_vec.assign(total_holes, nInitialBeansPerHole);
    
    m_southpot_index = nHoles; // item after end of south board
    m_northpot_index = 2 * nHoles + 1; // item after end of north board;
    
    m_vec[m_southpot_index] = 0;
    m_vec[m_northpot_index] = 0;
    
}
/* Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0. */



int Board::holes() const{
    
    return m_holes - 1; // subtract to account for pot
}
/* Return the number of holes on a side (not counting the pot). */



int Board::beans(Side s, int hole) const{
    // TODO: IMPLEMENT
    
    // if we're out of bounds
    int min = 0, max = holes(); // 0 (pot) through number of holes
    
    if (hole < min || hole > max){ // invalid hole
        return -1;
    }
    
    // convert hole to index
    return m_vec[holeToIndex(s, hole)];
}
/* Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid. */



int Board::beansInPlay(Side s) const{ // returns negative 1 if invalid side
    // TODO: IMPLEMENT
    int total = 0;
    
    int start_index = 0, end_index = m_southpot_index;
    
    if (s == NORTH){
        start_index = m_southpot_index + 1;
        end_index = m_northpot_index;
    }
    for (int i = start_index; i < end_index; i++){
        if (i == m_southpot_index || i == m_northpot_index){
            continue;
        }
        total += m_vec[i];
    }
    return total;
}
/* Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot. */



int Board::totalBeans() const{
    int total = 0;
    for (int i = 0; i < m_vec.size(); i++)
        total += m_vec[i];
    
    return total;
}
/* Return the total number of beans in the game, including any in the pots. */



bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    
    // check parameters
    int hole_index = holeToIndex(s, hole);
    if (hole_index < 0 || hole_index >= m_vec.size()){ // if we're accessing out of bounds
        return false;
    }
    if (hole_index == m_southpot_index || hole_index == m_northpot_index){ // accessing a pot
        return false;
    }
    
    int skipMe = m_northpot_index; // south skips north's pot
    if (s == NORTH){ // north skips south's pot
        skipMe = m_southpot_index;
    }
    
    // actual sowing
    int sowMe = beans(s, hole); // retrieve number of beans to sow
    m_vec[holeToIndex(s, hole)] = 0; // empty the hole we picked up from
    while (sowMe > 0){ // we still have beans to sow
        hole_index++; // start sowing at hole after the one we pick up
        if (hole_index == m_vec.size())
            hole_index = 0;
        if (hole_index == skipMe){ // if we've reached opponent's pot, don't sow
            continue;
        }
        
        m_vec[hole_index]++;
        sowMe--;
    }
    
    if (hole_index > m_southpot_index) // if we are past the south side
        endSide = NORTH;
    else
        endSide = SOUTH;
    
    endHole = indexToHole(hole_index, endSide);
    return true;
}
/* If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.) */



bool Board::moveToPot(Side s, int hole, Side potOwner){
    /* If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true. */
    
    // check parameters
    int hole_index = holeToIndex(s, hole);
    if (hole_index < 0 || hole_index >= m_vec.size()){ // if we're accessing out of bounds
        return false;
    }
    if (hole_index == m_southpot_index || hole_index == m_northpot_index){ // accessing a pot
        return false;
    }
    
    // set the pot that we move to
    int pot = m_southpot_index;
    if (potOwner == NORTH)
        pot = m_northpot_index;
    
    m_vec[pot] += m_vec[hole_index]; // add the beans in the hole to our pot
    m_vec[hole_index] = 0; // empty that hole
    
    return true;
}

bool Board::setBeans(Side s, int hole, int beans){
    /* If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.) */
    
    // if invalid hole or beans
    if (hole < 0 || hole >= m_holes || beans < 0){
        return false;
    }
    int index = holeToIndex(s, hole);
    if (index < 0 || index >= m_vec.size())
        return false;
    
    m_vec[index] = beans;
    return true; // delete
}




int Board::holeToIndex(Side s, int hole) const{
    /* from USER point of view: north's pot is its zero pot, south's is the one after its last */
    
    
    // check parametrs
    int lowestHole = 0, highestHole = m_holes; // same for both north and south: 0 hole is hole

    if (hole < lowestHole || hole > highestHole)
        return -1;
    
    if (hole == 0){ // special case for someone looking to access the pot
        if (s == SOUTH)
            return m_southpot_index;
        else // NORTH pot
            return m_northpot_index;
    }
    
    // south
    if (s == SOUTH)
        return hole - 1; // account for zero-indexing
    else {
        int last_index = int(m_vec.size() - 1); // get index of last element
        // that weird syntax is to silence the size_t warning
        hole = last_index - hole;
        /* on the north side, 'hole' corresponds to the distance between a hole's array index and the end of the array */
    }
    return hole;
}


void Board::test(){
    assert(holeToIndex(SOUTH, 1) == 0); // 1st hole
    assert(holeToIndex(SOUTH, 2) == 1);
    assert(holeToIndex(SOUTH, 3) == 2);
    assert(holeToIndex(SOUTH, 4) == 3);
    assert(holeToIndex(SOUTH, 0) == 4); // pot
    
    assert(holeToIndex(NORTH, 4) == 5);
    assert(holeToIndex(NORTH, 3) == 6);
    assert(holeToIndex(NORTH, 2) == 7);
    assert(holeToIndex(NORTH, 1) == 8);
    assert(holeToIndex(NORTH, 0) == 9);

    Side test = NORTH;
    
    assert(indexToHole(0, test) == 1 && test == SOUTH);
    assert(indexToHole(1, test) == 2 && test == SOUTH);
    assert(indexToHole(2, test) == 3 && test == SOUTH);
    assert(indexToHole(3, test) == 4 && test == SOUTH);
    assert(indexToHole(4, test) == 0 && test == SOUTH);

    assert(indexToHole(5, test) == 4 && test == NORTH);
    assert(indexToHole(6, test) == 3 && test == NORTH);
    assert(indexToHole(7, test) == 2 && test == NORTH);
    assert(indexToHole(8, test) == 1 && test == NORTH);
    assert(indexToHole(9, test) == 0 && test == NORTH);
    cerr << "| ";
    for (int i = 0; i < m_vec.size(); i++)
        cerr << m_vec[i] << " | ";
    cerr << endl;
    
    cerr << "South pot: " << m_vec[m_southpot_index] << endl;
    cerr << "North pot: " << m_vec[m_northpot_index] << endl;
}
//                 SOUTH        P      NORTH        P

// board value: 9 | 9 | 9 | 9 | 0 | 9 | 9 | 9 | 9 | 0

// board index: 1 | 2 | 3 | 4 | 0 | 4 | 3 | 2 | 1 | 0
// array index  0   1   2   3   4   5   6   7   8   9


int Board::indexToHole(int index, Side &s) const{
    // check parameters
    if (index < 0 || index >= m_vec.size()){
        return -1;
    }
    
    s = SOUTH;
    if (index == m_southpot_index)
        return 0;
    else if (index > m_southpot_index){ // north of south pot
        s = NORTH;
        // TODO: convert index to hole
    }
    else{ // south index conversion
        return (index + 1);
    }
    
    // north index conversion
    return (int(m_vec.size() - index) -1); /* subtract 1 for 0-indexing, 'int()' silences compiler warning */
    
}
