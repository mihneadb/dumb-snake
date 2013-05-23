/*
** main.c
**
** Nokia 3310 Example Program
** Target: ATMEGA :: AVR-GCC
**
** Written by Tony Myatt - 2007
** Quantum Torque - www.quantumtorque.com
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "lcd.h"

typedef struct coords {
    int x;
    int y;
} coords;


#define CELL 'o'
#define FOOD 'X'

#define LIM_X 24
#define LIM_Y 80
#define MIN_X 2
#define MIN_Y 0
#define MAXLEN (LIM_X * LIM_Y)

#define STARTLEN 5
#define STARTX 10
#define STARTY 10

#define CICLIC 1
#define WALLS 2

coords snake[MAXLEN];
int snake_len = 0;
int start = 0;
int end = 0;

int mode = WALLS;
int quit = 0;

int t_val = 100;
int score = 0;

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
	lcd_goto_xy(x, y);
	lcd_chr(c);
}

void clear_screen() {
    lcd_clear();
}

void render() {
	// nothing
}

void put_string(int x, int y, char *s)
{
	lcd_goto_xy(x, y);
	lcd_str(s);
}

void draw_wall() {
    int i;
    for (i = 0; i < LIM_Y; ++i) {
        put_char(MIN_X, i, 'x');
        put_char(LIM_X - 1, i, 'x');
        put_char(i + 2, MIN_Y, 'x');
        put_char(i + 2, LIM_Y - 1, 'x');
    }
}

void lost() {
    char buf[100];
    sprintf(buf, "YOU LOST! SCORE: %d.", score);
    put_string(10, 10, buf);
    quit = 1;
}


int main()
{
    int ch;

	// Setup LCD
	lcd_init();
	lcd_contrast(0x40);

    srand(time(NULL));
    food.x = rand() % LIM_X + 1;
    food.y = rand() % LIM_Y + 1;

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
                    quit = 1;
                    break;
            case '1':
                    t_val = 100;
                    mode = CICLIC;
                    break;
            case '2':
                    t_val = 100;
                    mode = WALLS;
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
		// maybe _delay ms here?

        head = snake[end - 1];

        if (mode == CICLIC) {
            if (head.x + directions[dir].x < MIN_X) {
                head.x = LIM_X;
            } else if (head.x + directions[dir].x >= LIM_X) {
                head.x = MIN_X;
            }
            if (head.y + directions[dir].y < MIN_Y) {
                head.y = LIM_Y;
            } else if (head.y + directions[dir].y >= LIM_Y) {
                head.y = MIN_Y;
            }
        } else {
            draw_wall();
            if (head.x + directions[dir].x < MIN_X) {
                lost();
            } else if (head.x + directions[dir].x >= LIM_X) {
                lost();
            }
            if (head.y + directions[dir].y < MIN_Y) {
                lost();
            } else if (head.y + directions[dir].y >= LIM_Y) {
                lost();
            }
        }

        snake[end] = (coords) {head.x + directions[dir].x,
                               head.y + directions[dir].y};

        /* got food? */
        if (head.x == food.x && head.y == food.y) {
            food.x = rand() % LIM_X + 1;
            food.y = rand() % LIM_Y + 1;
            ++score;
        } else {
            ++start;
        }
        ++end;
		// maybe _delay ms here?
    }
    render();
    sleep(1);
	// maybe _delay ms here?

    return 0;
}


/* main -- Program starts here */
int main(void)
{	
}
