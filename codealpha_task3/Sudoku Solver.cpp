#include <iostream>
using namespace std;

const int N = 9;

void printBoard(int board[N][N]) {

    for (int r = 0; r < N; r++) {

        if (r % 3 == 0)
            cout << "+-------+-------+-------+\n";

        for (int c = 0; c < N; c++) {

            if (c % 3 == 0)
                cout << "| ";

            if (board[r][c] == 0)
                cout << ". ";
            else
                cout << board[r][c] << " ";
        }

        cout << "|\n";
    }

    cout << "+-------+-------+-------+\n";
}

bool isSafe(int board[N][N],
            int row, int col, int num) {

    for (int i = 0; i < N; i++) {

        if (board[row][i] == num ||
            board[i][col] == num)
            return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int r = startRow; r < startRow + 3; r++) {

        for (int c = startCol; c < startCol + 3; c++) {

            if (board[r][c] == num)
                return false;
        }
    }

    return true;
}

bool isBoardValid(int board[N][N]) {

    for (int r = 0; r < N; r++) {

        for (int c = 0; c < N; c++) {

            int num = board[r][c];

            if (num == 0)
                continue;

            board[r][c] = 0;

            bool valid = isSafe(board, r, c, num);

            board[r][c] = num;

            if (!valid)
                return false;
        }
    }

    return true;
}

bool solveSudoku(int board[N][N]) {

    int row = -1, col = -1;
    bool emptyFound = false;

    for (int r = 0; r < N && !emptyFound; r++) {

        for (int c = 0; c < N; c++) {

            if (board[r][c] == 0) {
                row = r;
                col = c;
                emptyFound = true;
                break;
            }
        }
    }

    if (!emptyFound)
        return true;

    for (int num = 1; num <= 9; num++) {

        if (isSafe(board, row, col, num)) {

            board[row][col] = num;

            if (solveSudoku(board))
                return true;

            board[row][col] = 0;
        }
    }

    return false;
}

int main() {

    int board[N][N];

    cout << "===== SUDOKU SOLVER =====\n";

    cout << "Enter 9 rows with 9 numbers each.\n";
    cout << "Use 0 for empty cells.\n\n";

    for (int r = 0; r < N; r++) {

        for (int c = 0; c < N; c++) {

            cin >> board[r][c];

            if (!cin || board[r][c] < 0 ||
                board[r][c] > 9) {

                cout << "Invalid input.\n";
                return 0;
            }
        }
    }

    cout << "\nOriginal Sudoku:\n";
    printBoard(board);

    if (!isBoardValid(board)) {
        cout << "The puzzle contains duplicate numbers.\n";
        return 0;
    }

    if (solveSudoku(board)) {

        cout << "\nSolved Sudoku:\n";
        printBoard(board);
    }
    else {
        cout << "No solution exists.\n";
    }

    return 0;
}