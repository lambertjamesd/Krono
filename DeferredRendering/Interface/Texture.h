#pragma once
class Texture
{
public:
	virtual void BindToSlot(int slotIndex) = 0;

	Texture(void);
	virtual ~Texture(void);
};

