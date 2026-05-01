#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include "utils.h"

/*			 Name: Areeba Riaz
 *   			 Roll no: i22-1244 
 *
 * PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * */

void movement(int delta_x) {
    for (int i = 0; i < 4; i++) {
        point_2[i] = point_1[i];
        point_1[i].x += delta_x;
    }
    if (!anamoly()) {
        for (int i = 0; i < 4; i++) {
            point_1[i] = point_2[i];
        }
    }
}

void rotation(bool rotate) {
    if (rotate) {
        for (int i = 0; i < 4; i++) {
            int a = point_1[i].y - point_1[1].y;
            int b = point_1[i].x - point_1[1].x;
            point_1[i].x = point_1[1].x - a;
            point_1[i].y = point_1[1].y + b;
        }
        if (!anamoly()) {
            for (int i = 0; i < 4; i++) {
                point_1[i] = point_2[i];
            }
        }
    }
}

void fallingPiece(float& timer, float& delay, int& colorNum) {
    if (timer > delay) {
        for (int i = 0; i < 4; i++) {
            point_2[i] = point_1[i];
            point_1[i].y += 1;
        }

        if (!anamoly()) {
            int n = rand() % 7;
            for (int i = 0; i < 4; i++) {
                gameGrid[point_2[i].y][point_2[i].x] = colorNum;
            }
            colorNum = 1 + rand() % 7;
            for (int i = 0; i < 4; i++) {
                point_1[i].x = BLOCKS[n][i] % 2;
                point_1[i].y = BLOCKS[n][i] / 2;
            }
        }
        timer = 0;
    }
}

int line(int& a, int& score, bool& gameover) {
    for (int i = a; i >= 1; --i) {
        int count = 0;
        for (int j = 0; j < N; ++j) {
            if (gameGrid[i][j]) {
                if (i == 1) gameover = true;
                ++count;
            }
            gameGrid[a][j] = gameGrid[i][j];
        }
        if (count < N) {
            --a;
        } else if (count == 10) {
            score += 10;
        }
    }
    return score;
}

#endif
