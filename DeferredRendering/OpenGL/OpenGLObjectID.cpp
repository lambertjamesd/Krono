
#include "OpenGLObjectID.h"


OpenGLObjectIDGenerator::OpenGLObjectIDGenerator(void)
{
}


OpenGLObjectIDGenerator::~OpenGLObjectIDGenerator(void)
{
}

OpenGLObjectID OpenGLObjectIDGenerator::GetNextID()
{
	return ++gCurrentID;
}

OpenGLObjectID OpenGLObjectIDGenerator::gCurrentID = 0;