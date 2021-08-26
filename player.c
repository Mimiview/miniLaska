//
// Created by Alex on 18/08/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "headers/player.h"
#include "headers/board.h"
/**
 * funzione principale per la modalità 1v1
 * @param board pedina
 * @param win pedina
 * @param player pedina
 * @return void;
 */
void one_vs_one(board_t board, int win,int player) {
    if(!win) {
        int x, y, x_moved, y_moved;
        if (!winner(board, player % 2)) {
            if (player % 2 == 0)
                printf("------------------------RED TURN-------------------------\n\n");
            else
                printf("------------------------BLUE TURN-------------------------\n\n");

            print_board(&board);
            printf("scegli il pedone da muovere \n");
            scanf("%d%d", &x, &y);
            printf("scegli la posizione in cui muoverlo\n");
            scanf("%d%d", &x_moved, &y_moved);

            if (!is_in_bounds(x_moved,y_moved) || !is_in_bounds(x,y) || !move_factory(board, board->b[x][y], x_moved, y_moved) )
                printf("\n\n\nMossa non disponibile, provane un'altra\n\n");
            one_vs_one(board,0, player+1);

        }
        else
            one_vs_one(board,1,player);

    }else {
        if(player%2 == 0){
            printf("RED TEAM WINS");
        } else
            printf("BLUE TEAM WINS");
        delete_board(&board);
    }
}

