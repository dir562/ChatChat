#pragma once

#include "VAO_OBJ.h"
#include "ChatManager.h"

/* moving obj */
class DynamicObj : public OBJ
{
public:
	explicit DynamicObj(ObjDataPtr obj_data, GLuint shader)
		:OBJ{ obj_data, shader } {}

public:
	glm::vec3 get_speed() { return _linear_speed; }
	glm::vec3 get_moving_dir() { return glm::normalize(_linear_speed); }
protected:
	void update(float time_elapsed)
	{
		update_rotate(time_elapsed);
		update_speed(time_elapsed);
		update_movement(time_elapsed);
	}

private:
	void update_rotate(float time_elapsed)
	{
		/* rotate */
		rotate(_angular_speed * time_elapsed);
	}

	void update_speed(float time_elapsed)
	{
		auto head_dir = get_head_dir();

		/* fric */
		auto prev_dir = glm::normalize(_linear_speed);
		auto prev_speed = glm::length(_linear_speed);
		if (prev_speed < ellipsis)
		{
			prev_dir = V_ZERO;
		}
		auto fric = -1 * prev_dir * _friction * time_elapsed;

		/* accel */
		auto accel = head_dir * _acceleration * time_elapsed;

		///* accumulate */
		auto new_linear_speed = _linear_speed + accel + fric;

		/* no backward movement */
		auto new_dir = glm::normalize(new_linear_speed);
		auto new_speed = glm::length(new_linear_speed);
		if (new_speed < ellipsis)
		{
			new_dir = V_ZERO;
		}

		/* overspeed */
		bool over_speed = _max_speed < new_speed;
		// [B] if (over_speed) { new_linear_speed = new_dir * _max_speed; }
		new_linear_speed -= new_linear_speed * over_speed;
		new_linear_speed += new_dir * _max_speed * over_speed;

		_linear_speed = new_linear_speed;
	}

	void update_movement(float time_elapsed)
	{
		move(_linear_speed * time_elapsed);
	}

public:
	static constexpr float _max_speed = 15.0f;

protected:
	glm::vec3 _linear_speed{ 1.0f,0.0f,0.0f };
	glm::vec3 _angular_speed{};

	float _acceleration = 0.f;
	float _friction = 0.f;

};

enum class PLAYER_STATE
{
	idle,
	chat
};

/* controllable obj */
class ControllObj : public DynamicObj
{
public:
	enum class CONTROLL { negative = -1, none = 0, positive = 1 };

public:
	explicit ControllObj(size_t id, ObjDataPtr obj_data, GLuint shader)
		: DynamicObj{ obj_data, shader }, _id{ id }, _chat{ id }
	{}

public:
	void update(float time_elapsed)
	{
		_chat.update(GAME_SYSTEM::instance().game_time());
		update_state();
		DynamicObj::update(time_elapsed);
	}

private:
	void update_state()
	{
		const glm::vec3 _angular_power = Y_DEFAULT * 1.5f;
		constexpr float _acceleration_power = 16.f;
		constexpr float _friction_power = 2.0f;

		_angular_speed = _angular_power * static_cast<int>(_angular_control);
		_acceleration = _acceleration_power * static_cast<int>(_accel_control);

		// [B] if(_brake_on)_acceleration = 0;
		//_acceleration -= _acceleration * _brake_on;

		_friction = _friction_power;
		// [B] if(_brake_on)_friction += _friction_power;
		_friction += _friction_power * _brake_on;
		_friction += _friction_power * _brake_on;

		int accel_control{ static_cast<int>(CONTROLL::none) };
		int angular_control{ static_cast<int>(CONTROLL::none) };
		accel_control += _up_on;
		accel_control -= _down_on;
		angular_control -= _right_on;
		angular_control += _left_on;
		_accel_control = static_cast<CONTROLL>(accel_control);
		_angular_control = static_cast<CONTROLL>(angular_control);
	}

public:
	void render_chat(glm::vec3 color = {});
	
public:
	void apply_input_press(UCHAR key);
	void apply_input_unpress(UCHAR key);

private:
	CONTROLL _accel_control{ CONTROLL::none };
	CONTROLL _angular_control{ CONTROLL::none };

	bool _up_on = false;
	bool _down_on = false;
	bool _right_on = false;
	bool _left_on = false;

	bool _brake_on = false;
	bool _use_item = false;

	PLAYER_STATE _state{ PLAYER_STATE::idle };

	size_t _id;
	ChatMachine _chat;
};
using Player0Ptr = shared_ptr<ControllObj>;


