#pragma once

class Renderable
{
public:
	virtual ~Renderable(void);

	virtual void PreRender() = 0;
protected:
	Renderable(void);
};

