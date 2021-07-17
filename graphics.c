//
// Created by Alex on 08/07/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include "headers/graphics.h"

#define SQUAREX 29
#define SQUAREY 71

//todo vedere in che modo fare una mega riga di spaces
void print_empty_square(int x, int y) {
    for (int i = 0; i < SQUAREX; ++i) {
        for (int j = 0; j < SQUAREY; ++j) {
            if (j % 10 == 0 && i % 4 == 0) {
                printf("%c", 177);

            } else if (j % 10 == 0) {
                printf("%c", 177);
            } else if (i % 4 == 0)
                printf("%c", 177);
            else
                printf(" ");


        }
        printf("\n");
    }
}

void print_full_square(int x, int y) {
    for (int i = 0; i < x / 2; ++i) {
        for (int j = 0; j < y; ++j) {
            printf("%c", 177);
        }
        printf("\n");
    }
}

void print_raster(int x, int y) {
    int flag = 0, cont = 1;
    for (int i = 0; i < x * 7; ++i) {
        for (int j = 0; j < y * 7; ++j) {
            if (!flag) {
                if (i == 0 || j == 0 || i % 7 == 0 || j % 7 == 0) {
                    printf("%c", 177);
                } else
                    printf(" ");
                if (cont++ == 7) {
                    flag = 1;
                    cont = 1;
                }
            } else {
                printf("%c", 177);
                if (cont++ == 7) {
                    flag = 0;
                    cont = 1;
                }
            }
        }
    }
}

