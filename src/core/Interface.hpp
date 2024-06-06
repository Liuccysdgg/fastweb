#pragma once
#include "base/error.h"

namespace fastweb
{
	class app;
	class Interface :public ylib::error_base
	{
	public:
		Interface(fastweb::app* ptr)
		{
			this->m_ptr = ptr;
		}
		fastweb::app* app() { return this->m_ptr; }
	public:
		fastweb::app* m_ptr = nullptr;
	};
}

