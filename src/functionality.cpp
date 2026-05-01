#include "functionality.h"
#include "pieces.h"
#include <cstdlib>

void handleMovement(int delta_x) {
    for (int i = 0; i < 4; i++) {
        prevPiece[i] = currentPiece[i];
        currentPiece[i].x += delta_x;
    }
    if (!isValidPosition()) {
        for (int i = 0; i < 4; i++) {
            currentPiece[i] = prevPiece[i];
        }
    }
}

void handleRotation(bool rotate) {
    if (rotate) {
        for (int i = 0; i < 4; i++) {
            int a = currentPiece[i].y - currentPiece[1].y;
            int b = currentPiece[i].x - currentPiece[1].x;
            currentPiece[i].x = currentPiece[1].x - a;
            currentPiece[i].y = currentPiece[1].y + b;
        }
        if (!isValidPosition()) {
            for (int i = 0; i < 4; i++) {
                currentPiece[i] = prevPiece[i];
            }
        }
    }
}

void handleHardDrop() {
    while (isValidPosition()) {
        for (int i = 0; i < 4; i++) {
            prevPiece[i] = currentPiece[i];
            currentPiece[i].y += 1;
        }
    }
    // Move back up one step because the last step was invalid
    for (int i = 0; i < 4; i++) {
        currentPiece[i] = prevPiece[i];
    }
}

bool handleFalling(float& timer, float& delay, int& colorNum, int& nextColorNum, int& nextPieceType) {
    if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            prevPiece[i] = currentPiece[i];
            currentPiece[i].y += 1;
        }

        if (!isValidPosition()) {
            // Place the piece on the grid
            for (int i = 0; i < 4; i++) {
                grid[prevPiece[i].y][prevPiece[i].x] = colorNum;
            }

            // The current piece takes the shape and color of the "Next" piece
            colorNum = nextColorNum;
            int n = nextPieceType;

            // Generate a NEW "Next" piece for the future
            nextColorNum = 1 + rand() % 7;
            nextPieceType = rand() % 7;

            // Spawn the piece (which was previously the "Next" piece)
            for (int i = 0; i < 4; i++) {
                currentPiece[i].x = BLOCKS[n][i] % 2;
                currentPiece[i].y = BLOCKS[n][i] / 2;
            }
            timer = 0;
            return true; // Piece landed!
        }
        timer = 0;
    }
    return false;
}

int handleLineClearing(int& score, bool& gameover) {
    int linesClearedThisTurn = 0;
    int rowToFill = M - 1;

    for (int i = M - 1; i >= 1; i--) {
        int filledCells = 0;
        for (int j = 0; j < N; j++) {
            if (grid[i][j]) {
                if (i == 1) gameover = true;
                filledCells++;
            }
            grid[rowToFill][j] = grid[i][j];
        }
        if (filledCells < N) {
            rowToFill--;
        } else {
            linesClearedThisTurn++;
        }
    }

    // Award bonus points based on how many lines were cleared in one go
    if (linesClearedThisTurn == 1) score += 10;
    else if (linesClearedThisTurn == 2) score += 30; // 10 bonus
    else if (linesClearedThisTurn == 3) score += 60; // 30 bonus
    else if (linesClearedThisTurn == 4) score += 100; // 60 bonus

    return score;
}
