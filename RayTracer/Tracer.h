#pragma once
#include "Framebuffer.h"
#include "Ray.h"
#include "Camera.h"
class Tracer 
{
	public:
		Tracer() = default;

		void Render(Framebuffer& framebuffer, const class Camera& camera);
};