
local kge = require("kge")
local base = extends(kge.LuaBehavior)

foo = {}
function foo.get(self)
  return "bar"
end

function Update(self, deltaTime)
  local transform = self.gameObject.transform
  local position = transform.localPosition
  position[2] = position[2] + deltaTime * 0.1
  transform.localPosition = position
end