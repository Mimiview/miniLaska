//
// Created by Alex on 04/07/2021.
//

#ifndef MINILASKA_BOARD_H
#define MINILASKA_BOARD_H



#define DIMENSION 7
/**
 * struttura della board composta da un array di pedine staticamente inizializzato 7x7
 */

enum color_pawn {
    BLUE, RED
};
enum status {
    SOLDIER, GENERAL
};

typedef struct pawn {
    int x, y;
    enum color_pawn color;
    enum status status;
    struct pawn *next;
} *pawn_t;

typedef struct board {
    pawn_t b[DIMENSION][DIMENSION];
} *board_t;


pawn_t init_pawn(int x, int y, enum color_pawn color, enum status status);

void delete_pawn(pawn_t p);

int delete_last_pawn(pawn_t p);

pawn_t delete_head_pawn(board_t b, pawn_t *p);

int count_stack(pawn_t p);

int append(pawn_t p, pawn_t r);

board_t init_board();

void delete_board(board_t *b);

void print_board(board_t *b);

int is_in_bounds(int x, int y);

int there_is_mandatory_move(board_t b, enum color_pawn color);

int move_factory(board_t b, pawn_t p, int x, int y);

int eat(board_t b, pawn_t p, int x, int y);

int normal_move(board_t b, pawn_t p, int x, int y);

int winner(board_t b, enum color_pawn color);

#endif //MINILASKA_BOARD_H
