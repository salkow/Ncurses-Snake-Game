#include "snake.hpp"
#include "string.h"

snakepart::snakepart(int col, int row)
	: x(col), y(row)
{}

snakepart::snakepart()
	: x(0), y(0)
{}

snakeclass::snakeclass()
{
	initscr();  // Initialize ncurses.
	nodelay(stdscr, true);  // The getch doesn't wait for the user to press a key.
	keypad(stdscr, true);  // Enable the keypad.
	noecho();  // Doesn't write the typed characters into the screen.
	curs_set(0);  // Hide the cursor;
	getmaxyx(stdscr, maxheight, maxwidth);  // Get dimensions of the terminal.

	snake_char = SNAKE_CHAR;
	border_char = BORDER_CHAR;
	food_char = FOOD_CHAR;

	for (int i = 0; i < 5; i++)
	{
		snake.push_back(snakepart(40 + i, 10));
	}

	points = 0;
	speed = SPEED_START;  // The snake's starting speed.
	ate_food = false;
	direction = DIRECTION_START;  // The direction which the snake faces in the start.

	srand(time(NULL));

	putfood();

	// Draw borders.
	for (int i = 0; i < maxwidth - 1; i++)
	{
		move(0, i);
		addch(border_char);
	}

	for (int i = 0; i < maxwidth - 1; i++)
	{
		move(maxheight - 2, i);
		addch(border_char);
	}

	for (int i = 0; i < maxheight - 1; i++)
	{
		move(i, 0);
		addch(border_char);
	}

	for (int i = 0; i < maxheight - 1; i++)
	{
		move(i, maxwidth - 1);
		addch(border_char);
	}

	// Draw snake.
	for (int i = 0; i < (int)snake.size(); i++)
	{
		move(snake[i].y, snake[i].x);
		addch(snake_char);
	}

	move(maxheight - 1, 0);
	printw("%d", points);
}

snakeclass::~snakeclass()
{
	nodelay(stdscr, false);
	clear();
	endwin();
}

void snakeclass::putfood()
{
	// Loop until food location if a valid value.
	while (1)
	{
		// Get a random value for the location of the food,
		// which is inside the borders.
		int tmpx = (rand() % (maxwidth - 2)) + 1;
		int tmpy = (rand() % (maxheight - 3)) + 1;

		bool food_in_snake = false;

		// We don't want the food to appear inside the snake.
		for (int i = 0; i < (int)snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				food_in_snake = true;
				break;
			}
		}

		if (food_in_snake)
		{
			continue;
		}

		food.x = tmpx;
		food.y = tmpy;

		break;
	}

	move (food.y, food.x);
	addch(food_char);
	refresh();
}

bool snakeclass::collision()
{
	// Snake has collided with the edge.
	if (snake[0].x == 0 || snake[0].x == maxwidth - 1 ||
		snake[0].y == 0 || snake[0].y == maxheight - 2)
	{
		return true;
	}

	// Snake has collided with itself.
	for (int i = 2; i < (int)snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			return true;
		}
	}

	// Snake has collided with the food.
	if (snake[0].x == food.x && snake[0].y == food.y)
	{
		ate_food = true;
		putfood();

		points += 10;

		move(maxheight - 1, 0);
		printw("%d", points);

		// Get faster if the score gets high.
		if (points % 100 == 0)
		{
			speed -= 10000;
		}
	}

	else
	{
		ate_food = false;
	}

	return false;
}

void snakeclass::movesnake()
{
	int key = getch();

	// Get direction from user's pressed key.
	if ((key == 'h' || key == KEY_LEFT) && direction != 'r')
	{
		direction = 'l';
	}

	else if ((key == 'j' || key == KEY_DOWN) && direction != 'u')
	{
		direction = 'd';
	}

	else if ((key == 'k'|| key == KEY_UP) && direction != 'd')
	{
		direction = 'u';
	}

	else if ((key == 'l'|| key == KEY_RIGHT) && direction != 'l')
	{
		direction = 'r';
	}

	else if (key == 'q')
	{
		direction = 'q';
	}

	else if (key == 'r')
	{
		direction = 'a';
	}

	else if (key == 'p')
	{
		nodelay(stdscr, false); // The getch doesn't wait for the user to press a key.
		getchar();
		nodelay(stdscr, true);
	}

	// If the snake didn't eat the food,
	// delete its last block.
	if (!ate_food)
	{
		move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);

		addch(' ');
		refresh();

		snake.pop_back();
	}

	// Draw the next block in the direction of the snake.
	switch(direction)
	{
	case 'l':
		snake.insert(snake.begin(), snakepart(snake[0].x - 1, snake[0].y));
		break;

	case 'd':
		snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y + 1));
		break;

	case 'u':
		snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y - 1));
		break;

	case 'r':
		snake.insert(snake.begin(), snakepart(snake[0].x + 1, snake[0].y));
		break;
	}

	// Add a new block for the head of the snake.
	move(snake[0].y, snake[0].x);
	addch(snake_char);
	refresh();
}

bool snakeclass::start()
{
	while (1)
	{
		// If the snake collided with itself or one of the edges.
		if (collision())
		{
			move(maxheight / 2 - 5, maxwidth / 2);
			printw("Game over!");
			refresh();

			nodelay(stdscr, false);
			char ch = getchar();

			if (ch == 'q')
			{
				break;
			}

			return true;
		}

		movesnake();

		// Quit.
		if (direction == 'q')
		{
			break;
		}

		// Restart.
		else if (direction == 'a')
		{
			return true;
		}

		// Sleep for a small amount of time.
		// When going up or down, sleep for more cause terminal is weird.
		if (direction == 'u' || direction == 'd')
		{
			usleep(speed * 1.35);
		}

		else
		{
			usleep(speed);
		}
	}

	return false;
}
