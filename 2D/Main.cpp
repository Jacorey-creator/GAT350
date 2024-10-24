#include "SDL.h"
#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "Actor.h"
#include "Random.h"
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
	Camera camera(r.m_width, r.m_height);
	camera.SetProjection(60.0f, 800.0f / 600, 0.1f, 200.0f);
	Transform cameraTransform{ { 0, 0, -100 } };
	
	// initialize SDL
	Time time;
	Input input;

	input.Initialize();
	r.Init_SDL();

	// create window
	// returns pointer to window if successful or nullptr if failed
	r.Create_Window("Game Engine", 800, 600);

	//SetBlendMode(BlendMode::Normal);

	Framebuffer framebuffer(r, 800, 600);
	Image image;
	image.Load(img + "Scenic.jpg");
	
	Image alpha_image;
	alpha_image.Load(img + "colors.png");
	PostProcess::Alpha(alpha_image.m_buffer, 128);

	

	verticies_t verticies = { { -5, 5, 0},
							  { 5, 5, 0 }, 
							  {-5, -5, 0 } };

	//Model model(verticies, { 0, 255, 0, 255 });
	std::shared_ptr<Model> model = std::make_shared<Model>();
	
	model->Load("C:/Users/jrowe/source/repos/GAT350/Build/cube.obj");

	std::vector<std::unique_ptr<Actor>> actors;

	Transform transform{ {0 , 0, 0 }, glm::vec3{ 0, 0, 0 }, glm::vec3 { 2 } };
	Transform potTransform{ {0 , 0, 0 }, glm::vec3{ 15, 0, 180 }, glm::vec3 { 8 } };
	Actor actor(transform, model);

	Model teapot;
	Model slug;
	
	for (int i = 0; i < 20; i++)
	{
		Transform transform{ { randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f)}, glm::vec3{0, 0, 0}, glm::vec3{ randomf(2, 20) }};
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
		actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
		actors.push_back(std::move(actor));
	}

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
	
#pragma region ALPHA_BLEND
		SetBlendMode(BlendMode::Normal);	
		framebuffer.DrawImage(100, 100, image);	
		SetBlendMode(BlendMode::Alpha);	
		//framebuffer.DrawImage(50, 100, alpha_image);
#pragma endregion	

#pragma region POST_PROCESS 
		PostProcess::Invert(framebuffer.m_buffer);
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

#pragma region MOVE_CAM
		if (input.GetMouseButtonDown(2))
		{
			input.SetRealativeMode(true);

			glm::vec3 direction{ 0 };
			if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1;
			if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
			if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = 1;
			if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = -1;
			if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
			if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;

			cameraTransform.rotation.y = input.GetMousePosition().x * 0.25f;
			cameraTransform.rotation.x = input.GetMousePosition().y * 0.25f;

			glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

			cameraTransform.position += offset * 70.0f * time.GetDeltaTime();

			//transform.rotation.z += 90 * time.GetDeltaTime();
		}
		else 
		{
			input.SetRealativeMode(false);
		}
		
		camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());
		


#pragma endregion

#pragma region MODELS
		teapot.Draw(framebuffer, potTransform.GetMatrix(), camera);
		teapot.SetColor({ 128, 77, 178, 255 });

		//slug.Draw(framebuffer, potTransform.GetMatrix(), camera);
		//slug.SetColor({ 0, 0, 255, 255 });

		//model.Draw(framebuffer, transform.GetMatrix(), camera);
		//actor.Draw(framebuffer, camera);
		for (auto& actor : actors)
		{
			//actor->SetColor({0, 0, 255, 255});
			actor->Draw(framebuffer, camera);
		}
#pragma endregion
		
		//framebuffer.DrawImage(50, 100, image);
		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}
	return 0;
}