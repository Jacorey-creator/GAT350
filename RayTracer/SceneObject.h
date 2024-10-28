#pragma once
#include "Material.h"
#include <memory>
#include "Ray.h"
class SceneObject 
{
public:
	//SceneObject() = default;
	SceneObject(std::shared_ptr<Material> material) : m_material{ material } {}
	
	virtual bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float MaxDistance) = 0;

	std::weak_ptr<Material> GetMaterial() { return m_material; }
protected:
	std::shared_ptr<Material> m_material;
};