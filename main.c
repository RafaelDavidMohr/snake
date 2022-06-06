#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef int point[2];

typedef struct {
    int length;
    int capacity;
    point *coordinates;
    int direction;
} snake_type;

const char *SNAKE_SEG = "o";
const size_t POINT_SIZE = sizeof(int) * 2;
const point UP = { -1, 0 };
const point DOWN = { 1, 0 };
const point LEFT = { 0, -1 };
const point RIGHT = { 0, 1 };

void render_snake(WINDOW **buffer, snake_type snake);
void unrender_snake(WINDOW **snake_segs, unsigned int length);
void destroy_snake_segment(WINDOW *snake_seg);
bool move_snake(snake_type *snake, const int direction);

int main(void) {
    int start_x;
    int start_y;
    int length;
    point *coordinates;
    snake_type snake;
    WINDOW **snake_segments;
    WINDOW *new_wind;
    int user_key;
    
    // ncurses init stuff
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    refresh();
    
    start_x = 10;
    start_y = 10;

    length = 3;
    coordinates = (point*) malloc(sizeof(point) * length);
    coordinates[0][0] = start_y, coordinates[0][1] = start_x;
    coordinates[1][0] = start_y, coordinates[1][1] = start_x + 2;
    coordinates[2][0] = start_y, coordinates[2][1] = start_x + 4;
    snake.length = length;
    snake.coordinates = coordinates;
    snake.direction = KEY_LEFT;

    snake_segments = (WINDOW**) malloc(sizeof(WINDOW*) * snake.length);

    render_snake(snake_segments, snake);

    while(true) {
	user_key = getch();
	if(move_snake(&snake, user_key)) {
	    unrender_snake(snake_segments, snake.length);
	    render_snake(snake_segments, snake);
	}
	if(user_key == KEY_BACKSPACE) {
	    break;
	}
    }

    free(snake_segments);
    endwin();
    return EXIT_SUCCESS;
}

void render_snake(WINDOW **buffer, snake_type snake) {
    point coordinates;
    WINDOW *snake_segment_window;
    
    for(int i = 0; i < snake.length; i++) {
	coordinates[0] = snake.coordinates[i][0];
	coordinates[1] = snake.coordinates[i][1];
	
	snake_segment_window = newwin(1, 1, coordinates[0], coordinates[1]);
	wprintw(snake_segment_window, SNAKE_SEG);
	wrefresh(snake_segment_window);
        buffer[i] = snake_segment_window;
    }    
}

void unrender_snake(WINDOW **snake_segments, unsigned int length) {
    for(int i = 0; i < length; i++) {
	werase(snake_segments[i]);
	wrefresh(snake_segments[i]);
	delwin(snake_segments[i]);
    }
}

bool move_snake(snake_type *snake, const int direction) {

    point direction_point;
    if(direction == KEY_UP) {
	if(snake->direction == KEY_DOWN) {
	    return false;
	}
        memcpy(direction_point, UP, POINT_SIZE);
        snake->direction =  KEY_UP;
    } else if(direction == KEY_DOWN) {
	if(snake->direction == KEY_UP) {
	    return false;
	}
        memcpy(direction_point, DOWN, POINT_SIZE);
	snake->direction =  KEY_DOWN;
    } else if(direction == KEY_LEFT) {
	if(snake->direction == KEY_RIGHT) {
	    return false;
	}
        memcpy(direction_point, LEFT, POINT_SIZE);
	snake->direction =  KEY_LEFT;	
    } else if(direction == KEY_RIGHT) {
	if(snake->direction == KEY_LEFT) {
	    return false;
	}
        memcpy(direction_point, RIGHT, POINT_SIZE);
	snake->direction =  KEY_RIGHT;	
    } else {
	return false;
    }
    
    point old_location = { snake->coordinates[0][0], snake->coordinates[0][1] };
    point old_location_2 = { snake->coordinates[0][0], snake->coordinates[0][1] };
    snake->coordinates[0][0] += direction_point[0];
    snake->coordinates[0][1] +=  2 * direction_point[1];
    
    for(int i = 1; i < snake->length; i++) {
	memcpy(old_location_2, snake->coordinates[i], POINT_SIZE);
	snake->coordinates[i][0] = old_location[0];
	snake->coordinates[i][1] = old_location[1];
	memcpy(old_location, old_location_2, POINT_SIZE);
    }
    return true;
}
