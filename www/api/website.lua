local dkjson = require("api.dkjson")

function json(data)
	response:header("Content-Type","application/json")
	response:send(dkjson.encode(data))
end
function session()
	 local token = request:header("token")
	 return request:session(token)
end
function param(name)
	return request:param(name,false)
end
function param_throw(name)
	return request:param(name,true)
end
function reply(code,msg,data)
	local data ={
		code =code,
		msg = msg,
		data = data
	}
	json(data)
end