#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct coords {
    int x;
    int y;
} coords;


#define CELL 'o'
#define FOOD 'X'

#define LIM_X 24
#define LIM_Y 80
#define MAXLEN (LIM_X * LIM_Y)

#define STARTLEN 5
#define STARTX 10
#define STARTY 10

coords snake[MAXLEN];
int snake_len = 0;
int start = 0;
int end = 0;

int t_val = 100;

coords food;

coords directions[] = {
    (coords) {0, 1}, /* -> */
    (coords) {1, 0}, /* v */
    (coords) {0, -1},/* <- */
    (coords) {-1, 0} /* ^ */
};
int dir = 0;

#define LEFT 42
#define RIGHT 43


void change_dir(int to)
{
    if (to == LEFT) {
        if (dir == 0) {
            dir = 3;
        } else {
            dir -= 1;
        }
    } else {
        if (dir == 3) {
            dir = 0;
        } else {
            dir += 1;
        }
    }
}


void put_char(int x, int y, char c)
{
    mvaddch(x, y, c);
}

void clear_screen() {
    erase();
}

void render() {
    refresh();
}

void put_string(int x, int y, char *s)
{
    mvprintw(x, y, s);
}


int main()
{
    int quit = 0;
    int ch;

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(t_val);

    srand(time(NULL));
    food.x = rand() % LIM_X + 1;
    food.y = rand() % LIM_Y + 1;

    FILE *log = fopen("log.txt", "w");

    coords head;

    /* initial snake */
    int i;
    int x = STARTX;
    int y = STARTY;
    for (i = 0; i < STARTLEN; ++i) {
        snake[i] = (coords) {x, y};
        ++y;
        ++end;
    }

    while (!quit) {
    	ch = getch();
    	switch(ch) {
            case KEY_LEFT:
                    change_dir(LEFT);
                    break;
            case KEY_RIGHT:
                    change_dir(RIGHT);
                    break;
            case 'q':
                    quit = true;
                    break;
            case '1':
                    t_val = 300;
                    break;
            case '2':
                    t_val = 100;
                    break;
            case '3':
                    t_val = 50;
                    break;
            default:
                    break;
    	}

        /* print the snake */
        clear_screen();
        for (i = start; i < end; ++i) {
            put_char(snake[i].x, snake[i].y, CELL);
        }
        /* and the food */
        put_char(food.x, food.y, FOOD);

        char buf[100];
        sprintf(buf, "head at: %d %d", head.x, head.y);
        put_string(0, 0, buf);
        sprintf(buf, "food at: %d %d", food.x, food.y);
        put_string(1, 0, buf);

        render();

        head = snake[end - 1];
        snake[end] = (coords) {head.x + directions[dir].x,
                               head.y + directions[dir].y};

        /* got food? */
        if (head.x == food.x && head.y == food.y) {
            food.x = rand() % LIM_X + 1;
            food.y = rand() % LIM_Y + 1;
        } else {
            ++start;
        }
        ++end;
    }

    fclose(log);

    return 0;
}


