//
// Created by Alex on 18/08/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "headers/player.h"

void one_vs_one(board_t board, int win) {
    int x, y, x_moved, y_moved;
    if (!win) {
        printf("PLAYER ONE STARTS\n\n\n");
        print_board(&board);
        printf("scegli il pedone da muovere \n");
        scanf("%d%d", &x, &y);
        printf("scegli la posizione in cui muoverlo\n");
        scanf("%d%d", &x_moved, &y_moved);

        if(!move_factory(board, board->b[x][y],x_moved,y_moved))
            printf("Mossa non disponibile, rifai siooo\n\n");
        one_vs_one(board,0);

    }
}

