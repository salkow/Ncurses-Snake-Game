#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <unistd.h>

#ifndef SNAKE_HPP
#define SNAKE_HPP


// Default values you can change.
// Snake and border char (255) are error characters but they look nice.
#define SNAKE_CHAR ((char)255)
#define BORDER_CHAR ((char)255)
#define FOOD_CHAR '*'
#define SPEED_START 50000
#define DIRECTION_START 'l'


class snakepart
{
public:
    snakepart(int col, int row);
    snakepart();

    int x, y;
};

class snakeclass
{
public:
    snakeclass();
    ~snakeclass();

    bool start();

private:
    int points, speed, maxwidth, maxheight;

    char direction, snake_char, border_char, food_char;

    bool ate_food;

    snakepart food;

    std::vector<snakepart> snake;

    void putfood();
    bool collision();
    void movesnake();
};

#endif // SNAKE_HPP
