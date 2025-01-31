#include "game_of_life.h"

int main() {
    int grid_static[HEIGHT][WIDTH];
    int compare_matrix_static[HEIGHT][WIDTH];
    int *grid[HEIGHT];
    int *compare_matrix[HEIGHT];

    transform(grid, (int *)grid_static);
    transform(compare_matrix, (int *)compare_matrix_static);

    int exit_flag = inputFile(grid);
    startGame();

    int delay = 100;
    int similarity = 0;
    int counter = 0;

    while (similarity < 4 && !exit_flag) {
        clear();
        napms(delay);

        draw(grid, delay);
        keyboard(&delay, &exit_flag);

        if (!exit_flag) {
            exit_flag = nextGeneration(grid);

            if (counter % 8 == 0) {
                similarity = updateSimilarity(grid, compare_matrix, similarity);
            }

            counter++;
        }
    }
    endGame();
    return 0;
}

int inputFile(int **matr) {
    int flag = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            scanf("%d", &matr[i][j]);
        }
    }
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        flag = 1;
    }
    return flag;
}

void keyboard(int *delay, int *flag) {
    char button = getch();

    switch (button) {
        case 'q':
        case 'Q':
            *flag = 1;
            break;
        case 's':
        case 'S':
            *delay += 10;
            break;
        case 'w':
        case 'W':
            *delay -= 10;
            break;
    }
}

void draw(int **matr, int delay) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matr[i][j] == 1) {
                printw("&");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    printw("Delay: %d msec\n", delay);
}

int nextGeneration(int **matr) {
    int memory_flag = 0;
    int **temp_matrix = malloc(HEIGHT * sizeof(int *) + HEIGHT * WIDTH * sizeof(int));

    if (temp_matrix == NULL) {
        memory_flag = 1;
    }

    if (!memory_flag) {
        int *ptr = (int *)(temp_matrix + HEIGHT);
        transform(temp_matrix, ptr);

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                int alive_neighs = countAlives(matr, i, j);
                temp_matrix[i][j] = rule(alive_neighs, matr[i][j]);
            }
        }
        copyMatrix(temp_matrix, matr);
    }
    free(temp_matrix);
    return memory_flag;
}

int countAlives(int **matr, int i, int j) {
    int alive_neighs = 0;
    for (int i_neigh = i - 1; i_neigh <= i + 1; i_neigh++) {
        for (int j_neigh = j - 1; j_neigh <= j + 1; j_neigh++) {
            if (i_neigh == i && j_neigh == j) {
                ;
            } else if (matr[(i_neigh + HEIGHT) % HEIGHT][(j_neigh + WIDTH) % WIDTH] != 0) {
                alive_neighs++;
            }
        }
    }
    return alive_neighs;
}

int rule(int alive_neighs, int cell) {
    int live = 0;

    if (cell == 1) {
        if (alive_neighs == 2 || alive_neighs == 3) {
            live = 1;
        } else {
            live = 0;
        }
    } else {
        if (alive_neighs == 3) {
            live = 1;
        } else {
            live = 0;
        }
    }
    return live;
}

void transform(int **matr, int *ptr) {
    for (int i = 0; i < HEIGHT; i++) {
        matr[i] = ptr + i * WIDTH;
    }
}

void copyMatrix(int **matr1, int **matr2) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matr2[i][j] = matr1[i][j];
        }
    }
}

int compareMatrix(int **matr1, int **matr2) {
    int equal_flag = 1;
    for (int i = 0; i < HEIGHT && equal_flag; i++) {
        for (int j = 0; j < WIDTH && equal_flag; j++) {
            if (matr1[i][j] != matr2[i][j]) {
                equal_flag = 0;
            }
        }
    }
    if (!equal_flag) {
        copyMatrix(matr1, matr2);
    }
    return equal_flag;
}

int updateSimilarity(int **matr1, int **matr2, int sim) {
    if (compareMatrix(matr1, matr2)) {
        sim += 1;
    } else {
        sim = 0;
    }
    return sim;
}

void startGame() {
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
}

void endGame() {
    printw("Game over. Press  enter.");
    while (getch() != '\n') {
        ;
    }
    curs_set(1);
    endwin();
}