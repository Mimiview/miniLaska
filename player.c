//
// Created by Alex on 18/08/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include "headers/player.h"
#include "headers/board.h"
//todo vedere dove cazzo sta lo stack overflow serpe divina, di sicuro in winnere
void one_vs_one(board_t board, int win) {
    int player_cont=0, x, y, x_moved, y_moved;
    if (!winner(board, player_cont%2)) {
        if(player_cont %2 == 0)
            printf("------------------------PLAYER ONE TURN-------------------------\n\n");
        else
            printf("------------------------PLAYER TWO TURN-------------------------\n\n");

        print_board(&board);
        printf("scegli il pedone da muovere \n");
        scanf("%d%d", &x, &y);
        printf("scegli la posizione in cui muoverlo\n");
        scanf("%d%d", &x_moved, &y_moved);

        if(!move_factory(board, board->b[x][y],x_moved,y_moved))
            printf("Mossa non disponibile, rifai siooo\n\n");
        player_cont++;

    }else
        printf("HA VINTO IL GIOCATORE NUMERO  %d", player_cont%2);
}

