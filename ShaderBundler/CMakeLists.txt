project (ShaderBundler)

set (SOURCE_FILES
	ShaderBundler.cpp

	Bundle/Bundle.cpp
	Bundle/BundleDefinition.cpp
	Bundle/GLSLBundler.cpp

	Bundle/FileNotFoundException.cpp
	Bundle/BundlerFileHelper.cpp

	JSON/json.cpp
)

if (USE_DX11)
	set (SOURCE_FILES
		${SOURCE_FILES}
		Bundle/HLSLBundler.cpp
	)
endif (USE_DX11)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable (ShaderBundler ${SOURCE_FILES})
