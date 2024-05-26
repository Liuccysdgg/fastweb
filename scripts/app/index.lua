require "website"

-- 通用入口函数
function access()

	-- MYSQL执行SELECT查询map表
	--local result = mysql:select():table("map"):query()

	-- 构建回复数据
	--local data = result:table()

	local data = {
		name = "Fast Web 快速网站开发框架"
	}
	-- 返回JSON数据
	json(data)
end