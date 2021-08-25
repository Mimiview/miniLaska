//
// Created by Alex on 04/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/board.h"
#include "headers/pawn.h"

/**
 * inizializza la board di gioco con opportune pedine messe nei loro specifici posti
 * @return void
 */

board_t init_board() {
    int i, j;
    board_t board = (board_t) malloc(sizeof(struct board));
    if (board) {
        for (i = 0; i < DIMENSION; i++) {
            for (j = 0; j < DIMENSION; j++) {
                board->b[i][j] = NULL;
                if ((i + j) % 2 == 0) {
                    if (i < 3)
                        board->b[i][j] = init_pawn(i, j, BLUE, SOLDIER);
                    if (i > 3)
                        board->b[i][j] = init_pawn(i, j, RED, SOLDIER);
                }
            }
        }
        return board;
    }
    printf("malloc crushed");
    exit(1);

}

/**
 * elimina lo spazio in memoria della board
 * @param b puntatore alla board da eliminare
 * @return void
 */
//da vedere se distruggerla del tutto oppure se porla a NULL
void delete_board(board_t *b) {
    int i, j;

    if (*b) {
        for (i = 0; i < DIMENSION; i++) {
            for (j = 0; j < DIMENSION; j++) {
                delete_pawn((*b)->b[i][j]);
            }
        }
        free(*b);
    }
}

/**
 * controlla se determinate coordinate sono tra i numeri 0 ed 6
 * @param x coordinata x
 * @param y coordinata y
 * @return 1 se è tra i bounds, 0 se non lo è;
 */
int is_in_bounds(int x, int y) {
    return x >= 0 && x <= 6 && y >= 0 && y <= 6;
}

/**
 * studia la legalità della mossa con salti di 1 o 2 caselle,
 * la prima condizione serve a vedere se la mossa entra nei range di mossa,
 * al suo interno lo statement studia la correttezza del movimento della pedina
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se il movimento da fare è legale, 0 altrimenti;
 */
int is_a_legal_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {

        if (is_in_bounds(x, y) && ((p->x == x - 1 || p->x == x + 1 || p->x == x + 2 || p->x == x - 2) &&
                                   (p->y == y - 1 || p->y <= y + 1 || p->y == y + 2 || p->y <= y - 2))) {
            if (p->status == GENERAL)
                return 1;
            if (p->color == BLUE) {
                return (((x == p->x + 1) && (y == p->y - 1) || (x == p->x + 2) && (y == p->y - 2)) ||
                        ((x == p->x + 1) && (y == p->y + 1) || (x == p->x + 2) && (y == p->y + 2)));
            } else
                return (((x == p->x - 1) && (y == p->y - 1) || (x == p->x - 2) && (y == p->y - 2)) ||
                        ((x == p->x - 1) && (y == p->y + 1) || (x == p->x - 2) && (y == p->y + 2)));
        }
    } else
        return 0;
}

/**
 * vede se la pedina presa in questione ha la possibilità di muoversi
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se è possibile fare il movimento, 0 altrimenti;
 */
int is_possible_to_move(board_t b, pawn_t p, int x, int y) {
    if (b && p) {
        if (b->b[x][y] == NULL) {
            if (is_a_legal_move(b, p, x, y)) {
                return 1;
            } else
                return 0;
        } else
            return 0;
    }
}

/**
 * vede se la pedina p ha la possibilità di mangiare
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se è possibile mangiare, 0 altrimenti;
 */

int is_possible_to_eat(board_t b, pawn_t p, int x, int y) {
    if (is_possible_to_move(b, p, x, y)) {
        if (b->b[(p->x + x) / 2][(p->y + y) / 2] != NULL) {
            if (b->b[(p->x + x) / 2][(p->y + y) / 2]->color != p->color) {
                if (b->b[(p->x + x) / 2][(p->y + y) / 2]->status != GENERAL) {
                    return 1;
                } else
                    return p->status == GENERAL;
            }
            return 0;
        }
        return 0;
    }
}


