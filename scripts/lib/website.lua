local dkjson = require 'dkjson'

function json(data)
	response:send(dkjson.encode(data))
end
function session()
	 return request:session(request:token())
end
