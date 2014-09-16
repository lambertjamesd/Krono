project (ShaderBundler)

set (SOURCE_FILES
	ShaderBundler.cpp

	Bundle/Bundle.cpp
	Bundle/BundleDefinition.cpp
	Bundle/GLSLBundler.cpp

	Bundle/FileNotFoundException.cpp
	Bundle/BundlerFileHelper.cpp

	HLSLParser/GLSLGenerator/GLSLGenerator.cpp
	HLSLParser/GLSLGenerator/GLSLSamplerPurger.cpp
	HLSLParser/GLSLGenerator/GLSLEntryPointBuilder.cpp
	HLSLParser/GLSLGenerator/GLSLTextureClass.cpp

	HLSLParser/HLSLParser.cpp
	HLSLParser/HLSLParserException.cpp
	HLSLParser/HLSLToken.cpp
	HLSLParser/HLSLTokenizer.cpp

	HLSLParser/HLSLBinaryOperatorNode.cpp
	HLSLParser/HLSLExpressionNode.cpp
	HLSLParser/HLSLFlowControlNode.cpp
	HLSLParser/HLSLNode.cpp
	HLSLParser/HLSLNodeVisitor.cpp
	HLSLParser/HLSLPostfixNode.cpp
	HLSLParser/HLSLPrefixNode.cpp
	HLSLParser/HLSLStatementNode.cpp
	HLSLParser/HLSLType.cpp
	HLSLParser/HLSLTypeNode.cpp
	HLSLParser/HLSLTypeVisitor.cpp
	HLSLParser/HLSLUnaryOperator.cpp
	HLSLParser/HLSLTextureClass.cpp
	HLSLParser/HLSLBuiltInFunctions.cpp

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

INSTALL_TARGETS(/bin ShaderBundler)
