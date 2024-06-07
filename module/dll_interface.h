#if defined(_WIN32) || defined(_WIN64)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif
extern "C" {
	/// <summary>
	/// 注册模块入口
	/// </summary>
	/// <param name="sol2"></param>
	/// <param name="lua"></param>
	/// <returns>
	/// 0=成功
	/// 1=失败
	/// </returns>
	DLL_EXPORT int fastweb_module_regist(void* sol2,void* lua);
}
 