
local kge = require("kge")

local BaseTest = kge.Extends("BaseTest", getfenv())
local Test = kge.Using("BaseTest")

function Update(self, deltaTime)
  BaseTest.Update(self, deltaTime)
end