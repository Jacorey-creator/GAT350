#pragma once
#include "Color.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using vertex_t = glm::vec3;
using verticies_t = std::vector<vertex_t>;

class Model
{
public:
		Model() = default;
		Model(const verticies_t& verticies, const color_t& color) : m_vertices{ verticies }, m_color{ color } {}

		void SetColor(const color_t& newColor);
		void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
		bool Load(const std::string& filename);

private:
	verticies_t m_vertices;
	color_t m_color;

};