
local kge = require("kge")

local LuaBehavior = kge.Extends(kge.LuaBehavior, getfenv())
local Test = kge.Using("Test")

function Update(self, deltaTime)
  local transform = self.gameObject.transform
  local position = transform.localPosition
  position[2] = position[2] + deltaTime * 0.1
  transform.localPosition = position
end