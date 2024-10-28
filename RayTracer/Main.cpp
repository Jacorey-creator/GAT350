#include "SDL.h"
#include "iostream"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "Actor.h"
#include "Scene.h"
#include "Random.h"
#include "Tracer.h"
#include "Plane.h"
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
	srand((unsigned int)time(NULL));

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
	
	Scene scene;

	// materials
	std::array<std::shared_ptr<Material>,3> palette;
	std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
	std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	std::shared_ptr<Material> plane_material = std::make_shared<Lambertian>(color3_t{ 0,1,1 });

	palette[0] = gray;
	palette[1] = red;
	palette[2] = blue;

	// spheres
	for (int i = 0; i < 10; i++)
	{
		auto spheres = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(2.0f, 5.0f), palette.at(random(3)));
		scene.AddObject(std::move(spheres));
	}

	// plane
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -1, 0 }, glm::vec3{ 0.5f, 0.5f, 0 }, plane_material);
	scene.AddObject(std::move(plane));


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
		
		scene.Render(framebuffer, cam);

		framebuffer.Update();

		r.CopyFrameBuffer(framebuffer);
		//show screen
		SDL_RenderPresent(r.m_renderer);
	
	}
	return 0;
}