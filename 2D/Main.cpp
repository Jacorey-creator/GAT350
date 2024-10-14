#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "MathUtils.h"

int main(int argc, char* argv[])
{
	std::string img = "C:/Users/jrowe/source/repos/GAT350/Build/Photos/";
	class Renderer r;
	
	// initialize SDL
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("Game Engine", 800, 600);

	Framebuffer framebuffer(r, 800, 600);
	Image image;
	image.Load(img + "Scenic.jpg");
	
	Image alpha_image;
	alpha_image.Load(img + "colors.png");
	//PostProcess::Alpha(alpha_image.m_buffer, 128);


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
		}
		SetBlendMode(BlendMode::Normal);
		framebuffer.DrawImage(100, 100, image);
		//SetBlendMode(BlendMode::Alpha);
		//framebuffer.DrawImage(50, 100, alpha_image);
		
		
#pragma region POST_PROCESS 
		//PostProcess::Invert(framebuffer.m_buffer);
		//PostProcess::Monochrome(framebuffer.m_buffer);
		//PostProcess::Brightness(framebuffer.m_buffer, -50);
		//PostProcess::ColorBalance(framebuffer.m_buffer, 50, 200, 0);
		//PostProcess::Noise(framebuffer.m_buffer, 80);
		//PostProcess::Threshold(framebuffer.m_buffer, 100);
		//PostProcess::Posterize(framebuffer.m_buffer, 3);
		//PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::GaussianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 3);
		//PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);

		#pragma endregion

		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}

	return 0;
}