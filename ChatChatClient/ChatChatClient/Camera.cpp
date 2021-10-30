#include "stdafx.h"

#include "Camera.h"
#include "DynamicObj.h"

void Camera::update(float time_elpased)
{
	/* position */
	{
		const auto target_rotate = _ownner->get_rotation();
		const auto trans_diff = glm::translate(_diff);
		const auto trans = glm::translate(_ownner->get_position());
		const auto scale = glm::scale(_ownner->get_scale());
		auto m = trans * glm::toMat4(target_rotate) * trans_diff * scale;
		_position = m * V4_DEFAULT;
	}

	/* target */
	{
		const auto head_dir = _ownner->get_head_dir();
		const auto moving_speed = reinterpret_cast<DynamicObj*>(_ownner)->get_speed();
		const auto moving_len = glm::length(moving_speed);
		auto moving_dir = glm::normalize(moving_speed);
		if (moving_len < 0.125f)
		{
			moving_dir = head_dir;
		}
		auto target_dir = glm::lerp(head_dir, moving_dir, clamp(0.2f * moving_len, 0.00f, 0.4f));
		_target = _ownner->get_position() + target_dir * 10.0f;
	}

	/* shake */
	if (_shaking)
	{
		_shaking_time -= time_elpased;
		if (_shaking_time <= 0.f)
		{
			_shaking = false;
		}
		constexpr float magic_limit = 0.171875;
		constexpr float friq = 10.f;
		const float rand_val = static_cast<float>(rand() % 10);
		_position.x += cos(_shaking_time * friq * rand_val) * magic_limit;
		_position.y += sin(_shaking_time * friq * rand_val) * magic_limit;
		_position.z += cos(_shaking_time * friq * rand_val) * magic_limit;
	}
}
