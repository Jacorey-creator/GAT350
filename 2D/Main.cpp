#include <SDL.h>
#include "iostream"
#include "Renderer.h"

int main(int argc, char* argv[])
{
	printf ("Hello, World!");

	class Renderer r;
	
	// initialize SDL
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("Game Engine", 800, 600);

	return 0;
}