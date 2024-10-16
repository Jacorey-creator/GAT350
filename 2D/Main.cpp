#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	std::string img = "C:/Users/jrowe/source/repos/GAT350/Build/Photos/";
	class Renderer r;
	
	// initialize SDL
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("Game Engine", 800, 600);

	SetBlendMode(BlendMode::Normal);

	Framebuffer framebuffer(r, 800, 600);
	Image image;
	image.Load(img + "Scenic.jpg");
	
	Image alpha_image;
	alpha_image.Load(img + "colors.png");
	PostProcess::Alpha(alpha_image.m_buffer, 128);

	

	verticies_t verticies = { { -5, 5, 0},
							  { 5, 5, 0 }, 
							  {-5, -5, 0 } };

	Model model(verticies, { 0, 255, 0, 255 });

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
#pragma region ALPHA_BLEND
		SetBlendMode(BlendMode::Normal);	
		framebuffer.DrawImage(100, 100, image);	
		//SetBlendMode(BlendMode::Alpha);	
		framebuffer.DrawImage(50, 100, alpha_image);
#pragma endregion	
		
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

		int ticks = SDL_GetTicks(); 
		float time = ticks * 0.001f;

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(240.0f, 240.0f, 0.0f));
		glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(5));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(time * 90), glm::vec3{ 0, 1, 0});
		modelMatrix = translate * scale * rotate;

		//model.Draw(framebuffer, modelMatrix);
		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}

	return 0;
}