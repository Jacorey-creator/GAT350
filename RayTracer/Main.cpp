#include "SDL.h"
#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "Actor.h"
#include "Random.h"
#include "Tracer.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Camera.h"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	std::string img = "C:/Users/jrowe/source/repos/GAT350/Build/Photos/";
	class Renderer r;
	
	// initialize SDL
	Time time;
	Input input;

	input.Initialize();
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("RayTracer", 800, 600);

	SetBlendMode(BlendMode::Normal);

	Framebuffer framebuffer(r, r.m_width, r.m_height);

	Camera cam{ 70.0f, framebuffer.m_width / (float)framebuffer.m_height };
	cam.SetView({ 0, 0, -20 }, { 0, 0, 0 });
	
	bool quit = false;
	while (!quit) //main loop
	{
		time.Tick();
		input.Update();
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
		framebuffer.Clear(ColorConvert(color4_t{ 0, 0.25f, 0, 1 }));
		
		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}
	return 0;
}