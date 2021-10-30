#pragma once
#include "Renderer.h"

enum class TEXT_ALIGNMENT
{
	left, mid, right
};

struct Pointf
{
	Pointf() = default;
	Pointf(float x, float y) :_x{ x }, _y{ y }{};
	Pointf(glm::vec3 v) :_x{ v.x }, _y{ v.y }{};

	float _x;
	float _y;
};

struct TextSpace
{
	float _width;
	float _height;
};

struct TextOpt
{
	TEXT_ALIGNMENT _align;
	TextSpace _space;
};

/*
GLUT_BITMAP_8_BY_13;
GLUT_BITMAP_9_BY_15;
GLUT_BITMAP_TIMES_ROMAN_10;
GLUT_BITMAP_TIMES_ROMAN_24;
GLUT_BITMAP_HELVETICA_10;
GLUT_BITMAP_HELVETICA_12;
GLUT_BITMAP_HELVETICA_18;
*/
#define default_font GLUT_BITMAP_TIMES_ROMAN_24
class TextRenderer
{
public:

	static auto get_height(void* font = default_font)
	{
		return pixelsize_to_float(glutBitmapHeight(font), screen.height);
	}


	static auto get_width(const unsigned char* str, void* font = default_font)
	{
		return pixelsize_to_float(glutBitmapLength(font, str), screen.width);
	}

	static auto get_width(string_view str, void* font = default_font)
	{
		auto uchar_ptr = reinterpret_cast<const unsigned char*>(str.data());
		return get_width(uchar_ptr, font);
	}

	static void rend_text(string_view txt, Pointf point, TextOpt opt,
		bool back_ground, void* font = default_font)
	{
		const float x_diff = opt._space._width;
		const float y_diff = opt._space._height;
		auto str = reinterpret_cast<const unsigned char*>(txt.data());

		float space_len = x_diff * (txt.size() - 1);
		float len = get_width(str, font) + space_len;
		point = caculate_raster_pos(point, opt._align, len);

		glUseProgram(0);

		bool activate_space_opt = x_diff != 0 || y_diff != 0;
		if (activate_space_opt)
		{
			for (const auto c : txt)
			{
				glRasterPos2f(point._x, point._y);
				glutBitmapCharacter(font, c);
				point._x += x_diff;
				point._y += y_diff;
			}
		}
		else
		{
			glRasterPos2f(point._x, point._y);
			glutBitmapString(font, str);
		}

		if (back_ground)
		{
			auto xl = point._x - get_width(" ", font);
			auto xh = point._x + len + get_width(" ", font);
			auto yl = point._y - get_height(font) * 0.2f;
			auto yh = point._y + get_height(font) * 0.7f;
			glColor4f(1, 1, 1, 0.8f);
			glBegin(GL_QUADS);
			glVertex3f(xl, yl, 0.9f);
			glVertex3f(xh, yl, 0.9f);
			glVertex3f(xh, yh, 0.9f);
			glVertex3f(xl, yh, 0.9f);
			glEnd();
		}
	}
private:
	static float pixelsize_to_float(int pixelsize, int max_pixel)
	{
		return static_cast<float>(pixelsize) / (max_pixel >> 1);;
	}

	// ignored height, can be added
	static Pointf caculate_raster_pos(Pointf point, TEXT_ALIGNMENT align, float len)
	{
		Pointf raster_pos = point;
		switch (align)
		{
		case TEXT_ALIGNMENT::left:	// :1234
			break;
		case TEXT_ALIGNMENT::mid:	// 12:34
			raster_pos._x -= len / 2;
			break;
		case TEXT_ALIGNMENT::right:	// 1234:
			raster_pos._x -= len;
			break;
		}
		return raster_pos;
	}
};