/**
 * funzione che fa mangiare una pedina di cui studiamo le coordinate all'interno della funzione, dalla pedina p
 * @param board la board di gioco
 * @param p la pedina che mangia
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se ha mangiato e p esiste, 0 altrimenti;
 */
int eat(board_t board, pawn_t p, int x, int y) {
    if (p) {
        int x_food, y_food;
        pawn_t tmp;
        x_food = (p->x + x) / 2; // con questa espressione troviamo la x e la y del mangiato
        y_food = (p->y + y) / 2;



        append(p, board->b[x_food][y_food]);

        delete_head_pawn(&board->b[x_food][y_food]);
        if (count_stack(p) >= 3)
            delete_last_pawn(p);

        normal_move(board, board->b[p->x][p->y], x, y);

        return 1;
    }
    return 0;
}

/**
 * fa compiere alla pedina p un movimento senza vincoli
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se esiste la pedina e se la muoviamo, 0 altrimenti;
 */
int normal_move(board_t b, pawn_t p, int x, int y) {
    if (p) {
        b->b[x][y] = b->b[x - (x - p->x)][y - (y - p->y)];
        b->b[x - (x - p->x)][y - (y - p->y)] = NULL;
        p->x = x;
        p->y = y;
        if (p->color == BLUE && p->x == 6 || p->color == RED && p->x == 0)
            p->status = GENERAL;


        while (p->next) {
            p->next->x = x;
            p->next->y = y;
            p = p->next;
        }
        return 1;
    }
    return 0;
}

/**
 * questa è la funzione che, chiamata all'interno della funzione onevsone nel file player, decide quale mossa è da fare, se un semplice movimento oppure se una cattura.
 * Inoltre qui controlliamo se non vi sono mosse obbligatorie da fare
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @param x coordinata x in cui vogliamo muoverla
 * @param y coordinata y in cui muoverla
 * @return 1 se è possibile fare il movimento, 0 altrimenti;
 */
int move_factory(board_t b, pawn_t p, int x, int y) {
    if (p) {
        if (is_possible_to_eat(b, p, x, y)) {
            return eat(b, p, x, y);
        } else if (is_possible_to_move(b, p, x, y) && !there_is_mandatory_move(b, p->color))
            return normal_move(b, p, x, y);
        else
            return 0;
    }
    return 0;
}

/**
 * vede se una pedina possa fare alcun movimento
 * @param board la board di gioco
 * @param p la pedina da muovere
 * @return 1 se è possibile fare il movimento, 0 altrimenti;
 */
int paw_can_moves(board_t b, pawn_t p) {
    if (b && p)
        return (is_possible_to_move(b, p, p->x + 1, p->y + 1) || is_possible_to_move(b, p, p->x + 1, p->y - 1) ||
                is_possible_to_move(b, p, p->x + 2, p->y + 2) || is_possible_to_move(b, p, p->x + 2, p->y - 2) ||
                is_possible_to_move(b, p, p->x - 1, p->y - 1) || is_possible_to_move(b, p, p->x - 2, p->y - 2) ||
                is_possible_to_move(b, p, p->x - 1, p->y + 1) || is_possible_to_move(b, p, p->x - 2, p->y + 2));
    return 0;
}

/**
 * vede se una pedina ha la possibilità di mangiare
 * @param board la board di gioco
 * @param p la pedina da studiare
 * @return 1 se la pedina ha possibilità di cattura, 0 altrimenti;
 */
int pawn_can_eat(board_t b, pawn_t p) {
    if (b && p)
        return (
                is_possible_to_eat(b, p, p->x + 2, p->y + 2) ||
                is_possible_to_eat(b, p, p->x + 2, p->y - 2) ||
                is_possible_to_eat(b, p, p->x - 2, p->y - 2) ||
                is_possible_to_eat(b, p, p->x - 2, p->y + 2));
    return 0;
}

/**
 * studia, in base al colore della pedina, la possibilità di avere una mossa obbligatoria o meno all'interno della board
 * @param board la board di gioco
 * @param color colore delle pedine da studiare
 * @return 1 se esiste un movimento obbligatorio da fare, 0 altrimenti;
 */
