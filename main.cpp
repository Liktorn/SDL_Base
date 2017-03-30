#include <SDL.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Screen.h"


using namespace bw;

int main(int argc, char ** argv)
{
	srand((unsigned int)time(NULL));

	// Create the screen
	Screen screen;
	if (screen.init() == false)
	{
		std::cout << "Error initialising SDL." << std::endl;
	}

//	Swarm swarm;

	// main loop
	while (true)
	{	
		//returns the ticks since the loop started to elapsed
		int elapsed = SDL_GetTicks();



		// Draw the screen
		screen.update();
		// check and see wether to continue the loop
		if (screen.processEvents() == false)
			break;
	}
	//destroy all windows, renderers, textures etc...
	screen.close();

	return 0;
}