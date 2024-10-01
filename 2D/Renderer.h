#pragma once
#include <string>
#include <iostream>

class Renderer
{
public:
	virtual int Init_SDL();
	virtual int Create_Window(const char* name, int width, int height);
};

