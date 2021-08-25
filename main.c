#include <stdio.h>
#include <stdlib.h>
#include "headers/board.h"
#include "headers/pawn.h"
#include "headers/graphics.h"
#include "headers/player.h"

#define BOARD_SIZE 7


int main() {
    board_t b = init_board();
    //test_for_piggies();
    one_vs_one(b,0,0);
}
