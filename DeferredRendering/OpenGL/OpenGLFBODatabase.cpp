
#include "OpenGLFBODatabase.h"
#include "Core/Hash.h"

OpenGLFBODatabase::OpenGLFBODatabase(void)
{

}

OpenGLFBODatabase::~OpenGLFBODatabase(void)
{

}

OpenGLFramebuffer& OpenGLFBODatabase::GetFrameBuffer(const std::vector<Auto<OpenGLRenderTarget> > &renderBuffers, const OpenGLDepthBuffer* depthBuffer)
{
	HashUInt32 signatureHash;

	for (auto it = renderBuffers.cbegin(); it != renderBuffers.cend(); ++it)
	{
		OpenGLObjectID objectID = (*it)->GetObjectID();
		signatureHash.Update(&objectID, sizeof(objectID));
	}

	UInt32 signature = signatureHash.GetDigest();

	auto findResult = mExistingBuffers.find(signature);

	if (findResult == mExistingBuffers.end())
	{
		Auto<OpenGLFramebuffer> result(new OpenGLFramebuffer(renderBuffers, depthBuffer));
		mExistingBuffers[signature] = result;
		return *result;
	}
	else
	{
		return *findResult->second;
	}
}