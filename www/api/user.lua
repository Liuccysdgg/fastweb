local request = require("fastweb.request")
local response = require("fastweb.response")
local fw = require("fastweb")
local cjson = require("cjson")
local function reply(code,msg,data)
    
    response.header("Content-Type","application/json")
    response.send(cjson.encode({
        code = code,
        msg = msg,
        data = data
    }))
    
end

-- 登录
local function login()
    local token = request.header("token")
    -- 获取session
    local session = request.session(token)
    -- 验证session有效
    if session:check() then
        reply(201,"你已经登录过了")
        return
    end

    -- 验证账号密码
    if request.param("username",false) ~= "fastweb" or request.param("password",false) ~= "123456" then
        reply(201,"账号或密码不正确")
        return
    end

    -- 生成TOKEN
    token = fw.make_software_guid()
    session:init(token)


    -- 填充用户信息
    session:set("login_time",os.date("%Y-%m-%d %H:%M:%S"))

    reply(200,"",{
        token = token
    })
end
-- 获取个人信息
local function getinfo()
    local token = request.header("token")
    -- 获取session
    local session = request.session(token)
    if session:check() == false then
        reply(401,"登录信息已过期")
        return
    end

    reply(200,"",{
        login_time = session:get("login_time")
    })
end

-- 判断请求类型
if request.param("action",false) == "login" then
    login()
elseif request.param("action",false) == "getinfo" then
    getinfo()
end