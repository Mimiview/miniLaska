//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/pawn.h"


pawn_t init_pawn(int x, int y, enum color_pawn color, enum status status) {
    pawn_t p;
    p = (pawn_t) malloc(sizeof(struct pawn));
    if (p) {
        p->x = x;
        p->y = y;
        p->status = status;
        p->color = color;
        p->next = NULL;
        return p;
    }
    printf("malloc crushed");
    exit(1);
}
//todo risolvere problema delete
void delete_pawn(pawn_t p) {
    if (p) {
        pawn_t aux;
        if (p->next)
            delete_pawn(p->next);
        aux = p;
        free(aux);
        p = NULL;
    }
}

//inserisce il pedone food alla fine dello stack, appena messo ritorna 1;
int append(pawn_t p, pawn_t food) {
    if (p->next == NULL) {
        p->next = food;
        p->next->y = p->y;
        p->next->x = p->x;
        return 1;
    } else
        return append(p->next, food);
}

int count_stack(pawn_t p){
    if(p) {
        if (p->next == NULL) {
            return 1;
        }
        return 1 + count_stack(p->next);
    }else
        return 0;
}




