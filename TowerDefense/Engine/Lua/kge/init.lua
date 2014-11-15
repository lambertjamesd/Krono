
-- append to the c++ engine parts
-- this changes the kgecpp module as
-- well. I probably should make a deep
-- copy here, but whatever
local kge = require("kgecpp")
local util = require("util")

kge.Vector3 = require("kge.Vector3")

kge.LuaBehavior.transform = {__kge_isprop = true}

function kge.LuaBehavior.transform.get(self)
  return self.gameObject.transform
end

local classSources = {}
local loadedClasses = {}
local loadedEnvironments = {}
local includeStack = {}
local classDependencies = {}

local function MapClass(className, chunkLoader)
  classSources[className] = chunkLoader
end

function kge.MapClassToSource(className, source)
  MapClass(className, function()
      return loadstring(source, className)
  end)
end

function kge.MapClassToPath(className, filename)
  MapClass(className, function ()
      return loadfile(filename)
  end)
end

function kge.DefineClass(cleanedUpClass, className, classEnvironment)
  loadedEnvironments[className] = classEnvironment
  
  -- copy over values that aren't in the global scope
  for key, value in pairs(classEnvironment) do
    if _G[key] ~= value then
      cleanedUpClass[key] = value
    end
  end
  
  cleanedUpClass.className = className
  
  if cleanedUpClass.base then
    cleanedUpClass.__index = cleanedUpClass.base.__index
    cleanedUpClass.__newindex = cleanedUpClass.base.__newindex
  end
  
  return cleanedUpClass
end

function kge.Extends(className, classEnvironment)
  local baseClass = nil;
  
  if type(className) == "string" then
    for _, includedClass in pairs(includeStack) do
      if includedClass == className then
        error("Circular class dependency detected: " .. table.concat(includeStack, " => ") .. " => " .. className)
      end
    end
    -- setup class dependencies
    classDependencies[includeStack[#includeStack]] = className
    
    baseClass = kge.RequireClass(className)
  elseif type(className) == "table" then
    baseClass = className
  else
    error("Invalid base class type")
  end
  
  util.Merge(classEnvironment, baseClass)
  classEnvironment.base = baseClass
  
  return baseClass
end

local sandboxWhitelist = {
  "assert",
  "collectgarbage",
  "dofile",
  "error",
  "getfenv",
  "getmetatable",
  "ipairs",
  "load",
  "loadfile",
  "loadstring",
  "module",
  "next",
  "pairs",
  "pcall",
  "print",
  "rawequal",
  "rawget",
  "rawset",
  "require",
  "select",
  "setfenv",
  "setmetatable",
  "tonumber",
  "tostring",
  "type",
  "unpack",
  "xpcall",
  "_VERSION"
  }

function kge.RequireClass(className)
  table.insert(includeStack, className)
  local result = kge.Using(className)
  table.remove(includeStack)
  return result;
end

function kge.Using(className)
  if loadedClasses[className] then
    return loadedClasses[className]
  elseif classSources[className] then
    -- keep a history of loaded files to prevent an include loop
    
    local classChunk = classSources[className]()
    -- create a sandboxed environment for the class
    local classEnviornment = {}
    local cleanedUpClass = {}
  
    loadedClasses[className] = cleanedUpClass
    
    for _, key in ipairs(sandboxWhitelist) do
      classEnviornment[key] = _G[key]
    end
    
    classEnviornment._G = classEnviornment
    
    setfenv(classChunk, classEnviornment)
    
    classDependencies[className] = {}
    
    classChunk()
    
    
    kge.DefineClass(cleanedUpClass, className, classEnviornment)
    
    return cleanedUpClass
  else
    error("No class named " .. className)
  end
end

function kge.LoadClasses()
  for className in classSources do
    if not loadedClasses[className] then
      kge.Using(className)
    end
  end
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