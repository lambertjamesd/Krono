
local kge = require("kge")
local base = extends("BaseTest")

function foo.get(self)
  return "foobar"
end

function Update(self, deltaTime)
  base.Update(self, deltaTime)
end