int there_is_mandatory_move(board_t b, enum color_pawn color) {
    int flag;
    flag = 0;
    for (int i = 0; i < DIMENSION; ++i) {
        for (int j = 0; j < DIMENSION; ++j) {
            if (b->b[i][j] != NULL)
                if (b->b[i][j]->color == color && pawn_can_eat(b, b->b[i][j]))
                    flag = 1;
        }
    }
    return flag;
}

/**
 * studia, in base al colore della pedina, se vi sono o meno mosse da fare
 * @param board la board di gioco
 * @param color colore delle pedine da studiare
 * @return 1 se non esistono più movimenti da fare, 0 altrimenti;
 */
int no_more_moves(board_t b, enum color_pawn color) {
    int flag;
    flag = 1;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (b->b[i][j] != NULL)
                if (b->b[i][j]->color == color && paw_can_moves(b, b->b[i][j]))
                    flag = 0;
        }
    }
    return flag;
}

/**
 * studia, in base al colore della pedina, se vi sono pedine rimanenti
 * @param board la board di gioco
 * @param color colore delle pedine da studiare
 * @return 1 se non ci sono più pedine aventi color = color, 0 altrimenti;
 */
int no_more_paws(board_t b, enum color_pawn color) {
    int flag = 1;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (b->b[i][j] != NULL && b->b[i][j]->color == color)
                flag = 0;
        }
    }
    return flag;
}

/**
 * vede se le pedine di colore color hanno vinto o meno
 * @param board la board di gioco
 * @param color colore delle pedine da studiare
 * @return 1 se il giocatore con le pedine di colore color ha vinto, 0 se la partita continua;
 */
int winner(board_t b, enum color_pawn color) {
    enum color_pawn enemy;
    if (color == RED)
        enemy = BLUE;
    else
        enemy = RED;


    if (no_more_paws(b, enemy) || no_more_moves(b, enemy))
        return 1;
    return 0;
}

//
// ******** GRAPHICS ***************

void print_pedina(pawn_t p) {
    if (p) {
        while (p) {
            if (p->color == BLUE) {
                if (p->status == GENERAL)
                    printf("B");
                else
                    printf("b");
            } else {
                if (p->status == GENERAL)
                    printf("R");
                else
                    printf("r");
            }
            p = p->next;
        }
    } else printf(" ");


}

void print_board(board_t *board) {
    int i, j;
    printf("  ");
    for (i = 0; i <= 27; i++) {
        if (i % 4 == 0) {
            printf("%d", i / 4);
        } else
            printf(" ");
    }
    printf("\n");
    for (i = 0; i < DIMENSION; i++) {
        printf("%d ", i);
        for (j = 0; j < DIMENSION; j++) {
            print_pedina((*board)->b[i][j]);
            printf(" | ");
        }
        printf("\n");
    }
}

char get_char(pawn_t pawn) {
    if (pawn) {
        if (pawn->color == BLUE) {
            if (pawn->status == GENERAL) {
                return 'B';
            } else return 'b';
        } else {
            if (pawn->status == GENERAL) {
                return 'R';
            } else return 'r';
        }
    }
    return ' ';
}


void test_for_piggies() {
    board_t b = init_board();
    pawn_t p = init_pawn(4, 2, BLUE, SOLDIER);
    move_factory(b, b->b[4][2], 3, 3);
    move_factory(b, b->b[2][4], 4, 2);
    move_factory(b, b->b[5][1], 3, 3);
    move_factory(b, b->b[3][3], 2, 4);
    move_factory(b, b->b[1][5], 3, 3);
    delete_last_pawn(b->b[3][3]);
    delete_pawn(b->b[6][0]);
    normal_move(b, b->b[2][2], 6, 0);
    //eat(b, b->b[4][2], 2, 4);
    //
    print_board(&b);
    printf("pedine %d", count_stack(b->b[3][3]));
    //printf("%d ", is_possible_to_eat(b, b->b[1][5], 3, 3));
    //print_pedina(b->b[3][3]);
    //printf("%d mimmo", is_possible_to_move(b, b->b[5][1], 3, 4));
    //printf("%d", count_stack(b->b[4][2]));
    delete_board(&b);
}