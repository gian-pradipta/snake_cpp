#include <iostream>
#include "snake.hpp"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <random>

#define ROWS 20 
#define COLUMNS 50 

char GRAPHICS[ROWS][COLUMNS]; 

void refresh_graphics(void);
void print_graphics(void);
void put_pixel(int x, int y);
bool is_pos_valid(int x, int y);
void move_cursor(int n);
void rm_pixel(int x, int y);
void something();
void draw(std::vector<int> xs, std::vector<int> ys);
void draw_snake();
void moving_snake(Snake* s);
int get_input_from_user();
int get_next_food_pos();

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
    // code start here
    refresh_graphics();
    Snake *s = new Snake(ROWS, COLUMNS);
    std::cout << std::endl;
    moving_snake(s);
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

void moving_snake(Snake* s)
{
    int tmp, result;
    while (true) {
        refresh_graphics();
        draw_snake(s);
        print_graphics();
        usleep(200000);
        result = get_input_from_user(&tmp);
        if (result == 0) {
            s->change_direction(static_cast<Snake::Directions>(tmp));
        }
        s->move_snake();
        move_cursor(27);
    }
}