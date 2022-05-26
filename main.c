#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

const char *snake_seg = "o";

typedef unsigned int point[2];

typedef struct {
    unsigned int length;
    point *coordinates;
} snake_type;

WINDOW **render_snake(snake_type snake);
WINDOW *snake_segment(int y, int x);
void destroy_snake_segment(WINDOW *snake_seg);

int main(void) {
    int start_x;
    int start_y;
    unsigned int length;
    point *coordinates;
    snake_type snake;
    WINDOW **snake_segments;
    WINDOW *new_wind;
    
    // ncurses init stuff
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
    refresh();
    
    start_x = 10;
    start_y = 10;

    length = 3;
    coordinates = (point*) malloc(sizeof(point) * length);
    coordinates[0][0] = start_y, coordinates[0][1] = start_x;
    coordinates[1][0] = start_y, coordinates[1][1] = start_x + 2;
    coordinates[2][0] = start_y + 1, coordinates[2][1] = start_x;
    snake.length = length;
    snake.coordinates = coordinates;

    snake_segments = render_snake(snake);
    
    while(getch() == ERR) {
	continue;
    }
    
    endwin();
    return EXIT_SUCCESS;
}

WINDOW *snake_segment(int y, int x) {

    WINDOW *snake_segment_window;

    snake_segment_window = newwin(1, 1, y, x);
    wprintw(snake_segment_window, snake_seg);
    wrefresh(snake_segment_window);

    return snake_segment_window;
}

void destroy_snake_segment(WINDOW *snake_seg) {

    delwin(snake_seg);
}

WINDOW **render_snake(snake_type snake) {
    point coordinates;
    WINDOW **snake_windows = (WINDOW**) malloc(sizeof(WINDOW*) * snake.length);

    for(unsigned int i = 0; i < snake.length; i++) {
	coordinates[0] = snake.coordinates[i][0];
	coordinates[1] = snake.coordinates[i][1];
	snake_windows[i] = snake_segment(coordinates[0], coordinates[1]);
    }
    
    return snake_windows;
}
