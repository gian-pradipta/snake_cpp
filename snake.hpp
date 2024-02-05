#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
class Snake
{    
public:
    typedef  enum direction {
        UP,
        LEFT,
        DOWN,
        RIGHT
    } Directions;
    Snake(int xOffset, int yOffset);
    ~Snake();
    void eat(int foodPosX, int foodPosY);
    void move_snake();
    void sayHello();
    void print_all_pos();
    std::vector<int> get_xbodyparts();
    int change_direction(enum direction new_direction);
    std::vector<int> get_ybodyparts();
    
private:
    int xOffset, yOffset;
    enum direction direction;
    int get_next_y_pos();
    int get_next_x_pos();
    std::vector<int> xbodyparts;
    std::vector<int> ybodyparts;
};

#endif
