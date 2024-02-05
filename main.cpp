#include <iostream>
#include "snake.hpp"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <cstdlib>

#define ROWS 20 
#define COLUMNS 50 

char GRAPHICS[ROWS][COLUMNS]; 
bool IS_FOOD_ON_SCREEN = false;
int FOOD_X_POS = 0;
int FOOD_Y_POS = 0;

void refresh_graphics(void);
void print_graphics(void);
void put_pixel(int x, int y);
bool is_pos_valid(int x, int y);
void move_cursor(int n);
void rm_pixel(int x, int y);
void something();
void draw(std::vector<int> xs, std::vector<int> ys);
void draw_snake();
void start_game(Snake* s);
int get_input_from_user();
int get_random_x();
int get_random_y();
void spawn_food();
bool is_snake_get_food(Snake* s);

int main()
{
    //non blocking
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    struct termios attr;
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    attr = old;
    attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &attr);
    // random numb generator initialization
    srand((unsigned) time(0));
    // code start here
    refresh_graphics();
    Snake *s = new Snake(ROWS, COLUMNS);
    std::cout << std::endl;
    start_game(s);
    delete s;
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return 0;

}

void print_graphics(void)
{
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j  = 0; j < COLUMNS; j++) {
            std::cout << GRAPHICS[i][j];
        }
        std::cout << std::endl;
    }
}

bool is_pos_valid(int x, int y)
{
    if (x < 0 || x >= ROWS)
        return false;
    if (y < 0 || y >= COLUMNS)
        return false;
    return true;
}

void put_pixel(int x, int y)
{
    if (!is_pos_valid(x, y))
        return;
    GRAPHICS[x][y] = '#';
}

void refresh_graphics(void)
{
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j  = 0; j < COLUMNS; j++) {
            GRAPHICS[i][j] = ' ';
            if (i == 0 || j == 0)
                GRAPHICS[i][j] = '*';   
            if (i == ROWS - 1 || j == COLUMNS - 1)
                GRAPHICS[i][j] = '*';   
        }
    }
}

void move_cursor(int n)
{
    std::cout << "\033[20A"; 
}

void something(void)
{
    while (true) {
        put_pixel(10, 11);
        print_graphics();
        rm_pixel(10, 11);
        put_pixel(10, 10);
        move_cursor(20);
        print_graphics();
        rm_pixel(10, 10);
        move_cursor(20);
    }
}

void rm_pixel(int x, int y)
{
    if (!is_pos_valid(x, y))
        return;
    GRAPHICS[x][y] = '*';
}

void draw(std::vector<int> xs, std::vector<int> ys)
{
    int x, y;
    for (int i = 0; i < xs.size(); i++) {
        xs[i] = xs[i] % ROWS;
        ys[i] = ys[i] % COLUMNS;
        put_pixel(xs[i], ys[i]);
    }
}

void draw_snake(Snake* s)
{
    draw(s->get_xbodyparts(), s->get_ybodyparts());
}

int get_input_from_user(int* out)
{
    std::string input;
    char buf[20];
    ssize_t bytes;
    if ((bytes = read(STDIN_FILENO, buf, 20)) > 0) {
        input = buf[0] + '\0';
    } else {
        return -1;
    }
    if (input == "a")
        *out = Snake::LEFT;
    else if (input == "d")
        *out = Snake::RIGHT;
    else if (input == "s")
        *out = Snake::DOWN;
    else if (input == "w")
        *out = Snake::UP; 
    else
        return -1;
    return 0;
}

void start_game(Snake* s)
{
    int tmp, result;
    while (true) {
        refresh_graphics();
        draw_snake(s);
        if (s->is_hit_myself())
            return;
        if (!IS_FOOD_ON_SCREEN)
            spawn_food();
        else
            GRAPHICS[FOOD_X_POS][FOOD_Y_POS] = '#';
        if (is_snake_get_food(s)) {
            IS_FOOD_ON_SCREEN = false;
            s->eat(FOOD_X_POS, FOOD_Y_POS);
            continue;
        }
        print_graphics();
        usleep(150000);
        result = get_input_from_user(&tmp);
        if (result == 0) {
            s->change_direction(static_cast<Snake::Directions>(tmp));
        }
        s->move_snake();
        move_cursor(27);
    }
}

int get_random_x()
{
    int x = (rand()%ROWS) + 1;
    return x;
}

int get_random_y()
{
    int y = (rand()%COLUMNS) + 1;
    return y;
}

void spawn_food()
{
    int x, y;
    while (true) {
        x = get_random_x();
        y = get_random_y();
        if (GRAPHICS[x][y] != ' ')
            continue;
        GRAPHICS[x][y] = '#';
        FOOD_X_POS = x;
        FOOD_Y_POS = y;
        IS_FOOD_ON_SCREEN = true;
        return;
    }
}

bool is_snake_get_food(Snake* s)
{
    int x = s->get_next_x_pos();
    int y = s->get_next_y_pos(); 
    if (y == FOOD_Y_POS && x == FOOD_X_POS) {
        return true;
    }
    return false;
}

