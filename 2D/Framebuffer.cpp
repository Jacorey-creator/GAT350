#include "Framebuffer.h"
#include "Renderer.h"


const int LEFT = 1;    // 0001
const int RIGHT = 2;   // 0010
const int BOTTOM = 4;  // 0100
const int TOP = 8;     // 1000

// Clipping area boundaries
const int CLIP_MIN_X = 0;
const int CLIP_MAX_X = SCREEN_WIDTH;
const int CLIP_MIN_Y = 0;
const int CLIP_MAX_Y = SCREEN_HEIGHT;


int GetOutcode(int x, int y)
{
	int outcode = 0;
	if (x < CLIP_MIN_X) outcode |= LEFT;   // to the left of the clipping area
	if (x > CLIP_MAX_X) outcode |= RIGHT;  // to the right of the clipping area
	if (y < CLIP_MIN_Y) outcode |= BOTTOM; // below the clipping area
	if (y > CLIP_MAX_Y) outcode |= TOP;    // above the clipping area
	return outcode;
}

bool ClipLine(int& x1, int& y1, int& x2, int& y2)
{
	int outcode1 = GetOutcode(x1, y1);
	int outcode2 = GetOutcode(x2, y2);
	bool accept = false;

	while (true) {
		if (!(outcode1 | outcode2)) {
			// Both points inside the clipping area
			accept = true;
			break;
		}
		else if (outcode1 & outcode2) {
			// Both points are outside the clipping area
			break;
		}
		else {
			// Some segment of the line lies within the clipping area
			int outcodeOut = outcode1 ? outcode1 : outcode2;

			int x, y;

			// Find the intersection point
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x1 + (x2 - x1) * (CLIP_MAX_Y - y1) / (y2 - y1);
				y = CLIP_MAX_Y;
			}
			else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x1 + (x2 - x1) * (CLIP_MIN_Y - y1) / (y2 - y1);
				y = CLIP_MIN_Y;
			}
			else if (outcodeOut & RIGHT) {  // point is to the right of the clip rectangle
				y = y1 + (y2 - y1) * (CLIP_MAX_X - x1) / (x2 - x1);
				x = CLIP_MAX_X;
			}
			else if (outcodeOut & LEFT) {   // point is to the left of the clip rectangle
				y = y1 + (y2 - y1) * (CLIP_MIN_X - x1) / (x2 - x1);
				x = CLIP_MIN_X;
			}

			// Now we move the outside point to the intersection point
			if (outcodeOut == outcode1) {
				x1 = x;
				y1 = y;
				outcode1 = GetOutcode(x1, y1);
			}
			else {
				x2 = x;
				y2 = y;
				outcode2 = GetOutcode(x2, y2);
			}
		}
	}
	return accept;
}

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

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	// Clip the line first
	if (!ClipLine(x1, y1, x2, y2)) return;

	//calculate deltas
	int dx = (x2 - x1);
	int dy = (y2 - y1);
	//check for steep line
	bool steep = std::abs(dx) > std::abs(dy);
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	//ensure left to right drawing
	if (x1 > y1) 
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	//recalculate
	dx = (x2 - x1);
	dy = abs(y2 - y1);
	//set error
	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	//draw line points
	for (int x = x1, y = y1; x <= x2; x++) 
	{
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;
		//update error
		if (error < 0) 
		{
			y += ystep;
			error += dx;
		}
	}

}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x1, y1, x3, y3, color);
}

void Framebuffer::DrawCircle(int r, int centerX, int centerY, const color_t& color)
{
	int radius = r;
	int y = 0;
	int d = 1 - radius;   // Decision variable for midpoint algorithm

	while (radius >= y) {
		// Plot the points in all eight octants
		DrawPoint(radius + centerX, y + centerY, color);
		DrawPoint(y + centerX, radius + centerY, color);
		DrawPoint(-radius + centerX, y + centerY, color);
		DrawPoint(-y + centerX, radius + centerY, color);
		DrawPoint(-radius + centerX, -y + centerY, color);
		DrawPoint(-y + centerX, -radius + centerY, color);
		DrawPoint(radius + centerX, -y + centerY, color);
		DrawPoint(y + centerX, -radius + centerY, color);

		y++;

		if (d <= 0) {
			d += 2 * y + 1;  // Midpoint inside the circle
		}
		else {
			radius--;
			d += 2 * (y - radius) + 1;  // Midpoint outside the circle
		}
	}
}


void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = (x2 - x1);
	int dy = (y2 - y1);
	
	if (dx == 0) 
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++) 
		{
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else
	{
		float m = dy / (float)dx;
		float b = y1 - (m * x1);

		if (std::abs(dx) > std::abs(dy))
		{
			for (int x = x1; x < x2; x++)
			{
				//y = mx + b
				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else
		{
			for (int y = y1; y <= y2; y++)
			{

				// Assuming that the round function finds 
				// closest integer to a given float. 
				int x = (int)round((m * y) + b);

				m_buffer[x + y * m_width] = color;
			}
		}
	}
}

