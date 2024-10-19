#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "PostProcess.h"
#include "MathUtils.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	std::string img = "C:/Users/jrowe/source/repos/GAT350/Build/Photos/";
	class Renderer r;
	Camera camera(r.m_width, r.m_height);
	camera.SetProjection(60.0f, 800.0f / 600, 0.1f, 200.0f);
	Transform cameraTransform{ { 0, 0, -20 } };
	
	// initialize SDL
	Time time;
	Input input;

	input.Initialize();
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
	Transform transform{ {0 , 0, 0 }, glm::vec3{ 0, 0, 45 }, glm::vec3 { 3 } };
	Transform potTransform{ {0 , 0, 0 }, glm::vec3{ 15, 0, 180 }, glm::vec3 { 8 } };
	Model teapot;
	Model slug;
	teapot.Load("C:/Users/jrowe/source/repos/GAT350/Build/teapot.obj");
	slug.Load("C:/Users/jrowe/source/repos/GAT350/Build/Nightmare_Fiend.obj");
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
		//SetBlendMode(BlendMode::Normal);	
		//framebuffer.DrawImage(100, 100, image);	
		////SetBlendMode(BlendMode::Alpha);	
		//framebuffer.DrawImage(50, 100, alpha_image);
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

#pragma region MOVE_TRIANGLE
		glm::vec3 direction{ 0 };
		if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1;
		if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
		if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = 1;
		if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = -1;
		if (input.GetKeyDown(SDL_SCANCODE_W)) direction.y = 1;
		if (input.GetKeyDown(SDL_SCANCODE_S)) direction.y = -1;

		cameraTransform.position += direction * 70.0f * time.GetDeltaTime();
		camera.SetView(cameraTransform.position, cameraTransform.position + glm::vec3{ 0, 0, 1 });

		transform.rotation.z += 90 * time.GetDeltaTime();

		//model.Draw(framebuffer, transform.GetMatrix(), camera);

#pragma endregion

		teapot.Draw(framebuffer, potTransform.GetMatrix(), camera);
		teapot.SetColor({ 128, 77, 178, 255 });

		slug.Draw(framebuffer, transform.GetMatrix(), camera);
		slug.SetColor({ 0, 0, 255, 255 });

		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}
	return 0;
}