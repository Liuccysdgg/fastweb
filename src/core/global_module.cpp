#include "global_module.h"
fastweb::global_module::global_module(fastweb::app* app):Interface(app)
{

}

fastweb::global_module::~global_module()
{
	clear();
}

bool fastweb::global_module::regist(const std::string& name, module::base* module_ptr)
{
	if (m_ptrs.find(name) != m_ptrs.end())
		return false;
	m_ptrs.emplace(name,module_ptr);
	return true;
}

void fastweb::global_module::clear()
{
	for_iter(iter,m_ptrs)
	{
		delete iter->second;
	}
	m_ptrs.clear();
}
