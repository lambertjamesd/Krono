
-- append to the c++ engine parts
-- this changes the kgecpp module as
-- well. I probably should make a deep
-- copy here, but whatever
local kge = require("kgecpp")
local util = require("util")

kge.Vector3 = require("kge.Vector3")

kge.behaviors = {}

function kge.NewBehavior(className, baseClassName, methods)
	local class = kge.behaviors[className] or {}
	
	for key in pairs(class) do
		class[key] = nil
	end
	
	local baseClass = kge.behaviors[baseClassName]
	
	if baseClass == nil then
		baseClass = kge.LuaBehavior
	end

	util.Merge(class, baseClass)
	
	for methodName, method in pairs(methods or {}) do
		if type(method) == "table" and (method.get or method.set) then
			class[2][methodName] = method.get
			class[3][methodName] = method.set
		else
			class[1][methodName] = method
		end
	end
	
	kge.behaviors[className] = class
end

-- check any weak references into the c++ code
-- if the object has been deleted then there is
-- no reason to hold the object in the reference
-- cache
local function CheckReferences()
	for id, reference in pairs(kge._ref) do
		if reference._kge_obj.isExpired then
			kge._ref[id] = nil
		end
	end
end

-- the strong pointer cache (the table named _ptr)
-- is used to ensure that there is only a single copy
-- of any shared pointers passed to lua. If the same
-- pointer is passed twice, the _ptr cache is used
-- to return the same object that was created the first
-- time
-- however, the cache would leak objects if they weren't
-- removed once the c++ is done since the lua context
-- would hold onto all references at all times
local function CheckPointers()
	for id, pointer in pairs(kge._ptr) do
		if pointer._kge_obj.isUnique then
			kge._ptr[id] = nil
		end
	end
end

function kge.GCRunCycle()
	CheckReferences()
	CheckPointers()
	
	kge.CreateGCCanary(kge.GCRunCycle)
end

kge.CreateGCCanary(kge.GCRunCycle)

return kge