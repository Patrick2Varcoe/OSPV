#ifndef WS0TTT_H
#define WS0TTT_H

extern char board[3][3];

void initialize_board();
void print_board();
bool is_valid_move(int row, int col);
void make_move(int row, int col, char player);
bool check_win(char player);
bool is_board_full();

#endif
