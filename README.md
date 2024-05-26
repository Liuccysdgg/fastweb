# Fast Web (快速网站开发框架)


## 简介
Fast Web 是使用 C++ 和 Lua 开发的网站框架，以快速开发、部署简单为理念，核心部分由C++实现，Lua 脚本语言处理业务逻辑，
Fast Web 旨在简化复杂的部署和环境配置，使开发者能够迅速启动并运行他们的网站。

QQ群：153076832
## 特性
- **LUA脚本**：通过Lua脚本实现业务逻辑开发，编写及维护更加容易
- **易于部署**：只需一个可执行文件
- **灵活易用**：适合各类个人博客、API接口到复杂的企业级项目

## 示例

URL: http://127.0.0.1:8888/scripts/index.lua?key=123456

#### 拦截器
```lua
function access()

    if request:pstring("key") == "123456" then
		return true
	end

	response:send("密钥不正确,已被拦截器拦截.")	
	return false
end
```
#### 业务代码
```lua
-- 通用入口函数
function access()

    -- MYSQL执行SELECT查询map表
	local result = mysql:select():table("map"):query()

	-- 构建回复数据
	local data = result:table()

	-- 返回JSON数据
	json(data)
end
```

## 部署
目前仅支持Windows部署，Linux将在下个版本支持
```bash
# 克隆仓库
git clone https://github.com/Liuccysdgg/fastweb.git
# 进入项目目录
cd fastweb
# 编译项目 (确保你的机器上安装了VS2022集成开发环境)
# 运行 fastweb.sln 编译
```

三方库均仅提供x64二进制发布版本,如需更新或修改请自行根据下方链接下载编译

## 🙇致谢
如果没有社区已有的优秀软件帮助，fastweb就不可能构建出来

https://github.com/Liuccysdgg/ylib 跨平台快速开发库

https://github.com/ldcsaa/HP-Socket HPSocket高性能网络库

https://github.com/ThePhD/sol2 C++ 与 Lua 的绑定

https://github.com/lua/lua 脚本解释器

