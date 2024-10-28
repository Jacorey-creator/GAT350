#pragma once
#include "MathUtils.h"

#include <glm/glm.hpp>
#include <SDL.h>
#include <algorithm>

using color_t = SDL_Color;
using color3_t = glm::vec3;
using color4_t = glm::vec4;

inline color_t ColorConvert(const color4_t& color4)
{
	color_t color;
	color.r = (uint8_t)(Clamp(color4.r, 0.0f, 1.0f) * 255);
	color.g = (uint8_t)(Clamp(color4.g, 0.0f, 1.0f) * 255);
	color.b = (uint8_t)(Clamp(color4.b, 0.0f, 1.0f) * 255);
	color.a = (uint8_t)(Clamp(color4.a, 0.0f, 1.0f) * 255);

	return color;
}
inline color_t ColorConvert(const color3_t& color3)
{
	color_t color;
	color.r = (uint8_t)(Clamp(color3.r, 0.0f, 1.0f) * 255);
	color.g = (uint8_t)(Clamp(color3.g, 0.0f, 1.0f) * 255);
	color.b = (uint8_t)(Clamp(color3.b, 0.0f, 1.0f) * 255);
	color.a = (uint8_t)255.0f;

	return color;
}
enum class BlendMode
{
	Normal,
	Alpha,
	Additive,
	Multiply
};

color_t ColorBlend(const color_t& src, const color_t& dest);
color_t NormalBlend(const color_t& src, const color_t& dest);
color_t AlphaBlend(const color_t& src, const color_t& dest);

color_t AdditiveBlend(const color_t& src, const color_t& dest);
color_t MultiplyBlend(const color_t& src, const color_t& dest);

void SetBlendMode(BlendMode blendmode);

