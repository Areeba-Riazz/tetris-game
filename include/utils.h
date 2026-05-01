#ifndef UTILS_H
#define UTILS_H

/*			 Name: Areeba Riaz
 *			 Roll no: i22-1244 
 *
 *  PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 *  This header contains the global/common variables of the project.
 * */

//---Title: PF-Project, Fall-2022 for BS(CS)---//
const char title[] = "PF-Project, Fall-2022";

//---Height and Width of the Actual Interactive Game---//
const int M = 20;   // Number of rows for a piece to cover on the screen = 20
const int N = 10;   // Number of columns for a piece to cover on the screen = 10

//---The Actual Interactive Game Grid - Built Over (MxN)---//
int gameGrid[M][N] = {0};

//---Struct to hold coordinates---//
struct Point {
    int x, y;
};

//---To Hold the Coordinates of the Piece---//
// point_1 is the current piece, point_2 is for collision checking/shadow
Point point_1[4], point_2[4];

//---Check Uncertain Conditions---//
bool anamoly() {
    for (int i = 0; i < 4; i++) {
        if (point_1[i].x < 0 || point_1[i].x >= N || point_1[i].y >= M)
            return false;
        else if (gameGrid[point_1[i].y][point_1[i].x])
            return false;
    }
    return true;
}

#endif
