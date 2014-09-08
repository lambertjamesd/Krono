#pragma once

#include "HLSLNode.h"
#include "HLSLStatementNode.h"
#include "HLSLExpressionNode.h"
#include "HLSLFlowControlNode.h"
#include "HLSLUnaryOperator.h"
#include "HLSLBinaryOperatorNode.h"
#include "HLSLPrefixNode.h"
#include "HLSLPostfixNode.h"

class HLSLNodeVisitor
{
public:
	~HLSLNodeVisitor(void);

	virtual void Visit(HLSLNode& node);
	virtual void Visit(HLSLNodeList& node);
	
	virtual void Visit(HLSLStatementNode& node);
	virtual void Visit(HLSLStatementBlock& node);
	virtual void Visit(HLSLTypedefDefinition& node);
	virtual void Visit(HLSLVariableDefinition& node);
	virtual void Visit(HLSLFunctionParameter& node);
	virtual void Visit(HLSLFunctionDefinition& node);
	virtual void Visit(HLSLReturnStatement& node);
	virtual void Visit(HLSLStructureMember& node);
	virtual void Visit(HLSLStructDefinition& node);
	virtual void Visit(HLSLCBufferDefinition& node);
	
	virtual void Visit(HLSLTypeNode& node);
	virtual void Visit(HLSLVoidNode& node);
	virtual void Visit(HLSLNamedTypeNode& node);
	virtual void Visit(HLSLScalarTypeNode& node);
	virtual void Visit(HLSLBufferTypeNode& node);
	virtual void Visit(HLSLArrayTypeNode& node);
	virtual void Visit(HLSLVectorTypeNode& node);
	virtual void Visit(HLSLMatrixTypeNode& node);
	virtual void Visit(HLSLTextureTypeNode& node);
	virtual void Visit(HLSLSamplerTypeNode& node);
	virtual void Visit(HLSLStructTypeNode& node);
	virtual void Visit(HLSLFunctionTypeNode& node);
	
	virtual void Visit(HLSLFlowControlNode& node);
	virtual void Visit(HLSLDoNode& node);
	virtual void Visit(HLSLIfNode& node);
	virtual void Visit(HLSLForNode& node);
	virtual void Visit(HLSLSwitchNode& node);
	virtual void Visit(HLSLSwitchLabelNode& node);
	virtual void Visit(HLSLDefaultNode& node);
	virtual void Visit(HLSLWhileNode& node);
	virtual void Visit(HLSLBreakNode& node);
	virtual void Visit(HLSLContinueNode& node);
	virtual void Visit(HLSLDiscardNode& node);

	virtual void Visit(HLSLExpressionNode& node);
	virtual void Visit(HLSLIdentifierNode& node);
	virtual void Visit(HLSLNumberConstantNode& node);
	virtual void Visit(HLSLBooleanConstantNode& node);
	virtual void Visit(HLSLTypeExpressionNode& node);
	virtual void Visit(HLSLParenthesisNode& node);
	
	virtual void Visit(HLSLBinaryOperatorNode& node);
	
	virtual void Visit(HLSLUnaryOperator& node);
	virtual void Visit(HLSLCastNode& node);
	
	virtual void Visit(HLSLPrefixNode& node);
	virtual void Visit(HLSLPrefixIncrNode& node);
	virtual void Visit(HLSLPrefixDecrNode& node);
	
	virtual void Visit(HLSLPostfixNode& node);
	virtual void Visit(HLSLIndexNode& node);
	virtual void Visit(HLSLPostfixIncr& node);
	virtual void Visit(HLSLPostfixDecr& node);
	virtual void Visit(HLSLStructureNode& node);
	virtual void Visit(HLSLFunctionCallNode& node);
protected:
	HLSLNodeVisitor(void);
};

