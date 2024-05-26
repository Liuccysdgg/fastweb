function access()

	if request:pstring("key") == "123456" then
		return true
	end

	response:send("密钥不正确,已被拦截器拦截.")	
	return false
end