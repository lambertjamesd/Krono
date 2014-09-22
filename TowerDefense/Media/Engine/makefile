
Bundler = ../../../Debug/ShaderBundler/ShaderBundler
BundlerDependencies = ../../../Debug/ShaderBundler/ShaderBundler -MM 
ShaderDefinitions = Lights/PointLight.json Shaders/VertexShader.json
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
