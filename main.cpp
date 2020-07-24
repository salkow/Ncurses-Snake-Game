#include "snake.hpp"

int main()
{
	bool again = true;

	// Loop while the users wants to play again.
	while (again)
	{
		snakeclass s;

		again = s.start();
	}

	return 0;
}
