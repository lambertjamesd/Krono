#pragma once

#include "OpenGLCommon.h"

#include "OpenGLObjectID.h"

namespace krono
{

class OpenGLRenderTarget
{
public:
	enum Type
	{
		TypeWindow,
		TypeTexture,
		TypeRenderBuffer
	};

	OpenGLRenderTarget(void);
	~OpenGLRenderTarget(void);

	virtual Type GetType() const = 0;
	virtual GLuint GetGLObject() const = 0;
	OpenGLObjectID GetObjectID() const;
private:
	OpenGLObjectID mObjectID;
};

}
