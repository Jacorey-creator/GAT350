#pragma once
#include <vector>
#include <memory>
#include "Sphere.h"
#include "Framebuffer.h"

class Scene 
{
public:
	Scene() = default;

	void AddObject(std::unique_ptr<class SceneObject> object) { m_objects.push_back(std::move(object)); }
	void Render(Framebuffer& framebuffer, const class Camera& camera);

	friend class Tracer;

private:
	std::vector<std::unique_ptr<class SceneObject>> m_objects;
};
