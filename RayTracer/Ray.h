#pragma once
#include "glm/glm.hpp"
struct ray_t
{
	glm::vec3 orgin;
	glm::vec3 direction;

	ray_t() = default;
	ray_t(const glm::vec3& orgin, const glm::vec3& direction) : orgin{ orgin }, direction{ direction } {}

	glm::vec3 At(float t) const { return orgin + direction * t; }
	glm::vec3 operator * (float t) const { return orgin + direction * t; }
};