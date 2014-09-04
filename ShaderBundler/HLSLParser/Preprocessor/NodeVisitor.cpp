#include "NodeVisitor.h"
#include "Exception.h"


namespace preproc
{

NodeVisitor::NodeVisitor(void)
{
}


NodeVisitor::~NodeVisitor(void)
{
}

void NodeVisitor::Visit(Node& node)
{
	throw Exception("Node visitor not implemented");
}

void NodeVisitor::Visit(OtherNode& node)
{
	Visit(static_cast<Node&>(node));
}

void NodeVisitor::Visit(DirectiveNode& node)
{
	Visit(static_cast<Node&>(node));
}

void NodeVisitor::Visit(DefineNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(IfNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(IncludeNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(LineNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(PragmaNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(UnDefNode& node)
{
	Visit(static_cast<DirectiveNode&>(node));
}

void NodeVisitor::Visit(ExpressionNode& node)
{
	Visit(static_cast<Node&>(node));
}

void NodeVisitor::Visit(DefinedOperatorNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(IdentifierNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(FunctionNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(ConstantNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(UnaryOperatorNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(NegateOperatorNode& node)
{
	Visit(static_cast<UnaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitwiseNotOperatorNode& node)
{
	Visit(static_cast<UnaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BooleanNotOperatorNode& node)
{
	Visit(static_cast<UnaryOperatorNode&>(node));
}


void NodeVisitor::Visit(BinaryOperatorNode& node)
{
	Visit(static_cast<ExpressionNode&>(node));
}

void NodeVisitor::Visit(AddNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(SubtractNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(MultiplyNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(DivideNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(ModulusNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(EqualNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(NotEqualNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(GreaterThanNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(LessThanNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(GreaterThanEqualNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(LessThanEqualNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BooleanAndNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BooleanOrNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitwiseAndNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitwiseOrNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitwiseXorNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitshiftLeftNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

void NodeVisitor::Visit(BitshiftRightNode& node)
{
	Visit(static_cast<BinaryOperatorNode&>(node));
}

}