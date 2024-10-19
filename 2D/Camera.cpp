#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetProjection(float fov, float aspect, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fov), aspect, near, far);
	
}

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
	m_view = glm::lookAt(eye, target, up);
}

glm::vec3 Camera::ModelToView(const glm::vec3& pos) const
{
	//convert point from world space to view space
	return m_view * glm::vec4{ pos, 1 };
}

glm::vec3 Camera::ViewToProjection(const glm::vec3& pos) const
{
	//convert point from view space to projection space
	return m_projection * glm::vec4{ pos, 1 };
}

glm::ivec2 Camera::ToScreen(const glm::vec3& pos) const
{
	//convert point from view space to projection space
	glm::vec4 clip = m_projection * glm::vec4{pos, 1};
	
	glm::vec3 ndc = clip / clip.w;
	float x = (ndc.x + 1) * (0.5f * screen_width);
	float y = (1 - ndc.y) * (0.5f * screen_height);
	return glm::ivec2(x, y);
}
