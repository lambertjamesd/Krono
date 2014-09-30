
Bundler = ../../../Debug/ShaderBundler/ShaderBundler
BundlerDependencies = ../../../Debug/ShaderBundler/ShaderBundler -MM 
ShaderDefinitions = Lights/PointLight.json Lights/DirectionalLight.json Shaders/VertexShader.json Shaders/VertexShaderTangent.json Shaders/PreTransformedVertexShader.json Shaders/CompositorVertexShader.json Shaders/NormalMapped.json Shaders/CompositorVertexShader.json Shaders/CompositorCopy.json Compositor/Blur/BasicBlur.json Compositor/Blur/SpacialBlur.json Compositor/HemisphericLighting/HemisphericLighting.json Compositor/LightProbes/LightProbes.json Compositor/SSAO/SSAO.json
Shaders = $(ShaderDefinitions:.json=.shader)

DEPENDTMP = depend.mk

all: shaders

depend:
	$(BundlerDependencies) $(ShaderDefinitions) > $(DEPENDTMP)

shaders: $(Shaders)

%.shader: %.json
	$(Bundler) $<

%.json:
	touch $@

include $(DEPENDTMP)

clean:
	rm -rf $(Shaders)
