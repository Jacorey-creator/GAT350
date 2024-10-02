#include "Framebuffer.h"
#include "Renderer.h"

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width,
	m_height = height,
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	//Error Handling
	if(!m_texture) std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color );
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	if (x > m_width - 1 || x < 0 || y > m_height - 1 || y < 0) return;
	//Creates Coordinate
	m_buffer[x + y * m_width] = color;
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x > m_width - 1 || x < 0 || y > m_height - 1 || y < 0) return;
	
	//Turns the Coordinate to an Area
	int endX = x + w;
	int endY = y + h;

	for (int i = x; i < endX; i++)
	{
		for (int j = y; j < endY; j++)
		{
			// Calculate Index Per Pixel
			int index = j * m_width + i;

			// Change Pixel Color
			m_buffer[index] = color;
		}
	}
}


