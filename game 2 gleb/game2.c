#include <stdio.h>

#include <stdlib.h>

#include <termios.h>

#include <unistd.h>

#include <sys/time.h>

#include <sys/types.h>

#define ROWS 25

#define COLS 80

void allocateMatrix(int ***arr, int m, int n);

void freeMatrix(int ***arr, int m);

int countNeighbours(int **arr, int y, int x);

void changeMatrix(int **arr, int **nextMatrix);

void draw(int **arr);

void input(int **arr);

void set_keypress(void);

void reset_keypress(void);

static struct termios stored_settings;

int main() {

  fd_set rfds;

  struct timeval tv;

  int **matrix, **nextMatrix;

  allocateMatrix(&matrix, ROWS, COLS);

  allocateMatrix(&nextMatrix, ROWS, COLS);

  input(matrix);

  if (freopen("/dev/tty", "r", stdin) != NULL) {

    int gameLife = 1;

    int speed = 5;

    while (gameLife) {

      set_keypress();

      char choice;

      FD_ZERO(&rfds);

      FD_SET(0, &rfds);

      tv.tv_sec = 0;

      tv.tv_usec = 0;

      int retval = select(2, &rfds, NULL, NULL, &tv);

      printf("\e[1;1H\e[2J");

      changeMatrix(matrix, nextMatrix);

      draw(matrix);

      if (retval) {

        choice = getchar();

      } else {

        choice = ' ';

      }

      if (choice == 'q') {

        freeMatrix(&matrix, ROWS);

        freeMatrix(&nextMatrix, ROWS);

        gameLife = 0;

      } else if ((choice == 'w' || choice == 'W') && speed > 1) {

        speed--;

      } else if ((choice == 's' || choice == 'S') && speed < 10) {

        speed++;

      }

      usleep(speed * 25000);

    }

  }

  reset_keypress();

  return 0;

}

void input(int **arr) {

  for (int i = 0; i < ROWS; i++) {

    for (int j = 0; j < COLS; j++) {

      int temp;

      scanf("%d", &temp);

      arr[i][j] = temp;

    }

  }

}

void draw(int **arr) {

  for (int i = 0; i < ROWS; i++) {

    for (int j = 0; j < COLS; j++) {

      if (arr[i][j] == 1) {

        printf("*");

      } else {

        printf(" ");

      }

    }

    printf("\n");

  }

}

void changeMatrix(int **arr, int **nextMatrix) {

  for (int i = 0; i < ROWS; i++) {

    for (int j = 0; j < COLS; j++) {

      int neighbours = countNeighbours(arr, i, j);

      int state = arr[i][j];

      if (neighbours == 3 && state == 0) {

        nextMatrix[i][j] = 1;

      } else if ((neighbours > 3 || neighbours < 2) && state == 1) {

        nextMatrix[i][j] = 0;

      } else {

        nextMatrix[i][j] = state;

      }

    }

  }

  for (int i = 0; i < ROWS; i++) {

    for (int j = 0; j < COLS; j++) {

      arr[i][j] = nextMatrix[i][j];

    }

  }

}

// Функция подсчета живых соседей

int countNeighbours(int **arr, int y, int x) {

  int sum = 0;

  for (int i = -1; i < 2; i++) {

    for (int j = -1; j < 2; j++) {

      int row = (y + i + ROWS) % ROWS;

      int col = (x + j + COLS) % COLS;

      sum += arr[row][col];

    }

  }

  sum -= arr[y][x];

  return sum;

}

// Выделение памяти

void allocateMatrix(int ***arr, int m, int n) {

  *arr = NULL;

  (*arr) = (int **)malloc(sizeof(int *) * m);

  for (int i = 0; i < m; i++) {

    (*arr)[i] = (int *)malloc(sizeof(int) * n);

  }

}

// Освобождение памяти

void freeMatrix(int ***arr, int m) {

  for (int i = 0; i < m; i++) {

    free((*arr)[i]);

  }

  free(*arr);

  *arr = NULL;

}

void set_keypress(void) {

  struct termios new_settings;

  tcgetattr(0, &stored_settings);

  new_settings = stored_settings;

  /*

      Отключение канонического режима и вывода на экран

      и установка буфера ввода размером в 1 байт

  */

  new_settings.c_lflag &= (~ICANON & ~ECHO);

  new_settings.c_cc[VTIME] = 0;

  new_settings.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, &new_settings);

  return;

}

void reset_keypress(void) {

  tcsetattr(0, TCSANOW, &stored_settings);

  return;

}
