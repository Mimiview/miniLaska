//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/pawn.h"

/**
 * Inizializza un puntatore ad una struct pawn
 * @param x coordinata x della pedina
 * @param y coordinata y della pedina
 * @param color colore da dare alla pedina
 * @param status lo stato della pedina(cosa che potevamo omettere a causa del fatto che la inizializziamo sempre a soldier)
 * @return pawn_t inizializzata in memoria
 */
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

/**
 * Elimina del tutto lo spazio in memoria di una pedina
 * @param p pedina da eliminare
 * @return void
 */
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

/**
 * mette in fondo lo "stack" la pedina che passiamo come secondo parametro
 * @param p pedina
 * @return 1 se l'append è andato a buon fine;
 */
int append(pawn_t p, pawn_t food) {
    if (p->next == NULL) {
        p->next = food;
        p->next->y = p->y;
        p->next->x = p->x;
        food->next = NULL;
        return 1;
    } else
        return append(p->next, food);
}


pawn_t delete_head_pawn(pawn_t *p) {
    if ((*p)) {
        pawn_t aux;
        aux = *p;
        if((*p)->next)
            *p = (*p)->next;
        return aux;

    } else
        (*p) = NULL;

}

/**
 * elimina l'ultima pedina dello stack
 * @param p pedina dalla quale eliminare l'utlimo elemento
 * @return 1 se viene eliminata, 0 se non lo è;
 */
int delete_last_pawn(pawn_t p) {
    if (p) {
        if (p->next->next) {
            p->next->next->next = NULL;
            return 1;
        }
        return 0;
    }
    return 0;
}

/**
 * conta quante pedine ci sono in uno stack
 * @param p pedina da studiare
 * @return il numero di pedine in uno stack;
 */
int count_stack(pawn_t p) {
    if (p) {
        if (p->next == NULL) {
            return 1;
        }
        return 1 + count_stack(p->next);
    } else
        return 0;
}




