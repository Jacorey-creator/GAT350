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

	Framebuffer framebuffer(r, 800, 600);
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
			int x = rand() % 800;
			int y = rand() % 600;
			
			int x2 = rand() % 800;
			int y2 = rand() % 600;
			//framebuffer.DrawPoint(x, y, color_t{ 0, 255, 0, 255 });
			framebuffer.DrawLine(x,y,x2, y2, color_t{ 0, 255, 0, 255 });
		}

		framebuffer.DrawRect(115, 50, 100, 30, color_t{ 0, 0, 255 });
		framebuffer.DrawTriangle(400, 213, 300, 386, 500, 386, color_t{ 255, 0, 100 });
		framebuffer.DrawCircle(25, 100, 200, color_t{ 0, 0, 255 });
		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}

	return 0;
}