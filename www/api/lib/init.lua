

print("init fast web success!")
-- 自定义模块演示
local hello = hello.new()
print("Custom Module: "..hello:name())
-- 增加拦截器
interceptor.add("/api/*.*","/api/lib/interceptor.lua")
return true