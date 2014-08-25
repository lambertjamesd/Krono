#pragma once

#include "Core/Hash.h"
#include "Interface/Graphics.h"
#include "Interface/RenderTarget.h"
#include "Interface/DepthBuffer.h"

#include <map>
#include <string>

namespace krono
{

class RenderTargetDatabase;

class RenderTargetDescription
{
public:
	RenderTargetDescription();

	void AddRenderTarget(UInt32 targetID);
	void SetDepthTarget(UInt32 depthBuffer);
private:
	friend class RenderTargetDatabase;

	std::vector<UInt32> mRenderTargets;
	UInt32 mDepthBuffer;
};

class RenderTargetDatabase
{
public:
	RenderTargetDatabase(void);
	~RenderTargetDatabase(void);

	void BeginCompositeRender(const Vector2i& screenSize, Graphics& graphics);
	
	const RenderTarget::Ptr& GetRenderTarget(UInt32 targetID);
	const DepthBuffer::Ptr& GetDepthBuffer(UInt32 targetID);
	Texture2D::Ptr GetRenderTexture(UInt32 targetID);

	const Vector2i& GetRenderSize() const;
	
	void SetDataFormat(UInt32 targetID, const DataFormat& format);
	DataFormat GetDataFormat(UInt32 targetID) const;

	void UseRenderTargets(const RenderTargetDescription& descrpition);

	static UInt32 GetTargetID(const std::string& targetName);

	static const std::string& GetScreenTargetName();
private:
	void Resize(const Vector2i& renderSize);

	Vector2i mRenderSize;
	Graphics* mGraphics;

	std::map<UInt32, DataFormat> mTargetFormats;

	std::map<UInt32, RenderTarget::Ptr> mRenderTargets;
	std::map<UInt32, DepthBuffer::Ptr> mDepthBuffers;
	
	static const std::string gScreenTargetName;
};

}