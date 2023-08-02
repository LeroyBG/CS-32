/* Leroy Betterton Gage
 Project 3
 5/30/2023
 Side.h */

#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s){
    return Side(NSIDES - 1 - s);
}

/* This may contain additional types, constants, and non-member function declarations that you find useful to add. */

#endif
