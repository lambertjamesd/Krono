project (ShaderBundler)

set (SOURCE_FILES
	ShaderBundler.cpp

	Bundle/Bundle.cpp
	Bundle/BundleDefinition.cpp
	Bundle/GLSLBundler.cpp

	Bundle/FileNotFoundException.cpp
	Bundle/BundlerFileHelper.cpp

	HLSLParser/HLSLParser.cpp
	HLSLParser/HLSLParserException.cpp
	HLSLParser/HLSLToken.cpp
	HLSLParser/HLSLTokenizer.cpp

	HLSLParser/Preprocessor/BinaryOperatorNode.cpp
	HLSLParser/Preprocessor/DirectiveNode.cpp
	HLSLParser/Preprocessor/EvaluateVisitor.cpp
	HLSLParser/Preprocessor/Exception.cpp
	HLSLParser/Preprocessor/ExpressionNode.cpp
	HLSLParser/Preprocessor/GenerateOutputVisitor.cpp
	HLSLParser/Preprocessor/IncludeHandler.cpp
	HLSLParser/Preprocessor/Macro.cpp
	HLSLParser/Preprocessor/Node.cpp
	HLSLParser/Preprocessor/NodeVisitor.cpp
	HLSLParser/Preprocessor/Preprocessor.cpp
	HLSLParser/Preprocessor/Tokenizer.cpp
	HLSLParser/Preprocessor/UnaryOperatorNode.cpp

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
