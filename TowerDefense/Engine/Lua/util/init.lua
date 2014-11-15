
local util = {}

function table.val_to_str ( v )
	if "string" == type( v ) then
		v = string.gsub( v, "\n", "\\n" )
		if string.match( string.gsub(v,"[^'\"]",""), '^"+$' ) then
			return "'" .. v .. "'"
		end
		return '"' .. string.gsub(v,'"', '\\"' ) .. '"'
	else
		return "table" == type( v ) and table.tostring( v ) or tostring( v )
	end
end

function table.key_to_str ( k )
	if "string" == type( k ) and string.match( k, "^[_%a][_%a%d]*$" ) then
		return k
	else
		return "[" .. table.val_to_str( k ) .. "]"
	end
end

function table.tostring( tbl )
	local result, done = {}, {}
	for k, v in ipairs( tbl ) do
		table.insert( result, table.val_to_str( v ) )
		done[ k ] = true
	end
	for k, v in pairs( tbl ) do
		if not done[ k ] then
			table.insert( result,
			table.key_to_str( k ) .. "=" .. table.val_to_str( v ) )
		end
	end
	return "{" .. table.concat( result, "," ) .. "}"
end

function util.ShallowCopy(value)
	if type(value) == "table" then
		local result = {}
		for key, value in pairs(value) do
			result[key] = value
		end
		return result
	else
		return value
	end
end

function util.DeepCopy(value, copyCache)
  copyCache = copyCache or {}
  
  
	if type(value) == "table" then
    if copyCache[value] then
      return copyCache[value]
    end
    
		local result = {}
    copyCache[result] = result
		for key, value in pairs(value) do
			result[key] = util.DeepCopy(value, copyCache)
		end
		return result
	else
		return value
	end
end

function util.Merge(targetTable, sourceTable, alreadyMerged)
  local copyCache = {}
  alreadyMerged = alreadyMerged or {}
  alreadyMerged[sourceTable] = true
  
	for key, value in pairs(sourceTable) do
		if targetTable[key] == nil or type(targetTable[key]) ~= type(value) then
			targetTable[key] = util.DeepCopy(value, copyCache)
		else
			if type(value) == "table" then
        if not alreadyMerged[value] then
          util.Merge(targetTable[key], value, alreadyMerged)
        end
			else
				targetTable[key] = value
			end
		end
	end
end

return util