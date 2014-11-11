
local Vector3_index = {}
local Vector3 = {__index = Vector3_index}

function Vector3.New(x, y, z)
	x = x or 0
	y = y or 0
	z = z or 0
	
	local result = {x, y, z}
	setmetatable(result, Vector3);
	return result
end

function Vector3.Add(a, b, target)
	target[1] = a[1] + b[1]
	target[2] = a[2] + b[2]
	target[3] = a[3] + b[3]
	return target
end

function Vector3_index:tostring()
	return "(" .. table.concat(self, ", ") .. ")"
end

function Vector3_index:GetMagnitude()
	return math.sqrt(self[1] * self[1] + self[2] * self[2] + self[3] * self[3])
end

function Vector3.__add(a , b)
	if getmetatable(a) == Vector3 and getmetatable(b) == Vector3 then
		return Vector3.New(a[1] + b[1], a[2] + b[2], a[3] + b[3])
	else
		error("Could not add Vector3 to non Vector3 type")
	end
end

return Vector3