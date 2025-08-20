#pragma once
#include "sol/sol.hpp"
#include "util/queue.hpp"
#include "module/basemodule.h"
namespace module
{
	class queue: public module::base{
	public:
		queue();
		~queue();
		void push(const std::string& value);
		sol::object pop(sol::this_state s);
		int size();
		void clear();
		static void regist(sol::state* lua);
		virtual void regist_global(const char* name, sol::state* lua) override;
		virtual void delete_global() { delete this; }
	private:
		ylib::queue<std::string> m_queue;
	};

}

