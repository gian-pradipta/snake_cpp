#include <vector>
#include <cmath>
#include <iostream>
#include "snake.hpp"


Snake::Snake(int xOffset, int yOffset)
{
    this->yOffset = yOffset - 1;
    this->xOffset = xOffset - 1;
    this->direction = DOWN;
    this->xbodyparts.push_back(2);
    this->ybodyparts.push_back(1);
    this->xbodyparts.push_back(2);
    this->ybodyparts.push_back(2);
    this->xbodyparts.push_back(2);
    this->ybodyparts.push_back(3);
    this->xbodyparts.push_back(2);
    this->ybodyparts.push_back(4);
}
Snake::~Snake()
{
    std::cout << "Game Over\n";
}

int Snake::get_next_y_pos()
{
    int y = this->ybodyparts[0];
    if (this->direction == RIGHT) {
        if (y + 1 >= this->yOffset) {
            y = 1;
            return y;
        }
        if (y + 1 < 1) {
            y = yOffset - 1;
            return y;
        }
        return ++y;
    }
    if (this->direction == LEFT) {
        if (y -  1 >= this->yOffset) {
            y = 1;
            return y;
        }
        if (y - 1 < 1) {
            y = yOffset - 1;
            return y;
        }
        return --y;
    }
    return y;
        
}

int Snake::get_next_x_pos()
{
    int x = this->xbodyparts[0];
    if (this->direction == DOWN) {
        if (x + 1 >= this->xOffset) {
            x = 1;
            return x;
        }
        if (x + 1 < 1) {
            x = xOffset - 1;
            return x;
        }
        return ++x;
    }
    if (this->direction == UP) {
        if (x - 1 >= this->xOffset) {
            x = 1;
            return x;
        }
        if (x - 1 < 1) {
            x = xOffset - 1;
            return x;
        }
        return --x;
    }
    return x;
}


void Snake::move_snake()
{
    int nextY = this->get_next_y_pos();
    int nextX = this->get_next_x_pos();
    this->xbodyparts.pop_back();
    this->xbodyparts.insert(this->xbodyparts.begin(), nextX);
    this->ybodyparts.pop_back();
    this->ybodyparts.insert(this->ybodyparts.begin(), nextY);
}


void Snake::print_all_pos()
{
    for (int i = 0; i < this->xbodyparts.size(); i++) {
        std::cout << this->xbodyparts[i] << ", " << this->ybodyparts[i] << std::endl;
    }
}

std::vector<int> Snake::get_xbodyparts()
{
    return this->xbodyparts;
}

std::vector<int> Snake::get_ybodyparts()
{
    return this->ybodyparts;
}


int Snake::change_direction(enum direction new_direction)
{
    char old_direction = this->direction;
    int diff = abs(old_direction - new_direction);
    if (diff == 2)
        return -1;
    this->direction = new_direction;
    return 0;
}

void Snake::eat(int foodPosX, int foodPosY)
{
    xbodyparts.insert(xbodyparts.begin(), foodPosX);
    ybodyparts.insert(ybodyparts.begin(), foodPosY);
}

bool Snake::is_hit_myself()
{
    int headX, headY;
    headX = this->xbodyparts[0];
    headY = this->ybodyparts[0];
    for (int i = 1; i < this->xbodyparts.size(); i++) {
        if (headX == this->xbodyparts[i] && headY == this->ybodyparts[i])
            return true;
    }
    return false;
}