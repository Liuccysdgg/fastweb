require "website"
function access()
	if param("key") == nil or param("key") ~= "123465" then
		print("is not found key")
	end
	return true
end