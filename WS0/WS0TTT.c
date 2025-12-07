#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "WS0TTT.h"

char board[3][3];

void initialize_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void print_board() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
    printf("\n");
}

bool is_valid_move(int row, int col) {
    return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
}

void make_move(int row, int col, char player) {
    board[row][col] = player;
}

bool check_win(char player) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }
    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }
    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    return false;
}

bool is_board_full() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    initialize_board();
    char current_player = 'X';
    bool game_over = false;

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("Players take turns entering row and column (0-2).\n");

    while (!game_over) {
        print_board();
        int row, col;
        printf("Player %c, enter row (0-2): ", current_player);
        scanf("%d", &row);
        printf("Player %c, enter column (0-2): ", current_player);
        scanf("%d", &col);

        if (is_valid_move(row, col)) {
            make_move(row, col, current_player);
            if (check_win(current_player)) {
                print_board();
                printf("Player %c wins!\n", current_player);
                game_over = true;
            } else if (is_board_full()) {
                print_board();
                printf("It's a draw!\n");
                game_over = true;
            } else {
                current_player = (current_player == 'X') ? 'O' : 'X';
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    return 0;
}
