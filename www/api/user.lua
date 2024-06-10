require("api.website")
local dkjson = require("api.dkjson")

-- 登录
local function login()
    -- 获取session
    local session = session()
    -- 验证session有效
    if session:check() then
        reply(201,"你已经登录过了")
        return
    end

    -- 验证账号密码
    if param("username") ~= "fastweb" or param("password") ~= "123456" then
        reply(201,"账号或密码不正确")
        return
    end

    -- 生成TOKEN
    local token = make_software_guid()
    session:init(request,token)


    -- 填充用户信息
    session:set("login_time",time.now_time("%Y-%m-%d %H:%M:%S"))

    reply(200,"",{
        token = token
    })
end
-- 获取个人信息
local function getinfo()
    local session = session()
    if session:check() == false then
        reply(401,"登录信息已过期")
        return
    end

    reply(200,"",{
        login_time = session:get("login_time")
    })
end

-- 判断请求类型
if param("action") == "login" then
    login()
elseif param("action") == "getinfo" then
    getinfo()
end