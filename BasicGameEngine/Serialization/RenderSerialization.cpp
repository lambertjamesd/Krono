#include "RenderSerialization.h"

#include "Serialization/SceneSerializer.h"
#include "Scene/Game.h"

using namespace krono;

namespace kge
{

RenderSerialization::RenderSerialization(RenderManager& renderManager) :
	mRenderManager(renderManager)
{

}

void RenderSerialization::SerializeDataFormat(SceneSerializer& serializer, const krono::DataFormat& dataFormat)
{
	serializer.BeginObject();

	serializer.WriteInt("count", dataFormat.count);
	serializer.WriteInt("type", dataFormat.type);

	serializer.EndObject();
}

krono::DataFormat RenderSerialization::DeserializeDataFormat(SceneDeserializer& deserializer)
{
	deserializer.BeginObject();
	DataFormat result((DataFormat::Type)deserializer.ReadInt("type", DataFormat::Int8UNorm), deserializer.ReadInt("count", 4));
	deserializer.EndObject();

	return result;
}

void RenderSerialization::SerializeRenderTarget(SceneSerializer& serializer, const krono::RenderTarget::Ptr& renderTarget)
{
	serializer.WriteSharedPointer(renderTarget, [&](SceneSerializer& serializer, const krono::Object::Ptr& objectToSerialize) {
		
		if (renderTarget == NULL)
		{
			serializer.WriteBool("null", true);
		}
		else if (mRenderManager.GetGameObjectScene().GetGame().GetWindowRenderTarget() == renderTarget)
		{
			serializer.WriteBool("window", true);
		}
		else
		{
			serializer.WriteKey("format");
			SerializeDataFormat(serializer, renderTarget->GetDataFormat());
			serializer.WriteInt("width", renderTarget->GetSize().x);
			serializer.WriteInt("height", renderTarget->GetSize().y);
		}
	});
}

krono::RenderTarget::Ptr RenderSerialization::DeserializeRenderTarget(SceneDeserializer& deserializer)
{
	return deserializer.ReadSharedPointer<krono::RenderTarget>([&](SceneDeserializer& deserializer)  -> krono::RenderTarget::Ptr {
		
		if (deserializer.ReadBool("null", false))
		{
			return krono::RenderTarget::Ptr(NULL);
		}
		else if (deserializer.ReadBool("window", false))
		{
			return mRenderManager.GetGameObjectScene().GetGame().GetWindowRenderTarget();
		}
		else
		{
			deserializer.ReadKey("format");
			DataFormat format = DeserializeDataFormat(deserializer);

			return mRenderManager.GetGraphics()->CreateOffscreenRenderTarget(
				Vector2i(deserializer.ReadInt("width", 0), deserializer.ReadInt("height", 0)),
				format);
		}
	});
}

void RenderSerialization::SerializeDepthBuffer(SceneSerializer& serializer, const krono::DepthBuffer::Ptr& depthBuffer)
{
	serializer.WriteSharedPointer(depthBuffer, [&](SceneSerializer& serializer, const krono::Object::Ptr& objectToSerialize) {
		if (depthBuffer == NULL)
		{
			serializer.WriteBool("null", true);
		}
		else
		{
			serializer.WriteInt("width", depthBuffer->GetSize().x);
			serializer.WriteInt("height", depthBuffer->GetSize().y);
			serializer.WriteInt("format", depthBuffer->GetFormat());
		}
	});
}

krono::DepthBuffer::Ptr RenderSerialization::DeserializeDepthBuffer(SceneDeserializer& deserializer)
{
	return deserializer.ReadSharedPointer<krono::DepthBuffer>([&](SceneDeserializer& deserializer)  -> krono::DepthBuffer::Ptr {
		
		if (deserializer.ReadBool("null", false))
		{
			return krono::DepthBuffer::Ptr(NULL);
		}
		else
		{
			return mRenderManager.GetGraphics()->CreateDepthBuffer(
				Vector2i(deserializer.ReadInt("width", 0), deserializer.ReadInt("height", 0)),
				(DataFormat::Type)deserializer.ReadInt("format", 0));
		}
	});
}

void RenderSerialization::SerializeRenderTargetConfig(SceneSerializer& serializer, const krono::RenderTargetConfiguration& configuration)
{
	serializer.BeginObject();

	serializer.BeginArray("renderTargets");

	for (size_t i = 0; i < configuration.GetRenderTargets().size(); ++i)
	{
		SerializeRenderTarget(serializer, configuration.GetRenderTargets()[i]);
	}

	serializer.EndArray();

	serializer.WriteKey("depthBuffer");
	SerializeDepthBuffer(serializer, configuration.GetDepthBuffer());

	serializer.EndObject();
}

krono::RenderTargetConfiguration RenderSerialization::DeserializeRenderTargetConfig(SceneDeserializer& deserializer)
{
	deserializer.BeginObject();

	krono::RenderTargetConfiguration result;

	size_t targetCount = deserializer.BeginArray("renderTargets");

	for (size_t i = 0; i < targetCount; ++i)
	{
		result.AddRenderTarget(DeserializeRenderTarget(deserializer));
	}

	deserializer.EndArray();

	deserializer.ReadKey("depthBuffer");
	result.SetDepthBuffer(DeserializeDepthBuffer(deserializer));

	deserializer.EndObject();

	return result;
}

void RenderSerialization::SerializeRenderStage(SceneSerializer& serializer, const RenderStage::Ptr& renderStage)
{
	serializer.BeginObject();

	serializer.WriteKey("renderTargets");
	SerializeRenderTargetConfig(serializer, renderStage->GetRenderTargets());

	Rectf viewport = renderStage->GetViewportRect();
	Rangef depthRange = renderStage->GetViewportDepth();

	serializer.WriteFloat("l", viewport.topLeft.x);
	serializer.WriteFloat("t", viewport.topLeft.y);
	
	serializer.WriteFloat("w", viewport.size.x);
	serializer.WriteFloat("h", viewport.size.y);

	serializer.WriteFloat("f", depthRange.start);
	serializer.WriteFloat("b", depthRange.end);

	serializer.WriteString("compositor", renderStage->GetCompositorName());

	serializer.EndObject();
}

void RenderSerialization::DeserializeRenderStage(SceneDeserializer& deserializer, RenderStage::Ptr target)
{
	deserializer.BeginObject();

	deserializer.ReadKey("renderTargets");
	target->SetRenderTargets(DeserializeRenderTargetConfig(deserializer));
	
	Rectf viewport = target->GetViewportRect();
	Rangef depthRange = target->GetViewportDepth();

	viewport.topLeft.x = deserializer.ReadFloat("l", viewport.topLeft.x);
	viewport.topLeft.y = deserializer.ReadFloat("t", viewport.topLeft.y);
	viewport.size.x = deserializer.ReadFloat("w", viewport.size.x);
	viewport.size.y = deserializer.ReadFloat("h", viewport.size.y);

	depthRange.start = deserializer.ReadFloat("f", depthRange.start);
	depthRange.end = deserializer.ReadFloat("b", depthRange.end);

	target->SetCompositorName(deserializer.ReadString("compositor", target->GetCompositorName()));

	deserializer.EndObject();
}

}