#ifndef UTILS_H
#define UTILS_H

//---Title: PF-Project, Fall-2022 for BS(CS)---//
const char title[] = "PF-Project, Fall-2022";

//---Height and Width of the Actual Interactive Game---//
const int M = 20;   // Number of rows for a piece to cover on the screen = 20
const int N = 10;   // Number of columns for a piece to cover on the screen = 10

//---The Actual Interactive Game Grid - Built Over (MxN)---//
inline int grid[M][N] = {0};

//---Struct to hold coordinates---//
struct Point {
    int x, y;
};

//---To Hold the Coordinates of the Piece---//
// currentPiece is the piece controlled by the player, prevPiece is for collision checking, ghostPiece is the shadow
inline Point currentPiece[4], prevPiece[4], ghostPiece[4];

//---Check if the current position of the piece is valid (no collisions, within boundaries)---//
inline bool isValidPosition() {
    for (int i = 0; i < 4; i++) {
        if (currentPiece[i].x < 0 || currentPiece[i].x >= N || currentPiece[i].y >= M)
            return false;
        else if (grid[currentPiece[i].y][currentPiece[i].x])
            return false;
    }
    return true;
}

//---Calculate the position of the ghost (shadow) piece---//
inline void updateGhostPiece() {
    for (int i = 0; i < 4; i++) ghostPiece[i] = currentPiece[i];
    
    // Save current piece to restore later
    Point tempPiece[4];
    for (int i = 0; i < 4; i++) tempPiece[i] = currentPiece[i];

    // Move currentPiece (the copy) down until it hits something
    while (isValidPosition()) {
        for (int i = 0; i < 4; i++) {
            prevPiece[i] = currentPiece[i];
            currentPiece[i].y += 1;
        }
    }
    
    // The ghost piece is at the last valid position
    for (int i = 0; i < 4; i++) ghostPiece[i] = prevPiece[i];

    // Restore the actual current piece
    for (int i = 0; i < 4; i++) currentPiece[i] = tempPiece[i];
}

#endif
