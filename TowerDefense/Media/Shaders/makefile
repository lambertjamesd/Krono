
Bundler = ../../../Debug/ShaderBundler/ShaderBundler
BundlerDependencies = ../../../Debug/ShaderBundler/ShaderBundler -MM 
ShaderDefinitions = Bundle/ColorMapper.json Bundle/DirectionalLight.json Bundle/PixelShaderTest.json Bundle/ScreenComposite.json Bundle/VertexShaderTest.json Bundle/PointLight.json Bundle/LightVertexShader.json
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
