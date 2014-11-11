#pragma once

#include "Serialization/ISerializable.h"
#include "Render/RenderStage.h"
#include "Render/RenderManager.h"

namespace kge
{

class RenderSerialization
{
public:
	RenderSerialization(RenderManager& renderManager);

	void SerializeDataFormat(SceneSerializer& serializer, const krono::DataFormat& dataFormat);
	krono::DataFormat DeserializeDataFormat(SceneDeserializer& deserializer);

	void SerializeRenderTarget(SceneSerializer& serializer, const krono::RenderTarget::Ptr& renderTarget);
	krono::RenderTarget::Ptr DeserializeRenderTarget(SceneDeserializer& deserializer);

	void SerializeDepthBuffer(SceneSerializer& serializer, const krono::DepthBuffer::Ptr& depthBuffer);
	krono::DepthBuffer::Ptr DeserializeDepthBuffer(SceneDeserializer& deserializer);

	void SerializeRenderTargetConfig(SceneSerializer& serializer, const krono::RenderTargetConfiguration& configuration);
	krono::RenderTargetConfiguration DeserializeRenderTargetConfig(SceneDeserializer& deserializer);

	void SerializeRenderStage(SceneSerializer& serializer, const RenderStage::Ptr& renderStage);
	void DeserializeRenderStage(SceneDeserializer& deserializer, RenderStage::Ptr target);
private:

	RenderManager& mRenderManager;
};

}