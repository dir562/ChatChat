#include "stdafx.h"
#include "DynamicObj.h"
#include "TextRenderer.h"

void ControllObj::render_chat(glm::vec3 color)
{
	TextOpt opt;
	opt._align = TEXT_ALIGNMENT::mid;
	opt._space = { 0.0f, 0.0f };

	auto p = get_project_pos({ 0,2,0 });
	auto h = TextRenderer::get_height();
	for (const auto& chat : views::reverse(_chat.get_chat_balloon()))
	{
		auto str = chat._str;

		glColor4f(color.r, color.g, color.b, 1.f);
		TextRenderer::rend_text(str, p, opt, true);
		p.y += h;
	}
}

void ControllObj::apply_input_press(UCHAR key)
{
	if ('`' == key)
	{
		_state = _state == PLAYER_STATE::idle ? PLAYER_STATE::chat : PLAYER_STATE::idle;
		cout << "\nmodecahnge\n";
		return;
	}
	if (_state == PLAYER_STATE::chat)
	{
		_chat.chatting(key);
		return;
	}

	switch (key)
	{
	case 'w':
	case 'W':
		_up_on = true;
		break;
	case 's':
	case 'S':
		_down_on = true;
		break;
	case 'd':
	case 'D':
		_right_on = true;
		break;
	case 'a':
	case 'A':
		_left_on = true;
		break;
	case ' ':
		_brake_on = true;
		break;
	default:
		break;
	}
}

void ControllObj::apply_input_unpress(UCHAR key)
{
	if (_state == PLAYER_STATE::chat)
	{
		return;
	}

	switch (key)
	{
	case 'w':
	case 'W':
		_up_on = false;
		break;
	case 's':
	case 'S':
		_down_on = false;
		break;
	case 'd':
	case 'D':
		_right_on = false;
		break;
	case 'a':
	case 'A':
		_left_on = false;
		break;
	case ' ':
		_brake_on = false;
		break;
	default:
		break;
	}
}