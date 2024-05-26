require "website"
-- MYSQL数据库池

function initMysql()
	local pool =  mysql_pool.new()
	if pool:start("127.0.0.1","test","123456","test","utf8mb4",3306,10) == false then
		print("start mysql pool failed")
		return false
	end
	 global_regist("mysql",pool:self())
	 return true
end


if initMysql() == false then
	return false
end

return true