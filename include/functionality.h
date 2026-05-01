#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include "utils.h"

// Movement of the piece (left/right)
void handleMovement(int delta_x);

// Rotation of the piece
void handleRotation(bool rotate);

// Falling logic
bool handleFalling(float& timer, float& delay, int& colorNum, int& nextColorNum, int& nextPieceType);

// Instant drop
void handleHardDrop();

// Line clearing and scoring
int handleLineClearing(int& score, bool& gameover);

#endif
