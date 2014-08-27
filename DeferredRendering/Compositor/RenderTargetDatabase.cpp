#include "RenderTargetDatabase.h"
#include <sstream>

namespace krono
{

RenderTargetDescription::RenderTargetDescription() :
	mDepthBuffer(0)
{

}

void RenderTargetDescription::AddRenderTarget(UInt32 targetID)
{
	mRenderTargets.push_back(targetID);
}

void RenderTargetDescription::SetDepthTarget(UInt32 depthBuffer)
{
	mDepthBuffer = depthBuffer;
}

RenderTargetDatabase::RenderTargetDatabase(void)
{
}


RenderTargetDatabase::~RenderTargetDatabase(void)
{
}

void RenderTargetDatabase::BeginCompositeRender(const RenderTargetConfiguration& output, Graphics& graphics)
{
	Resize(output.GetSize());
	mGraphics = &graphics;

	for (size_t index = 0; index < output.GetRenderTargets().size(); ++index)
	{
		std::ostringstream targetName;
		targetName << gOutputTargetName << index;
		mRenderTargets[GetTargetID(targetName.str())] = output.GetRenderTargets()[index];
	}

	mDepthBuffers[GetTargetID(gOutputDepthName)] = output.GetDepthBuffer();
}

void RenderTargetDatabase::ClearRenderTarget(UInt32 targetID, const Colorf& color)
{
	RenderTarget::Ptr target = GetRenderTarget(targetID);

	if (target != NULL)
	{
		target->Clear(color);
	}
}

void RenderTargetDatabase::ClearDepthBuffer(UInt32 targetID, float depth)
{
	DepthBuffer::Ptr target = GetDepthBuffer(targetID);

	if (target != NULL)
	{
		target->Clear(depth);
	}
}

const RenderTarget::Ptr& RenderTargetDatabase::GetRenderTarget(UInt32 targetID)
{
	auto result = mRenderTargets.find(targetID);

	if (result == mRenderTargets.end())
	{
		DataFormat dataFormat = GetDataFormat(targetID);

		if (dataFormat.type == DataFormat::TypeCount)
		{
			return RenderTarget::Null;
		}
		else
		{
			RenderTarget::Ptr result = mGraphics->CreateOffscreenRenderTarget(mRenderSize, dataFormat);
			mRenderTargets[targetID] = result;
			return mRenderTargets[targetID];
		}
	}
	else
	{
		return result->second;
	}
}

const DepthBuffer::Ptr& RenderTargetDatabase::GetDepthBuffer(UInt32 targetID)
{
	auto result = mDepthBuffers.find(targetID);

	if (result == mDepthBuffers.end())
	{
		DataFormat dataFormat = GetDataFormat(targetID);

		if (dataFormat.type == DataFormat::TypeCount)
		{
			return DepthBuffer::Null;
		}
		else
		{
			DepthBuffer::Ptr result = mGraphics->CreateDepthBuffer(mRenderSize, dataFormat.type);
			mDepthBuffers[targetID] = result;
			return mDepthBuffers[targetID];
		}
	}
	else
	{
		return result->second;
	}
}

Texture2D::Ptr RenderTargetDatabase::GetRenderTexture(UInt32 targetID)
{
	auto targetResult = mRenderTargets.find(targetID);

	if (targetResult != mRenderTargets.end())
	{
		return targetResult->second->GetTexture();
	}

	auto depthResult = mDepthBuffers.find(targetID);

	if (depthResult != mDepthBuffers.end())
	{
		return depthResult->second->GetTexture();
	}

	return Texture2D::Null;
}

const Vector2i& RenderTargetDatabase::GetRenderSize() const
{
	return mRenderSize;
}

void RenderTargetDatabase::SetDataFormat(UInt32 targetID, const DataFormat& format)
{
	mTargetFormats[targetID] = format;
}

DataFormat RenderTargetDatabase::GetDataFormat(UInt32 targetID) const
{
	auto result = mTargetFormats.find(targetID);

	if (result == mTargetFormats.end())
	{
		return DataFormat(DataFormat::TypeCount, 0);
	}
	else
	{
		return result->second;
	}
}

void RenderTargetDatabase::UseRenderTargets(const RenderTargetDescription& descrpition)
{
	std::vector<RenderTarget::Ptr> renderTargets;

	for (auto it = descrpition.mRenderTargets.cbegin(); it != descrpition.mRenderTargets.cend(); ++it)
	{
		renderTargets.push_back(GetRenderTarget(*it));
	}

	mGraphics->SetRenderTargets(renderTargets, GetDepthBuffer(descrpition.mDepthBuffer));
}

UInt32 RenderTargetDatabase::GetTargetID(const std::string& targetName)
{
	HashUInt32 hash;
	hash.Update(&targetName.front(), targetName.length());
	return hash.GetDigest();
}

void RenderTargetDatabase::Resize(const Vector2i& renderSize)
{
	if (mRenderSize != renderSize)
	{
		mRenderSize = renderSize;
		mRenderTargets.clear();
		mDepthBuffers.clear();
	}
}

const std::string RenderTargetDatabase::gOutputTargetName = "output";
const std::string RenderTargetDatabase::gOutputDepthName = "outputDepth";

}
