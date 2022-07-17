#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>


void matrix_initialization(int **matrix, int *nachinka, int rows, int cols);
void next_gen(int **matrix_1, int **matrix_2, int rows, int cols);
void input(int **matrix, int n, int m);
void output(int **matrix, int n, int m);
int dot_check(int row, int col, int **matrix, int rows, int cols);
void sleep(int var);
void speed_mode(int *speed);
void file_choice(int **matrix, int n, int m);
void start_screen();

int main() {
    char move = ' ';
    int width = 80, height = 25, speed = 5.5, flag = 0;
    int *matrix_first[height], nachinka_first[height * width];
    int *matrix_second[height], nachinka_second[height * width];
    matrix_initialization(matrix_first, nachinka_first, height, width);
    matrix_initialization(matrix_second, nachinka_second, height, width);
    file_choice(matrix_first, height, width);
    initscr();
    nodelay(stdscr, TRUE);
    while (1) {
        clear();
        if (flag % 2 == 0) {
            output(matrix_first, height, width);
            next_gen(matrix_first, matrix_second, height, width);
            flag++;
        } else {
            output(matrix_second, height, width);
            next_gen(matrix_second, matrix_first, height, width);
            flag++;
        }
        sleep(speed);
        move = getch();
        if (move == 'q' || move == 'Q')  break;
        else if ((move == 'w' || move == 'W') && speed > 1)  speed -= 1;
        else if ((move == 's' || move == 'S') && speed < 20)  speed += 2; //there changed
    }
    endwin();
    return 0;
}

void file_choice(int **matrix, int n, int m) {
    FILE *file;
    system("clear");
    start_screen();
    int file_mode = 0;
    scanf("%d", &file_mode);
    if (file_mode == 1)  file = fopen("./files/1.txt", "r");
    else if (file_mode == 2)  file = fopen("./files/2.txt", "r");
    else if (file_mode == 3)  file = fopen("./files/3.txt", "r"); //there changed
    else if (file_mode == 4)  file = fopen("./files/4.txt", "r");
    else if (file_mode == 5)  file = fopen("./files/5.txt", "r");
    if (file_mode < 6) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                fscanf(file, "%1d", &matrix[i][j]);
            }
        }
    } else {
        input(matrix, n, m);
    }
}

void start_screen() {
    int n = 19;
    for (int i = 0; i < n; i++) {
        if (i == 0 || i == n - 1) {
            for (int j = 0; j < 82; j++) { printf("#"); }
        } else if (i % 2 == 1)  printf("#%80s#", " ");
        else if (i == 2)  printf("#%34sGAME OF LIFE%34s#", " ", " ");
        else if (i == 4)  printf("#%34s-CHOOSE MAP-%34s#", " ", " ");
        else if (i == 6)  printf("#%34s1 -  CORNERS%34s#", " ", " ");
        else if (i == 8)  printf("#%34s2 -   CENTER%34s#", " ", " ");
        else if (i == 10)  printf("#%34s3 -    GUN 1%34s#", " ", " "); // there changed
        else if (i == 12)  printf("#%34s4 -    GUN 2%34s#", " ", " ");
        else if (i == 14)  printf("#%34s5 -   UNIQUE%34s#", " ", " ");
        else if (i == 16)  printf("#%34s6 -   RANDOM%34s#", " ", " ");
        printf("\n");
    }
}

void matrix_initialization(int **matrix, int *nachinka, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        matrix[i] = &nachinka[i * cols];
    }
}

void output(int **a, int n, int m) {
    for (int i = 0; i < n + 6; i++) {
        for (int j = 0; j < m + 2; j++) {
            if (i > n + 1) {
                if (j == 0 || j == m + 1 || i == n + 5) {
                    printw("#");
                } else if (j == 1 && (i == n + 3)) {
                    printw("%13sQ - QUIT%10sW - SPEED UP", " ", " ");
                    printw("%10sS - SPEED DOWN%13s", " ", " ");
                } else if (i != n + 3) {
                    printw(" ");
                }
            } else if (i == 0 || j == 0 || i == n + 1 || j == m + 1) {// no changes cuz may cause error
                printw("#");
            } else if (a[i - 1][j - 1] == 1) {
                printw("@");
            } else printw(" ");
        }
        printw("\n");
    }
}

void next_gen(int **matrix_1, int **matrix_2, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix_1[i][j] == 0) {
                if (dot_check(i, j, matrix_1, rows, cols) == 3) matrix_2[i][j] = 1;
                else matrix_2[i][j] = 0;
            } else {
                if (dot_check(i, j, matrix_1, rows, cols) == 2 ||
                    dot_check(i, j, matrix_1, rows, cols) == 3)  matrix_2[i][j] = 1;
                else matrix_2[i][j] = 0; //there changed
            }
        }
    }
}

int dot_check(int row, int col, int **matrix, int rows, int cols) {
    int alive_dots = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {
            } else { // no changes cuz after compilation may cause errors
                alive_dots += matrix[(row + i + rows) % rows][(col + j + cols) % cols];
            }
        }
    }
    return alive_dots;
}

void input(int **a, int n, int m) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[i][j] = rand() % 2;
        }
    }
}

void sleep(int var) {
    int x = 0;
    for (int i = 0; i < var * 10000000; i++) {
        x++;
    }
}

