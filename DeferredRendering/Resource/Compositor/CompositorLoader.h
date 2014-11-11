#pragma once

#include "Resource/ResourceLoader.h"
#include "Compositor/Compositor.h"
#include "JSON/json.h"

namespace krono
{

class CompositorLoader : public ResourceLoader
{
public:
	CompositorLoader(void);
	~CompositorLoader(void);

	virtual Auto<Resource> LoadResource(ResourceManager& resourceManager, std::istream& inputStream, const std::string& internalName);

	static Compositor::Ptr ParseCompositor(ResourceManager& resourceManager, const json::Value& root);
private:
	static void ParseRenderTargets(Compositor::Ptr& compositor, const json::Object& renderTargets);
	static void ParseStage(ResourceManager& resourceManager, Compositor::Ptr& compositor, const json::Value& stageJson);
	static void ParseStageConnections(CompositeStageConnections& connections, const json::Value& stageJson);
};

}

