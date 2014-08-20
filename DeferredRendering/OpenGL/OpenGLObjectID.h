#pragma once

#include "Core/Types.h"

typedef size_t OpenGLObjectID;

class OpenGLObjectIDGenerator
{
public:
	OpenGLObjectIDGenerator(void);
	~OpenGLObjectIDGenerator(void);

	static OpenGLObjectID GetNextID();
private:
	static OpenGLObjectID gCurrentID;
};

