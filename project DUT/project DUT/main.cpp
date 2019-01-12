#include "game.h"

int main()
{
	Game game;

	if (game.init())
		return (game.loop());
	return (1);
}