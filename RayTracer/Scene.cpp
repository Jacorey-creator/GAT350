#include "Scene.h"
#include "Framebuffer.h"
#include "Ray.h"
#include "ETime.h"
#include "Camera.h"
#include "Tracer.h"
#include "Random.h"
#include <iostream>


void Scene::Update()
{
	for (auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples, int depth)
{
	Time frameTimer;
	Time scanlineTimer;

	for (int y = 0; y < framebuffer.m_height; y++)
	{
		scanlineTimer.Reset();
		for (int x = 0; x < framebuffer.m_width; x++)
		{
			color3_t color{ 0 };
			// multi-sample for each pixel
			for (int s = 0; s < numSamples; s++)
			{
				glm::vec2 pixel{ x, y };
				// add a random value to the pixel (0-1)
				pixel += glm::vec2{ randomf(0,1), randomf(0,1) };
				// normalize the pixel value
				glm::vec2 point = pixel / glm::vec2{ framebuffer.m_width, framebuffer.m_height };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1 - point.y;

				ray_t ray = camera.GetRay(point);

				// accumulate colors from tracer
				color += Tracer::Trace(*this,ray, 0.001f, 100, depth);
			}
			// average the color
			color = color / (float)numSamples;
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
		std::cout << "y: " << y << " : " << scanlineTimer.GetElapsedTime() << std::endl;
	}
	std::cout << "frame time: " << frameTimer.GetElapsedTime() << std::endl;
}