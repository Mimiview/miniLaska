//
// Created by Alex on 04/07/2021.
//

#ifndef MINILASKA_PAWN_H
#define MINILASKA_PAWN_H

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

pawn_t init_pawn(int x, int y, enum color_pawn color, enum status status);

void delete_pawn(pawn_t p);

int delete_last_pawn(pawn_t p);

pawn_t delete_head_pawn(pawn_t *p);

int count_stack(pawn_t p);

int append(pawn_t p, pawn_t r);


#endif //MINILASKA_PAWN_H
