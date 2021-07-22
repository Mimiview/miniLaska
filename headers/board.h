//
// Created by Alex on 04/07/2021.
//

#ifndef MINILASKA_BOARD_H
#define MINILASKA_BOARD_H

#include "pawn.h"

#define DIMENSION 7

typedef struct board{
    pawn_t b[DIMENSION][DIMENSION];
}* board_t;

board_t init_board();

void delete_board(board_t *b);

void print_board(board_t *b);

int is_in_bounds(int x, int y);

void test_for_piggies();

int move_factory(board_t b,pawn_t p, int x, int y);

int eat(board_t b, pawn_t p, int x, int y);

int normal_move(board_t b, pawn_t p, int x, int y);

#endif //MINILASKA_BOARD_H
