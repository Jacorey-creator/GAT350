#include <SDL.h>
#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"

int main(int argc, char* argv[])
{
	printf ("Hello, World!");

	class Renderer r;
	
	// initialize SDL
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("Game Engine", 800, 600);

	Framebuffer framebuffer(r, 200, 150);
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
	
		//clear screen
		framebuffer.Clear(color_t{ 0,0,0,255 });
		for (int i = 0; i < 100; i++)
		{
			int x = rand() % 200;
			int y = rand() % 150;
			framebuffer.DrawPoint(x, y, color_t{ 0, 255, 0, 255 });
		}

		framebuffer.DrawRect(10, 100, 100, 30, color_t{ 0, 0, 255 });
		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}

	return 0;
}