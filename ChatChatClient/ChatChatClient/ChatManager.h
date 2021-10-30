#pragma once


////////////////////////////////////////

struct id_str
{
	id_str(size_t ID, string_view s) :id{ ID }, str{ s }{};
	id_str() = default;
	size_t id;
	string str;
};

class ChatManager
{
public:
	static ChatManager& instance()
	{
		static ChatManager _instance;
		return _instance;
	}

	ChatManager(ChatManager&) = delete;
	ChatManager& operator=(ChatManager&) = delete;

private:
	ChatManager()
	{
		_record.reserve(100);
	}

public:
	[[nodiscard]] const auto& add(size_t id, string_view str)
	{
		return _record.emplace_back(id, str);
	}

	const auto& get_record()
	{
		return _record;
	}

	void reset_record()
	{
		_record.clear();
	}

private:
	vector<id_str> _record;
};

////////////////////////////////////////

struct life_time_str_view
{
	string_view _str;
	size_t _daed_line;
};

class ChatMachine
{
	static constexpr size_t life_time = 5000;
public:
	explicit ChatMachine(size_t ownner) :_ownner{ ownner } {};
public:
	void chatting(char c)
	{
		if (c == VK_RETURN) [[unlikely]]
		{
			size_t dead_line = GAME_SYSTEM::instance().game_time() + life_time;
			enter(_buf, dead_line);
		}
		else if (_buf.size() < 30)
		{
			_buf += c;
			cout << c;
		}
	}

	const auto& get_chat_balloon()
	{
		return _chat_balloon;
	}

	void enter(string_view str, size_t dead_line)
	{
		if (str.empty()) return;
		const auto& str_ref = ChatManager::instance().add(_ownner, str);
		cout << "\n::" << str << "::\n";
		_chat_balloon.emplace_back(str_ref.str, dead_line);
		_buf.clear();
	}

	void update(size_t game_time)
	{
		if (_chat_balloon.empty()) return;

		if (_chat_balloon.begin()->_daed_line < game_time)
		{
			cout << "\npop" << _chat_balloon.size() << "\n";
			_chat_balloon.pop_front();
		}
	}

private:
	size_t _ownner;
	string _buf;
	list<life_time_str_view> _chat_balloon;
};
