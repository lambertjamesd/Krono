#pragma once

#include "Core/Hash.h"
#include "Interface/Graphics.h"
#include "Interface/RenderTarget.h"
#include "Interface/DepthBuffer.h"
#include "Interface/Color.h"
#include "RenderTargetConfiguration.h"

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

	void InitializeGraphics(Graphics& graphics, const Vector2i& size);
	void BeginCompositeRender(const RenderTargetConfiguration& output, Graphics& graphics);

	void ClearRenderTarget(UInt32 targetID, const Colorf& color);
	void ClearDepthBuffer(UInt32 targetID, float depth);
	
	const RenderTarget::Ptr& GetRenderTarget(UInt32 targetID);
	const DepthBuffer::Ptr& GetDepthBuffer(UInt32 targetID);
	Texture2D::Ptr GetRenderTexture(UInt32 targetID);

	void SetCurrentTargetSize(const Vector2i& size);
	const Vector2i& GetCurrentTargetSize() const;
	const Vector2i& GetRenderSize() const;
	
	void SetDataFormat(UInt32 targetID, const DataFormat& format);
	DataFormat GetDataFormat(UInt32 targetID) const;

	void SetTargetScale(UInt32 targetID, const Vector2f& value);
	Vector2f GetTargetScale(UInt32 targetID) const;

	void UseRenderTargets(const RenderTargetDescription& descrpition);

	static UInt32 GetTargetID(const std::string& targetName);
private:
	void Resize(const Vector2i& renderSize);

	Vector2i mCurrentTargetSize;
	Vector2i mRenderSize;
	Graphics* mGraphics;

	std::map<UInt32, DataFormat> mTargetFormats;
	std::map<UInt32, Vector2f> mTargetScale;

	std::map<UInt32, RenderTarget::Ptr> mRenderTargets;
	std::map<UInt32, DepthBuffer::Ptr> mDepthBuffers;
	
	static const std::string gOutputTargetName;
	static const std::string gOutputDepthName;
};

}
