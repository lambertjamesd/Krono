#include "HLSLNodeVisitor.h"
#include "HLSLParserException.h"

HLSLNodeVisitor::HLSLNodeVisitor(void)
{
}


HLSLNodeVisitor::~HLSLNodeVisitor(void)
{
}

void HLSLNodeVisitor::Visit(HLSLNode& node)
{
	throw HLSLParserException("Visit not implemented for node");
}

void HLSLNodeVisitor::Visit(HLSLNodeList& node)
{
	for (size_t i = 0; i < node.GetSize(); ++i)
	{
		node.GetElement(i).Accept(*this);
	}
}

void HLSLNodeVisitor::Visit(HLSLStatementNode& node)
{
	Visit(static_cast<HLSLNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLStatementBlock& node)
{
	Visit(static_cast<HLSLNodeList&>(node));
}

void HLSLNodeVisitor::Visit(HLSLTypedefDefinition& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLVariableDefinition& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLFunctionParameter& node)
{
	Visit(static_cast<HLSLNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLFunctionDefinition& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLReturnStatement& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLStructureMember& node)
{
	Visit(static_cast<HLSLNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLStructDefinition& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLCBufferDefinition& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLTypeNode& node)
{
	Visit(static_cast<HLSLNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLVoidNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLNamedTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLScalarTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLBufferTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLArrayTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLVectorTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLMatrixTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLTextureTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLSamplerTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLStructTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLFunctionTypeNode& node)
{
	Visit(static_cast<HLSLTypeNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLFlowControlNode& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLDoNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLIfNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLForNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLSwitchNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLSwitchLabelNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLDefaultNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLWhileNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLBreakNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLContinueNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLDiscardNode& node)
{
	Visit(static_cast<HLSLFlowControlNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLExpressionNode& node)
{
	Visit(static_cast<HLSLStatementNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLIdentifierNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLNumberConstantNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLBooleanConstantNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLTypeExpressionNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLParenthesisNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLBinaryOperatorNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLUnaryOperator& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLCastNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPrefixNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPrefixIncrNode& node)
{
	Visit(static_cast<HLSLPrefixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPrefixDecrNode& node)
{
	Visit(static_cast<HLSLPrefixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPostfixNode& node)
{
	Visit(static_cast<HLSLExpressionNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLIndexNode& node)
{
	Visit(static_cast<HLSLPostfixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPostfixIncr& node)
{
	Visit(static_cast<HLSLPostfixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLPostfixDecr& node)
{
	Visit(static_cast<HLSLPostfixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLStructureNode& node)
{
	Visit(static_cast<HLSLPostfixNode&>(node));
}

void HLSLNodeVisitor::Visit(HLSLFunctionCallNode& node)
{
	Visit(static_cast<HLSLPostfixNode&>(node));
}
