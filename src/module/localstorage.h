#pragma once
#include "util/localstorage.h"
#include "imodule.h"
namespace module
{
	/// <summary>
	/// 本地缓存(leveldb)
	/// </summary>
	class local_storage : public ylib::local_storage,public module::imodule {
	public:
		local_storage();
		~local_storage() override;
		/// <summary>
		/// 取数据
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		sol::optional<std::string> readex(const std::string& name);

		static void regist(sol::state* lua);
	private:
		// 通过 imodule 继承
		virtual void regist_global(const std::string& name, sol::state* lua);
		virtual void delete_global() { delete this; }
	};
}

