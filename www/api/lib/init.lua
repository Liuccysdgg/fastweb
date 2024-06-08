

print("init fast web success!")
-- 增加拦截器
interceptor.add("/api/*.*","/api/lib/interceptor.lua")
return true