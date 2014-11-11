
local kge = require("kge")

kge.NewBehavior("Test", nil, 
	{
		Start = function (self)
			
		end,
	
		Update = function (self, deltaTime)
			local transform = self.gameObject.transform
			local position = transform.localPosition
			position[2] = position[2] + deltaTime * 0.1
			transform.localPosition = position
		end
	}
);