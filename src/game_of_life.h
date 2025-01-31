#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 25
#define WIDTH 80

void draw(int **matr, int speed);
int inputFile(int **matr);
void keyboard(int *delay, int *flag);

void transform(int **matr, int *ptr);
void copyMatrix(int **matr1, int **matr2);
int compareMatrix(int **matr1, int **matr2);

int nextGeneration(int **matr);
int countAlives(int **matr, int i, int j);
int rule(int alive_neighs, int cell);

int updateSimilarity(int **matr1, int **matr2, int sim);

void startGame();
void endGame();

#endif