require "website"
-- 通用入口函数
route = {"/",GET}
function access()
	local data = {
		name = "Fast Web 快速网站开发框架"
	}
	-- 返回JSON数据
	reply(200,"OK",data)
end