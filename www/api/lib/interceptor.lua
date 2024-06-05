require "website"

if string.sub(request:filepath(),1,12) == "/api/public/" then
	--公共开放接口
	return true
elseif string.sub(request:filepath(),1,11) == "/api/admin/" then
	--管理员目录，需要验证权限
	print(param("key"),"\t")
	if param("key") == nil or param("key") ~= "123456" then
		response:send("key不正确")
		return false
	end
	return true
else
	response:send("仅允许访问 /api/public 和 /api/admin 目录")
	return false
end
